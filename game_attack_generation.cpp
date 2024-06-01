#include <vector>
#include <cstdint>
#include <optional>
#include "game.h"
#include "enums.h"
#include "game_machine.h"
#include "attack.h"
#include "utils.h"

MachineState attack_causes_state(GameMachine &machine)
{
    if (machine.machine_state == MachineState::Sprinted)
        return MachineState::Overcharged;
    if (machine.has_moved())
        return MachineState::MovedAndAttacked;
    if (machine.has_attacked())
        return MachineState::Overcharged;
    return MachineState::Attacked;
}

void Game::populate_adjacent_attacks(GameMachine &machine, MachineDirection direction, Coord source_coodinates, std::optional<std::pair<Attack, std::optional<Attack>>> &attack, std::vector<Coord> &affected_machines)
{
    // If we do not posses the sweep skill, stop.
    if (machine.machine.get().skill != MachineSkill::Sweep)
        return;

    // Look to the left and right of the destination.
    for (auto sweep_direction : {
             rotate_direction(direction, Rotation::Clockwise),
             rotate_direction(direction, Rotation::CounterClockwise),
         })
    {
        auto sweep_destination = traverse_direction(source_coodinates, sweep_direction);
        if (sweep_destination.out_of_bounds())
            continue;

        auto destination_machine = board.machine_at(sweep_destination);
        if (destination_machine.has_value())
        {
            // If there is a machine to the left or right (enemy or friendly), then it is attacked as part of the main attack.
            affected_machines.push_back(sweep_destination);

            // If the machine to the left or right is an enemy, and we have not already found an enemy to attack, then this is the main attack.
            if (!attack.has_value() && destination_machine.value().side != machine.side)
            {
                attack = std::make_pair(Attack(
                                            direction,
                                            source_coodinates,
                                            machine.coordinates,
                                            attack_causes_state(machine),
                                            machine.machine_state == MachineState::Ready),
                                        std::nullopt);

                // If we only have one machine and it has moved and if we haven't already moved two machines, we can attack again as if it were a second machine.
                if (get_turn_machine_count() == 1 && machine.has_moved() && !player_touched_required_machines())
                {
                    attack.value().second = attack.value().first; // This should copy
                    attack.value().second.value().causes_state = MachineState::Attacked;
                    attack.value().second.value().counts_as_touch = true;
                }
            }
        }
    }

    // If there is a machine right at the destination, then the only way we could have gotten here
    // is if the machine at the destination is a friendly. Since we have the sweep skill, the friendly must be attacked too 😭.
    if (attack.has_value() && board.machine_at(source_coodinates).has_value())
    {
        affected_machines.push_back(source_coodinates);
    }
}

std::optional<std::pair<Attack, std::optional<Attack>>> Game::first_machine_in_attack_range(MachineDirection direction, GameMachine &machine)
{
    std::vector<Coord> affected_machines;
    std::optional<std::pair<Attack, std::optional<Attack>>> attack;

    for (int i = 0; i < machine.machine.get().range && !attack.has_value(); ++i)
    {
        // Move one space in the direction.
        auto destination = traverse_direction(machine.coordinates, direction, i + 1);

        // If the destination is out of bounds, stop.
        if (destination.out_of_bounds())
            break;

        // Is the destination occupied by an enemy machine?
        auto destination_machine = board.machine_at(destination);
        if (destination_machine.has_value() && destination_machine.value().side != machine.side)
        {
            attack = std::make_pair(Attack(
                                        direction,
                                        destination,
                                        machine.coordinates,
                                        attack_causes_state(machine),
                                        machine.machine_state == MachineState::Ready),
                                    std::nullopt);

            // If we only have one machine and it has moved and if we haven't already moved two machines, we can attack again as if it were a second machine.
            if (get_turn_machine_count() == 1 && machine.has_moved() && !player_touched_required_machines())
            {
                attack.value().second = attack.value().first; // This should copy
                attack.value().second.value().causes_state = MachineState::Attacked;
                attack.value().second.value().counts_as_touch = true;
            }

            affected_machines.push_back(destination);
        }

        populate_adjacent_attacks(machine, direction, destination, attack, affected_machines);
    }

    if (attack.has_value())
    {
        attack.value().first.affected_machines = affected_machines;
        if (attack.value().second.has_value())
            attack.value().second.value().affected_machines = affected_machines;
    }

    return attack;
}

std::vector<Attack> Game::calculate_attacks(GameMachine &machine)
{
    std::vector<Attack> attacks;

    if (machine.side != turn) // If it's not our turn, we can't attack
        return attacks;

    if (machine.machine_state == MachineState::Overcharged && (get_turn_machine_count() > 1 || player_touched_required_machines()))
        return attacks;

    for (auto direction : {
             MachineDirection::North,
             MachineDirection::East,
             MachineDirection::South,
             MachineDirection::West,
         })
    {
        switch (machine.machine.get().machine_type)
        {
        case MachineType::Melee:
        case MachineType::Ram:
        case MachineType::Swoop:
        case MachineType::Pull:
        {
            auto attack = first_machine_in_attack_range(direction, machine);
            if (attack.has_value())
            {
                attacks.push_back(attack.value().first);
                if (attack.value().second.has_value())
                    attacks.push_back(attack.value().second.value());
            }

            break;
        }
        case MachineType::Gunner:
        {
            std::vector<Coord> affected_machines;
            std::optional<std::pair<Attack, std::optional<Attack>>> main_attack;
            auto end_of_attack_range = traverse_direction(machine.coordinates, direction, machine.machine.get().range);
            if (end_of_attack_range.out_of_bounds())
                continue;

            auto destination_machine = board.machine_at(end_of_attack_range);
            if (destination_machine.has_value() && destination_machine.value().side != machine.side)
            {
                main_attack = std::make_pair(Attack(
                                                 direction,
                                                 end_of_attack_range,
                                                 machine.coordinates,
                                                 attack_causes_state(machine),
                                                 machine.machine_state == MachineState::Ready),
                                             std::nullopt);

                // If we only have one machine and it has moved and if we haven't already moved two machines, we can attack again as if it were a second machine.
                if (get_turn_machine_count() == 1 && machine.has_moved() && !player_touched_required_machines())
                {
                    main_attack.value().second = main_attack.value().first; // This should copy
                    main_attack.value().second.value().causes_state = MachineState::Attacked;
                    main_attack.value().second.value().counts_as_touch = true;
                }

                affected_machines.push_back(end_of_attack_range);
            }

            populate_adjacent_attacks(machine, direction, end_of_attack_range, main_attack, affected_machines);

            if (main_attack.has_value())
            {
                main_attack.value().first.affected_machines = affected_machines;
                attacks.push_back(main_attack.value().first);

                if (main_attack.value().second.has_value())
                {
                    main_attack.value().second.value().affected_machines = affected_machines;
                    attacks.push_back(main_attack.value().second.value());
                }
            }

            break;
        }
        case MachineType::Dash:
        {
            auto end_of_attack_range = traverse_direction(machine.coordinates, direction, machine.machine.get().range);
            if (end_of_attack_range.out_of_bounds())
                continue;

            // Must be able to land on an empty space
            if (board.machine_at(end_of_attack_range).has_value())
                continue;

            // Grab the attack for the first enemy in the path.
            // Normally, to account for the sweep skill for dash machines that have an attack range greater than 2,
            // we'd have to loop over all the spaces in the path and consider all the adjacent machines in the path.
            // However, because there are no dash machines with the sweep skill that have an attack range greater than 2,
            // we can just grab the first enemy in the path and it should have all the information we need.
            auto first_enemy_in_path = first_machine_in_attack_range(direction, machine);
            if (first_enemy_in_path.has_value())
            {
                // The source and affected machines should already be what we want them to be.
                first_enemy_in_path.value().first.destination = end_of_attack_range;
                attacks.push_back(first_enemy_in_path.value().first);
                if (first_enemy_in_path.value().second.has_value())
                {
                    first_enemy_in_path.value().second.value().destination = end_of_attack_range;
                    attacks.push_back(first_enemy_in_path.value().second.value());
                }
            }
        }
        }
    }

    return attacks;
}

int32_t Game::get_skill_combat_power_modifier_when_defending(GameMachine &machine)
{
    switch (machine.machine.get().skill)
    {
    case MachineSkill::Shield:
        return 1;
    default:
        return 0;
    }
}

int32_t Game::get_skill_combat_power_modifier_when_attacking(GameMachine &machine)
{
    int32_t combat_power = 0;
    switch (machine.machine.get().skill)
    {
    case MachineSkill::Gallop:
        if (board.terrain_at(machine.coordinates) == Terrain::Grassland)
            combat_power = 1;
        break;
    case MachineSkill::Stalk:
        if (board.terrain_at(machine.coordinates) == Terrain::Forest)
            combat_power = 1;
        break;
    case MachineSkill::HighGround:
        if (board.terrain_at(machine.coordinates) == Terrain::Mountain)
            combat_power = 1;
        break;
    case MachineSkill::Climb:
        if (board.terrain_at(machine.coordinates) == Terrain::Hill)
            combat_power = 1;
        break;
    default:
        break;
    }

    return combat_power + machine.attack_power_modifier;
}

// If the second argument is std::nullopt, then a machine's armor will be ignored from the calculation (should only be used for printing the board).
int32_t Game::calculate_combat_power(GameMachine &machine, std::optional<MachineDirection> attack_direction)
{
    // A defending machine's combat power is only the terrain it is standing on, plus any modifiers.
    // An attacking machine's combat power is the terrain it is standing on, plus any modifiers, plus its attack power.
    int32_t combat_power = static_cast<int32_t>(board.terrain_at(machine.coordinates));

    // If the machine is a pull machine and the terrain is marsh, add 1 combat power.
    if (machine.machine.get().is_pull() && board.terrain_at(machine.coordinates) == Terrain::Marsh)
        ++combat_power;

    // All swoop machines get +1 combat power.
    if (machine.machine.get().is_flying())
        ++combat_power;

    if (machine.side == turn)
    {
        combat_power += machine.machine.get().attack;
        combat_power += get_skill_combat_power_modifier_when_attacking(machine);
        return combat_power;
    }
    else
    {
        if (attack_direction.has_value())
        {
            auto side_being_attacked = side_tangent_to_direction(attack_direction.value(), machine.direction);
            if (static_cast<int32_t>(side_being_attacked & machine.machine.get().armored_sides) != 0)
                combat_power += 1;
            else if (static_cast<int32_t>(side_being_attacked & machine.machine.get().weak_sides) != 0)
                combat_power -= 1;
        }

        combat_power += get_skill_combat_power_modifier_when_defending(machine);
        return combat_power;
    }
}

bool Game::is_in_attack_range(GameMachine &attacker, GameMachine &defender)
{
    // Both the attacker and the defender must be on either the same row or the same column.
    if (attacker.coordinates.row != defender.coordinates.row && attacker.coordinates.column != defender.coordinates.column)
        return false;

    // Get the direction from the attacker to the defender.
    auto direction = get_direction(attacker.coordinates, defender.coordinates);

    // See if the defender is on the attack path of the attacker.
    for (int i = 1; i < attacker.machine.get().range; ++i)
    {
        auto coord = traverse_direction(attacker.coordinates, direction, i);
        if (coord == defender.coordinates)
            return true;
    }

    return false;
}
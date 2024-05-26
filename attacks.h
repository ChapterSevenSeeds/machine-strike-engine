#pragma once
#include <vector>
#include <cstdint>
#include <optional>
#include "game.h"
#include "enums.h"
#include "game_machine.h"

class Attack
{
public:
    // The attack direction.
    MachineDirection attack_direction_from_source;
    // The attack destination. Can point to an empty space or a machine.
    Coord destination;
    // The source of the attack. Always points to the attacking machine.
    Coord source;
    // These coordinates should always point to machines on the board.
    std::vector<Coord> affected_machines;

    Attack(
        MachineDirection attack_direction_from_source,
        Coord destination,
        Coord source) : attack_direction_from_source(attack_direction_from_source),
                        destination(destination),
                        source(source) {}
};

void populate_adjacent_attacks(Game &game, GameMachine &machine, MachineDirection direction, Coord source_coodinates, std::optional<Attack> &attack, std::vector<Coord> &affected_machines)
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

        auto destination_machine = game.board.machine_at(sweep_destination);
        if (destination_machine.has_value())
        {
            // If there is a machine to the left or right (enemy or friendly), then it is attacked as part of the main attack.
            affected_machines.push_back(sweep_destination);

            // If the machine to the left or right is an enemy, and we have not already found an enemy to attack, then this is the main attack.
            if (!attack.has_value() && destination_machine.value().get().side != machine.side)
            {
                attack = Attack(
                    direction,
                    source_coodinates,
                    machine.coordinates);
            }
        }
    }
}

std::optional<Attack> first_machine_in_attack_range(MachineDirection direction, Game &game, GameMachine &machine)
{
    std::vector<Coord> affected_machines;
    std::optional<Attack> attack;

    for (int i = 0; i < machine.machine.get().range && !attack.has_value(); ++i)
    {
        // Move one space in the direction.
        auto destination = traverse_direction(machine.coordinates, direction);

        // If the destination is out of bounds, stop.
        if (destination.out_of_bounds())
            break;

        // Is the destination occupied by an enemy machine?
        auto destination_machine = game.board.machine_at(destination);
        if (destination_machine.has_value() && destination_machine.value().get().side != machine.side)
        {
            attack = Attack(
                direction,
                destination,
                machine.coordinates);

            affected_machines.push_back(destination);
        }

        populate_adjacent_attacks(game, machine, direction, destination, attack, affected_machines);
    }

    if (attack.has_value())
        attack.value().affected_machines = affected_machines;

    return attack;
}

std::vector<Attack> calculate_attacks(Game &game, GameMachine &machine)
{
    std::vector<Attack> attacks;

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
            auto attack = first_machine_in_attack_range(direction, game, machine);
            if (attack.has_value())
                attacks.push_back(attack.value());

            break;
        }
        case MachineType::Gunner:
        {
            std::vector<Coord> affected_machines;
            std::optional<Attack> main_attack;
            auto end_of_attack_range = traverse_direction(machine.coordinates, direction, machine.machine.get().range);
            if (end_of_attack_range.out_of_bounds())
                continue;

            auto destination_machine = game.board.machine_at(end_of_attack_range);
            if (destination_machine.has_value() && destination_machine.value().get().side != machine.side)
            {
                main_attack = Attack(
                    direction,
                    end_of_attack_range,
                    machine.coordinates);

                affected_machines.push_back(end_of_attack_range);
            }

            populate_adjacent_attacks(game, machine, direction, end_of_attack_range, main_attack, affected_machines);

            if (main_attack.has_value())
            {
                main_attack.value().affected_machines = affected_machines;
                attacks.push_back(main_attack.value());
            }

            break;
        }
        case MachineType::Dash:
        {
            auto end_of_attack_range = traverse_direction(machine.coordinates, direction, machine.machine.get().range);
            if (end_of_attack_range.out_of_bounds())
                continue;

            // Must be able to land on an empty space
            if (game.board.machine_at(end_of_attack_range).has_value())
                continue;

            // Grab the attack for the first enemy in the path.
            // Normally, to account for the sweep skill for dash machines that have an attack range greater than 2,
            // we'd have to loop over all the spaces in the path and consider all the adjacent machines in the path.
            // However, because there are no dash machines with the sweep skill that have an attack range greater than 2,
            // we can just grab the first enemy in the path and it should have all the information we need.
            auto first_enemy_in_path = first_machine_in_attack_range(direction, game, machine);
            if (first_enemy_in_path.has_value())
            {
                // The source and affected machines should already be what we want them to be.
                first_enemy_in_path.value().destination = end_of_attack_range;
                attacks.push_back(first_enemy_in_path.value());
            }
        }
        }
    }

    return attacks;
}

MachineSide side_tangent_to_direction(MachineDirection source_direction, MachineDirection target_direction)
{
    switch (source_direction)
    {
    case MachineDirection::North:
    {
        switch (target_direction)
        {
        case MachineDirection::North:
            return MachineSide::Rear;
        case MachineDirection::East:
            return MachineSide::Right;
        case MachineDirection::South:
            return MachineSide::Front;
        case MachineDirection::West:
            return MachineSide::Left;
        }
    }
    case MachineDirection::East:
    {
        switch (target_direction)
        {
        case MachineDirection::North:
            return MachineSide::Left;
        case MachineDirection::East:
            return MachineSide::Rear;
        case MachineDirection::South:
            return MachineSide::Right;
        case MachineDirection::West:
            return MachineSide::Front;
        }
    }
    case MachineDirection::South:
    {
        switch (target_direction)
        {
        case MachineDirection::North:
            return MachineSide::Front;
        case MachineDirection::East:
            return MachineSide::Left;
        case MachineDirection::South:
            return MachineSide::Rear;
        case MachineDirection::West:
            return MachineSide::Right;
        }
    }
    case MachineDirection::West:
    {
        switch (target_direction)
        {
        case MachineDirection::North:
            return MachineSide::Right;
        case MachineDirection::East:
            return MachineSide::Front;
        case MachineDirection::South:
            return MachineSide::Left;
        case MachineDirection::West:
            return MachineSide::Rear;
        }
    }
    }
}

int32_t get_skill_combat_power_modifier_when_defending(Game &game, GameMachine &machine)
{
    switch (machine.machine.get().skill)
    {
    case MachineSkill::Shield:
        return 1;
    default:
        return 0;
    }
}

int32_t get_skill_combat_power_modifier_when_attacking(Game &game, GameMachine &machine)
{
    int32_t combat_power = 0;
    switch (machine.machine.get().skill)
    {
    case MachineSkill::Gallop:
        if (game.board.terrain_at(machine.coordinates) == Terrain::Grassland)
        {
            combat_power = 1;
        }
        break;
    case MachineSkill::Stalk:
        if (game.board.terrain_at(machine.coordinates) == Terrain::Forest)
        {
            combat_power = 1;
        }
        break;
    case MachineSkill::HighGround:
        if (game.board.terrain_at(machine.coordinates) == Terrain::Mountain)
        {
            combat_power = 1;
        }
        break;
    case MachineSkill::Climb:
        if (game.board.terrain_at(machine.coordinates) == Terrain::Hill)
        {
            combat_power = 1;
        }
        break;
    default:
        break;
    }

    return combat_power + machine.attack_power_modifier;
}

int32_t calculate_combat_power(Game &game, GameMachine &machine, Attack &attack)
{
    // A defending machine's combat power is only the terrain it is standing on, plus any modifiers.
    // An attacking machine's combat power is the terrain it is standing on, plus any modifiers, plus its attack power.
    int32_t combat_power = static_cast<int32_t>(game.board.terrain_at(machine.coordinates));

    // If the machine is a pull machine and the terrain is marsh, add 1 combat power.
    if (machine.machine.get().is_pull() && game.board.terrain_at(machine.coordinates) == Terrain::Marsh)
        ++combat_power;

    // All swoop machines get +1 combat power.
    if (machine.machine.get().is_flying())
        ++combat_power;

    if (machine.side == game.turn)
    {
        combat_power += machine.machine.get().attack;
        combat_power += get_skill_combat_power_modifier_when_attacking(game, machine);
        return combat_power;
    }
    else
    {
        auto side_being_attacked = side_tangent_to_direction(attack.attack_direction_from_source, machine.direction);
        if (static_cast<int32_t>(side_being_attacked & machine.machine.get().armored_sides) != 0)
            combat_power += 1;
        else if (static_cast<int32_t>(side_being_attacked & machine.machine.get().weak_sides) != 0)
            combat_power -= 1;

        combat_power += get_skill_combat_power_modifier_when_defending(game, machine);
        return combat_power;
    }
}

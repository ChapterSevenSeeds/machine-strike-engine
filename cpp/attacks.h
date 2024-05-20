#pragma once
#include <vector>
#include <cstdint>
#include "game.h"
#include "enums.h"
#include "game_machine.h"

class Attack
{
public:
    MachineDirection attack_direction_from_source;
    int32_t attacked_row;
    int32_t attacked_column;
    int32_t source_row;
    int32_t source_column;

    Attack(
        MachineDirection attack_direction_from_source,
        int32_t attacked_row,
        int32_t attacked_column,
        int32_t source_row,
        int32_t source_column) : attack_direction_from_source(attack_direction_from_source),
                                 attacked_row(attacked_row),
                                 attacked_column(attacked_column),
                                 source_row(source_row),
                                 source_column(source_column) {}
};

Attack *first_machine_in_attack_range(MachineDirection direction, Game &game, GameMachine &machine)
{
    int32_t row = machine.row;
    int32_t column = machine.column;
    for (int i = 0; i < machine.machine.get().range; ++i)
    {
        switch (direction)
        {
        case MachineDirection::North:
            row -= 1;
            break;
        case MachineDirection::East:
            column += 1;
            break;
        case MachineDirection::South:
            row += 1;
            break;
        case MachineDirection::West:
            column -= 1;
            break;
        }

        if (row >= 8 || column >= 8 || row < 0 || column < 0)
        {
            break;
        }

        if (game.machines[row][column].has_value() && game.machines[row][column].value().get().side != machine.side)
        {
            return new Attack(
                direction,
                row,
                column,
                machine.row,
                machine.column);
        }
    }

    return nullptr;
}

std::vector<Attack *> calculate_attacks(Game &game, GameMachine &machine)
{
    std::vector<Attack *> attacks;

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
            if (attack != nullptr)
            {
                attacks.push_back(attack);
            }
            break;
        }
        case MachineType::Gunner:
        {
            auto row = machine.row;
            auto column = machine.column;
            switch (direction)
            {
            case MachineDirection::North:
            {
                if (row - machine.machine.get().range < 0)
                {
                    continue;
                }

                row -= machine.machine.get().range;
                break;
            }
            case MachineDirection::East:
            {
                if (column + machine.machine.get().range > 7)
                {
                    continue;
                }
                column += machine.machine.get().range;
                break;
            }
            case MachineDirection::South:
            {
                if (row - machine.machine.get().range > 7)
                {
                    continue;
                }
                row += machine.machine.get().range;
                break;
            }
            case MachineDirection::West:
            {
                if (column - machine.machine.get().range < 0)
                {
                    continue;
                }
                column -= machine.machine.get().range;
                break;
            }
            }

            if (game.machines[row][column].has_value() && game.machines[row][column].value().get().side != machine.side)
            {
                attacks.push_back(new Attack(
                    direction,
                    row,
                    column,
                    machine.row,
                    machine.column));
            }

            break;
        }
        case MachineType::Dash:
        {
            auto row = machine.row;
            auto column = machine.column;
            switch (direction)
            {
            case MachineDirection::North:
            {
                if (row - machine.machine.get().range < 0)
                {
                    continue;
                }

                row -= machine.machine.get().range;
                break;
            }
            case MachineDirection::East:
            {
                if (column + machine.machine.get().range > 7)
                {
                    continue;
                }
                column += machine.machine.get().range;
                break;
            }
            case MachineDirection::South:
            {
                if (row - machine.machine.get().range > 7)
                {
                    continue;
                }
                row += machine.machine.get().range;
                break;
            }
            case MachineDirection::West:
            {
                if (column - machine.machine.get().range < 0)
                {
                    continue;
                }
                column -= machine.machine.get().range;
                break;
            }
            }

            // Must be able to land on an empty space
            if (game.machines[row][column].has_value())
            {
                continue;
            }

            // Does there exist at least one enemy machine in the path?
            if (first_machine_in_attack_range(direction, game, machine) != nullptr) // Fix this memory leak
            {
                attacks.push_back(new Attack(
                    direction,
                    row,
                    column,
                    machine.row,
                    machine.column));
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
        if (game.terrain[machine.row][machine.column] == Terrain::Grassland)
        {
            combat_power = 1;
        }
        break;
    case MachineSkill::Stalk:
        if (game.terrain[machine.row][machine.column] == Terrain::Forest)
        {
            combat_power = 1;
        }
        break;
    case MachineSkill::HighGround:
        if (game.terrain[machine.row][machine.column] == Terrain::Mountain)
        {
            combat_power = 1;
        }
        break;
    case MachineSkill::Climb:
        if (game.terrain[machine.row][machine.column] == Terrain::Hill)
        {
            combat_power = 1;
        }
        break;
    default:
        break;
    }

    // Apply any attack power gains from friendly empower machines and subtract any attack power lost from enemy blinding machines.
    let modifier = game.count_machines_with_skill_able_to_attack_target_machine(
                       machine,
                       MachineSkill::Empower,
                       Player::Player, ) -
                   game.count_machines_with_skill_able_to_attack_target_machine(
                       machine,
                       MachineSkill::Blind,
                       Player::Opponent, );

    return combat_power + modifier;
}

int32_t calculate_combat_power(Game &game, GameMachine &machine, Attack &attack)
{
    // A defending machine's combat power is only the terrain it is standing on, plus any modifiers.
    // An attacking machine's combat power is the terrain it is standing on, plus any modifiers, plus its attack power.
    int32_t combat_power = static_cast<int32_t>(game.terrain[machine.row][machine.column]);

    // If the machine is a pull machine and the terrain is marsh, add 1 combat power.
    if (machine
            .machine.get()
            .is_pull() &&
        game.terrain[machine.row][machine.column] == Terrain::Marsh)
    {
        combat_power += 1;
    }

    // All swoop machines get +1 combat power.
    if (machine
            .machine.get()
            .is_flying())
    {
        combat_power += 1;
    }

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
        {
            combat_power += 1;
        }
        else if (static_cast<int32_t>(side_being_attacked & machine.machine.get().weak_sides) != 0)
        {
            combat_power -= 1;
        }
        combat_power += get_skill_combat_power_modifier_when_defending(game, machine);
        return combat_power;
    }
}

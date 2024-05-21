#include <algorithm>
#include <cstdint>
#include "moves.h"
#include "game_machine.h"
#include "game.h"
#include "enums.h"
#include "attacks.h"

int32_t Game::count_machines_with_skill_able_to_attack_target_machine(
    GameMachine &target,
    MachineSkill attacking_machine_skill,
    Player attacking_side)
{
    int32_t total = 0;
    for (const auto &machine_row : board.machines)
    {
        for (const auto &machine : machine_row)
        {
            if (!machine.has_value() || machine->get().machine.get().skill != attacking_machine_skill || machine->get().side != attacking_side)
                continue;

            auto attacks = calculate_attacks(*this, machine->get());
            total += std::count_if(attacks.begin(), attacks.end(), [&target](const Attack &attack)
                                   { return attack.attacked_row == target.row && attack.attacked_column == target.column; });
        }
    }

    return total;
}

void Game::make_move(Move &m)
{
    auto &machine = board.machines[m.source_row][m.source_column];
    if (m.requires_sprint)
    {
        machine.value().get().machine_state = MachineState::Sprinted;
    }
    else
    {
        machine.value().get().machine_state = MachineState::Moved;
    }

    board.unsafe_move_machine(
        m.source_row,
        m.source_column,
        m.destination_row,
        m.destination_column);
}

void Game::make_attack(Attack &attack)
{
    auto &attacker = board.machines[attack.source_row][attack.source_column].value().get();
    auto attacker_combat_power = calculate_combat_power(*this, attacker, attack);

    if (attacker.machine.get().machine_type == MachineType::Dash)
    {
        // When attacking everything in its path, is the combat power of friendly machines used as if they were enemy machines?
        todo("Dash attacks are not implemented yet");
    }

    auto &defender = board.machines[attack.attacked_row][attack.attacked_column].value().get();
    auto defender_combat_power = calculate_combat_power(*this, defender, attack);

    if (attacker_combat_power <= defender_combat_power)
    {
        todo("Perform defense break"); // Can defense break trigger a knockback?
    }

    // Apply the attack
    defender.health -= attacker_combat_power - defender_combat_power;

    switch (attacker.machine.get().machine_type)
    {
    case MachineType::Gunner:
    case MachineType::Melee:
        // Attack happens and nothing else.
        attacker.machine_state = MachineState::Attacked; // Attacked and moved?
        break;
    case MachineType::Pull:
        // Can a pull ram knock a machine into a chasm? If so, does anything happen?
        // Pull the enemy one terrain closer to it.
        int32_t pulled_to_row = 0;
        int32_t pulled_to_column = 0;
        switch (attack.attack_direction_from_source)
        {
        case MachineDirection::North:
            pulled_to_row = attack.attacked_row + 1;
            pulled_to_column = attack.attacked_column;
            break;
        case MachineDirection::East:
            pulled_to_row = attack.attacked_row;
            pulled_to_column = attack.attacked_column - 1;
            break;
        case MachineDirection::South:
            pulled_to_row = attack.attacked_row - 1;
            pulled_to_column = attack.attacked_column;
            break;
        case MachineDirection::West:
            pulled_to_row = attack.attacked_row;
            pulled_to_column = attack.attacked_column + 1;
            break;
        }

        if (board.machines[pulled_to_row][pulled_to_column].has_value())
        {
            // Apply one damage point to the machine that was pulled and to the machine occupying the space it was pulled to.
            board.machines[pulled_to_row][pulled_to_column].value().get().health -= 1;
            defender.health -= 1;
        }
        else
        {
            board.unsafe_move_machine(
                attack.attacked_row,
                attack.attacked_column,
                pulled_to_row,
                pulled_to_column);
        }
        break;
    case MachineType::Ram:
        // Push the enemy one terrain away from it and move into the spot it was pushed from.
        int32_t pushed_to_row = 0;
        int32_t pushed_to_column = 0;
        int32_t fallback_row = 0;
        int32_t fallback_column = 0;
        switch (attack.attack_direction_from_source)
        {
        case MachineDirection::North:
            pushed_to_row = attack.attacked_row - 1;
            pushed_to_column = attack.attacked_column;
            fallback_row = attack.attacked_row + 1;
            fallback_column = attack.attacked_column;
            break;
        case MachineDirection::East:
            pushed_to_row = attack.attacked_row;
            pushed_to_column = attack.attacked_column + 1;
            fallback_row = attack.attacked_row;
            fallback_column = attack.attacked_column - 1;
            break;
        case MachineDirection::South:
            pushed_to_row = attack.attacked_row + 1;
            pushed_to_column = attack.attacked_column;
            fallback_row = attack.attacked_row - 1;
            fallback_column = attack.attacked_column;
            break;
        case MachineDirection::West:
            pushed_to_row = attack.attacked_row;
            pushed_to_column = attack.attacked_column - 1;
            fallback_row = attack.attacked_row;
            fallback_column = attack.attacked_column + 1;
            break;
        }

        if (board.machines[pushed_to_row][pushed_to_column].has_value())
        {
            // Apply one damage point to the machine that was pushed and to the machine occupying the space it was pushed to.
            board.machines[pushed_to_row][pushed_to_column].value().get().health -= 1;
            defender.health -= 1;

            // Move the machine to the next spot.
            // Unless the attack was malformed, the fallback spot should always be empty.
            board.unsafe_move_machine(
                attack.source_row,
                attack.source_column,
                fallback_row,
                fallback_column);
        }
        else
        {
            board.unsafe_move_machine(
                attack.attacked_row,
                attack.attacked_column,
                pushed_to_row,
                pushed_to_column);
        }
        break;
    case MachineType::Dash:
        todo("Dash attacks are not implemented yet");
    }
}
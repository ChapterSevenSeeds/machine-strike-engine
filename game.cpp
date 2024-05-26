#include <algorithm>
#include <cstdint>
#include "moves.h"
#include "game_machine.h"
#include "game.h"
#include "enums.h"
#include "attacks.h"
#include "utils.h"

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
    switch (board.machine_at(attack.source).value().get().machine.get().machine_type)
    {
    case MachineType::Dash:
        perform_dash_attack(attack);
        break;
    case MachineType::Gunner:
        perform_gunner_attack(attack);
        break;
    case MachineType::Melee:
        perform_melee_attack(attack);
        break;
    case MachineType::Pull:
        perform_pull_attack(attack);
        break;
    case MachineType::Ram:
        perform_ram_attack(attack);
        break;
    case MachineType::Swoop:
        perform_swoop_attack(attack);
        break;
    }
}

void Game::pre_turn()
{
    for (const auto &machine : board)
    {
        machine.value().get().attack_power_modifier = 0;

        if (!machine.has_value())
            continue;

        auto &machine_ref = machine.value().get();

        switch (machine_ref.machine.get().skill)
        {
        case MachineSkill::Spray:
        {
            auto attacks = calculate_attacks(*this, machine_ref);
            for (const auto &attack : attacks)
            {
                if (!board.is_space_occupied(attack.attacked_row, attack.attacked_column))
                    continue;
                --board.machine_at[attack.attacked_row][attack.attacked_column].value().get().health;
            }
            break;
        }
        case MachineSkill::Whiplash:
        {
            auto attacks = calculate_attacks(*this, machine_ref);
            for (const auto &attack : attacks)
            {
                if (!board.is_space_occupied(attack.attacked_row, attack.attacked_column))
                    continue;
                board.machines[attack.attacked_row][attack.attacked_column].value().get().direction = opposite_direction(board.machines[attack.attacked_row][attack.attacked_column].value().get().direction);
            }
            break;
        }
        case MachineSkill::Empower:
        {
            auto attacks = calculate_attacks(*this, machine_ref);
            for (const auto &attack : attacks)
            {
                if (!board.is_space_occupied(attack.attacked_row, attack.attacked_column) || board.machines[attack.attacked_row][attack.attacked_column].value().get().side != machine_ref.side)
                    continue;

                ++board.machines[attack.attacked_row][attack.attacked_column].value().get().attack_power_modifier;
            }
            break;
        }
        case MachineSkill::Blind:
        {
            auto attacks = calculate_attacks(*this, machine_ref);
            for (const auto &attack : attacks)
            {
                if (!board.is_space_occupied(attack.attacked_row, attack.attacked_column) || board.machines[attack.attacked_row][attack.attacked_column].value().get().side == machine_ref.side)
                    continue;

                --board.machines[attack.attacked_row][attack.attacked_column].value().get().attack_power_modifier;
            }
            break;
        }
        }
    }
}
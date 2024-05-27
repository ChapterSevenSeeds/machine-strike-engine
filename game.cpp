#include <algorithm>
#include <cstdint>
#include "move.h"
#include "game_machine.h"
#include "game.h"
#include "attack.h"
#include "utils.h"

void Game::make_move(Move &m)
{
    auto &machine = board.machine_at(m.source).value().get();
    if (machine.machine_state == MachineState::Sprinted || machine.machine_state == MachineState::Moved || machine.machine_state == MachineState::MovedAndAttacked)
        machine.machine_state = MachineState::Overcharged;
    else
    {
        if (m.requires_sprint)
        {
            machine.machine_state = MachineState::Sprinted;
        }
        else
        {
            machine.machine_state = MachineState::Moved;
        }
    }

    board.unsafe_move_machine(m.source, m.destination);
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
        auto &machine_ref = machine.value().get();
        machine_ref.attack_power_modifier = 0;

        switch (machine_ref.machine.get().skill)
        {
        case MachineSkill::Spray:
        {
            for (const auto &other_machine : board)
            {
                auto &other_machine_ref = other_machine.value().get();
                if (&machine_ref == &other_machine_ref)
                    continue;

                if (is_in_attack_range(machine_ref, other_machine_ref))
                    --other_machine_ref.health;
            }
            break;
        }
        case MachineSkill::Whiplash:
        {
            for (const auto &other_machine : board)
            {
                auto &other_machine_ref = other_machine.value().get();
                if (&machine_ref == &other_machine_ref)
                    continue;

                if (is_in_attack_range(machine_ref, other_machine_ref))
                    other_machine_ref.direction = opposite_direction(other_machine_ref.direction);
            }
            break;
        }
        case MachineSkill::Empower:
        {
            for (const auto &other_machine : board)
            {
                auto &other_machine_ref = other_machine.value().get();
                if (&machine_ref == &other_machine_ref || other_machine_ref.side != machine_ref.side)
                    continue;

                if (is_in_attack_range(machine_ref, other_machine_ref))
                    ++other_machine_ref.attack_power_modifier;
            }
            break;
        }
        case MachineSkill::Blind:
        {
            for (const auto &other_machine : board)
            {
                auto &other_machine_ref = other_machine.value().get();
                if (&machine_ref == &other_machine_ref || other_machine_ref.side == machine_ref.side)
                    continue;

                if (is_in_attack_range(machine_ref, other_machine_ref))
                    --other_machine_ref.attack_power_modifier;
            }
            break;
        }
        }
    }
}
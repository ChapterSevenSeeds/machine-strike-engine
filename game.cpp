#include <algorithm>
#include <cstdint>
#include "moves.h"
#include "game_machine.h"
#include "game.h"
#include "enums.h"
#include "attacks.h"
#include "utils.h"

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
    switch (board.machines[attack.source_row][attack.source_column].value().get().machine.get().machine_type)
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
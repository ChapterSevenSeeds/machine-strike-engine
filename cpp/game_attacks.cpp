#include "enums.h"
#include "game.h"
#include "attacks.h"
#include "utils.h"

void post_attack(GameMachine &attacker)
{
    attacker.machine_state = attacker.machine_state == MachineState::Moved ? MachineState::MovedAndAttacked : MachineState::Attacked;
}

void Game::perform_dash_attack(Attack &attack)
{
    auto &attacker = board.machines[attack.source_row][attack.source_column].value().get();
    auto attacker_combat_power = calculate_combat_power(*this, attacker, attack);

    // Get every machine in the attack path and apply the attack. Also rotate each machine 180 degrees.
    auto current_row = attack.source_row;
    auto current_column = attack.source_column;

    while (current_row != attack.attacked_row && current_column != attack.attacked_column)
    {
        auto next = traverse_direction(current_row, current_column, attack.attack_direction_from_source);
        current_row = next.first;
        current_column = next.second;

        if (board.machines[current_row][current_column].has_value())
        {
            auto &machine = board.machines[current_row][current_column].value().get();
            // How does this work with the defender's combat power? Are there multiple defense breaks? What about friendly machines?
            auto machine_combat_power = calculate_combat_power(*this, machine, attack);
            machine.health -= attacker_combat_power - machine_combat_power;
            rotate_machine(machine.direction, Rotation::Clockwise, 2);
        }
    }

    board.unsafe_move_machine(
        attack.source_row,
        attack.source_column,
        attack.attacked_row,
        attack.attacked_column);

    post_attack(attacker);
}

std::tuple<GameMachine &, int32_t, GameMachine &, int32_t> perform_direct_attack_prelude(Game &game, Attack &attack)
{
    auto &attacker = game.board.machines[attack.source_row][attack.source_column].value().get();
    auto attacker_combat_power = calculate_combat_power(game, attacker, attack);
    auto &defender = game.board.machines[attack.attacked_row][attack.attacked_column].value().get();
    auto defender_combat_power = calculate_combat_power(game, defender, attack);

    if (attacker_combat_power <= defender_combat_power)
    {
        todo("Perform defense break"); // Can defense break trigger a knockback?
    }
    else
    {
        // Apply the attack
        defender.health -= attacker_combat_power - defender_combat_power;
    }

    return {defender, defender_combat_power, attacker, attacker_combat_power};
}

void Game::perform_gunner_attack(Attack &attack)
{
    auto [_, _, attacker, _] = perform_direct_attack_prelude(*this, attack);
    post_attack(attacker);
}

void Game::perform_melee_attack(Attack &attack)
{
    auto [_, _, attacker, _] = perform_direct_attack_prelude(*this, attack);
    post_attack(attacker);
}

void Game::perform_pull_attack(Attack &attack)
{
    auto [defender, _, attacker, _] = perform_direct_attack_prelude(*this, attack);
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

    post_attack(attacker);
}

void Game::perform_ram_attack(Attack &attack)
{
    auto [defender, _, attacker, _] = perform_direct_attack_prelude(*this, attack);

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

    post_attack(attacker);
}
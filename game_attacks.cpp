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

// Returns true if the machine was knocked one space, false otherwise.
bool Game::knock_machine(GameMachine &machine, MachineDirection direction)
{
    auto [next_row, next_column] = traverse_direction(machine.row, machine.column, direction);
    if (next_row < 0 || next_row >= 8 || next_column < 0 || next_column >= 8)
    {
        // The machine is knocked into the wall and loses 1 health.
        machine.health--;
        return false;
    }

    if (board.machines[next_row][next_column].has_value())
    {
        // The machine is knocked into another machine and loses 1 health.
        machine.health--;
        board.machines[next_row][next_column].value().get().health--;
        return false;
    }

    // The machine is knocked back one space. What about dash and pull attacks?
    board.unsafe_move_machine(machine.row, machine.column, next_row, next_column);
    return true;
}

std::tuple<GameMachine &, int32_t, GameMachine &, int32_t> perform_direct_attack_prelude(Game &game, Attack &attack)
{
    auto &attacker = game.board.machines[attack.source_row][attack.source_column].value().get();
    auto attacker_combat_power = calculate_combat_power(game, attacker, attack);
    auto &defender = game.board.machines[attack.attacked_row][attack.attacked_column].value().get();
    auto defender_combat_power = calculate_combat_power(game, defender, attack);

    if (attacker_combat_power <= defender_combat_power)
    {
        attacker.health--;
        defender.health--;

        // Does this apply to pull attacks and dash attacks?
        game.knock_machine(defender, attack.attack_direction_from_source);
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

    knock_machine(defender, opposite_direction(attack.attack_direction_from_source));

    post_attack(attacker);
}

void Game::perform_ram_attack(Attack &attack)
{
    auto [defender, _, attacker, _] = perform_direct_attack_prelude(*this, attack);

    auto knocked = knock_machine(defender, attack.attack_direction_from_source);

    if (knocked)
    {
        // The machine takes the spot of the machine that was knocked.
        board.unsafe_move_machine(
            attack.source_row,
            attack.source_column,
            attack.attacked_row,
            attack.attacked_column);
    }
    else
    {
        // The machine is moved to space next to the machine that was attacked.
        auto [next_row, next_column] = traverse_direction(attack.attacked_row, attack.attacked_column, opposite_direction(attack.attack_direction_from_source));
        board.unsafe_move_machine(
            attack.source_row,
            attack.source_column,
            next_row,
            next_column);
    }

    post_attack(attacker);
}

void Game::perform_swoop_attack(Attack &attack)
{
    auto [defender, _, attacker, _] = perform_direct_attack_prelude(*this, attack);

    // The attacker moves next to the defender.
    auto [next_row, next_column] = traverse_direction(attack.attacked_row, attack.attacked_column, opposite_direction(attack.attack_direction_from_source));
    board.unsafe_move_machine(
        attack.source_row,
        attack.source_column,
        next_row,
        next_column);

    post_attack(attacker);
}
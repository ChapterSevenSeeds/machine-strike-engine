#include "enums.h"
#include "game.h"
#include "attacks.h"
#include "utils.h"

void post_attack(GameMachine &attacker)
{
    attacker.machine_state =
        attacker.machine_state == MachineState::Attacked || attacker.machine_state == MachineState::MovedAndAttacked
            ? MachineState::Overcharged
        : attacker.machine_state == MachineState::Moved
            ? MachineState::MovedAndAttacked
            : MachineState::Attacked;
}

void Game::perform_dash_attack(Attack &attack)
{
    auto &attacker = board.machine_at(attack.source).value().get();
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
    auto moved_to = traverse_direction(machine.coordinates, direction);
    if (moved_to.out_of_bounds())
    {
        // The machine is knocked into the wall and loses 1 health.
        --machine.health;
        return false;
    }

    if (board.machine_at(moved_to).has_value())
    {
        // The machine is knocked into another machine and loses 1 health.
        --machine.health;
        --board.machine_at(moved_to).value().get().health;
        return false;
    }

    // The machine is knocked back one space.
    board.unsafe_move_machine(machine.coordinates, moved_to);
    return true;
}

GameMachine &perform_direct_attack_prelude(Game &game, Attack &attack)
{
    auto &attacker = game.board.machine_at(attack.source).value().get();
    auto attacker_combat_power = calculate_combat_power(game, attacker, attack);
    for (const auto &coord : attack.affected_machines)
    {
        auto &defender = game.board.machine_at(coord).value().get();
        auto defender_combat_power = calculate_combat_power(game, defender, attack);

        // Defense break.
        if (attacker_combat_power <= defender_combat_power)
        {
            attacker.health--;
            defender.health--;

            if (attacker.machine.get().machine_type != MachineType::Ram) // Ram attacks only knock the machine once, even if there was a defense break.
                game.knock_machine(defender, attack.attack_direction_from_source);
        }
        else
        {
            // Apply the attack
            defender.health -= attacker_combat_power - defender_combat_power;
        }
    }

    return attacker;
}

void Game::perform_gunner_attack(Attack &attack)
{
    auto &attacker = perform_direct_attack_prelude(*this, attack);
    post_attack(attacker);
}

void Game::perform_melee_attack(Attack &attack)
{
    auto &attacker = perform_direct_attack_prelude(*this, attack);
    post_attack(attacker);
}

void Game::perform_pull_attack(Attack &attack)
{
    auto attacker = perform_direct_attack_prelude(*this, attack);

    // I don't think any pull machines should be able to attack more than one machine at a time.
    // E.G. there are no pull machines with the swoop skill. Because of this, I have no way to confirm
    // how the pulling should work if a machine has the sweep skill. I am going to assume that all machines are pulled.
    for (const auto &coord : attack.affected_machines)
    {
        auto &defender = board.machine_at(coord).value().get();
        knock_machine(defender, opposite_direction(attack.attack_direction_from_source));
    }

    post_attack(attacker);
}

void Game::perform_ram_attack(Attack &attack)
{
    auto &attacker = perform_direct_attack_prelude(*this, attack);
    for (const auto &coord : attack.affected_machines)
    {
        auto &defender = board.machine_at(coord).value().get();
        knock_machine(defender, attack.attack_direction_from_source);
    }

    // Can the machine move to destination (the machine should have been knocked back one space)?
    if (!board.machine_at(attack.destination).has_value())
    {
        // The machine takes the spot of the machine that was knocked along the attack path.
        board.unsafe_move_machine(attack.source, attack.destination);
    }
    else
    {
        // The machine is moved to space next to the machine that was attacked.
        auto fallback_coord = traverse_direction(attack.destination, opposite_direction(attack.attack_direction_from_source));
        board.unsafe_move_machine(attack.source, fallback_coord);
    }

    post_attack(attacker);
}

void Game::perform_swoop_attack(Attack &attack)
{
    auto attacker = perform_direct_attack_prelude(*this, attack);

    // The attacker moves next to the defender along the attack path.
    auto destination = traverse_direction(attack.destination, opposite_direction(attack.attack_direction_from_source));
    board.unsafe_move_machine(attack.source, destination);

    post_attack(attacker);
}

void Game::perform_post_attack_skills(GameMachine &attacker, GameMachine &defender, Attack &attack)
{
    switch (attacker.machine.get().skill)
    {
    case MachineSkill::AlterTerrain:
        --board.terrain_at(attack.source);
        for (const auto &coord : attack.affected_machines)
            --board.terrain_at(coord);

        break;
    case MachineSkill::Burn:
        for (const auto &coord : attack.affected_machines)
        {
            if (board.terrain[coord.row][coord.column] == Terrain::Forest)
                board.terrain[coord.row][coord.column] = Terrain::Grassland;
        }
        break;
    case MachineSkill::Freeze:
        for (const auto &coord : attack.affected_machines)
        {
            if (board.terrain[coord.row][coord.column] == Terrain::Marsh)
                board.terrain[coord.row][coord.column] = Terrain::Grassland;
        }
        break;
    case MachineSkill::Growth:
        for (const auto &coord : attack.affected_machines)
        {
            if (board.terrain[coord.row][coord.column] == Terrain::Grassland)
                board.terrain[coord.row][coord.column] = Terrain::Forest;
        }
        break;
    }

    for (const auto &coord : attack.affected_machines)
    {
        auto &machine = board.machine_at(coord).value().get();
        if (machine.machine.get().skill != MachineSkill::Retaliate)
            continue;

        // Rotate the machine towards the attacker
        machine.direction = opposite_direction(attack.attack_direction_from_source);

        // Traverse the attack range of the defender and see if the attacker is within range.
        for (int i = 1; i <= machine.machine.get().range; i++)
        {
            auto new_coord = traverse_direction(coord, machine.direction, i);
            if (new_coord == attacker.coordinates)
            {
                // The attacker is within range and takes 1 damage.
                --attacker.health;
                break;
            }
        }
    }
}
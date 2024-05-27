#include "enums.h"
#include "game.h"
#include "attack.h"
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

GameMachine &Game::pre_apply_attack(Attack &attack)
{
    auto &attacker = board.machine_at(attack.source).value().get();
    auto attacker_combat_power = calculate_combat_power(attacker, attack);

    apply_attack(attack, attacker, attacker_combat_power);

    return attacker;
}

void Game::apply_attack(Attack &attack, GameMachine &attacker, uint32_t attacker_combat_power)
{
    for (const auto &coord : attack.affected_machines)
    {
        auto &defender = board.machine_at(coord).value().get();
        auto defender_combat_power = calculate_combat_power(defender, attack);

        // Defense break.
        if (attacker_combat_power <= defender_combat_power)
        {
            attacker.health--;
            defender.health--;

            if (attacker.machine.get().machine_type != MachineType::Ram) // Ram attacks only knock the machine once, even if there was a defense break.
                knock_machine(defender, attack.attack_direction_from_source);
        }
        else
        {
            // Apply the attack
            defender.health -= attacker_combat_power - defender_combat_power;
        }
    }
}

void Game::perform_dash_attack(Attack &attack)
{
    auto &attacker = board.machine_at(attack.source).value().get();
    auto attacker_combat_power = calculate_combat_power(attacker, attack);

    // Move the attacker to the destination immediately.
    board.unsafe_move_machine(attack.source, attack.destination);

    apply_attack(attack, attacker, attacker_combat_power);

    post_attack(attacker);
}

void Game::perform_gunner_attack(Attack &attack)
{
    auto &attacker = pre_apply_attack(attack);
    post_attack(attacker);
}

void Game::perform_melee_attack(Attack &attack)
{
    auto &attacker = pre_apply_attack(attack);
    post_attack(attacker);
}

void Game::perform_pull_attack(Attack &attack)
{
    auto attacker = pre_apply_attack(attack);

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
    auto &attacker = pre_apply_attack(attack);
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
    auto attacker = pre_apply_attack(attack);

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
            ++board.terrain_at(coord);

        break;
    case MachineSkill::Burn:
        for (const auto &coord : attack.affected_machines)
        {
            if (board.terrain_at(coord) == Terrain::Forest)
                board.terrain_at(coord) = Terrain::Grassland;
        }
        break;
    case MachineSkill::Freeze:
        for (const auto &coord : attack.affected_machines)
        {
            if (board.terrain_at(coord) == Terrain::Marsh)
                board.terrain_at(coord) = Terrain::Grassland;
        }
        break;
    case MachineSkill::Growth:
        for (const auto &coord : attack.affected_machines)
        {
            if (board.terrain_at(coord) == Terrain::Grassland)
                board.terrain_at(coord) = Terrain::Forest;
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
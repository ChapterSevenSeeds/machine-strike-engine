#include <algorithm>
#include <iostream>
#include <cstdint>
#include <ranges>
#include "move.h"
#include "game_machine.h"
#include "game.h"
#include "attack.h"
#include "utils.h"

bool Game::player_touched_required_machines() const
{
    return machines_touched >= 2;
}

int Game::get_turn_machine_count() const
{
    return turn == Player::Player ? player_machine_count : opponent_machine_count;
}

void Game::make_move(Move &m, bool single_machine_overcharge)
{
    auto &machine = board.machine_at(m.source).value();
    machine.machine_state = m.causes_state;
    if (m.counts_as_touch)
        ++machines_touched;

    board.move_machine(m.source, m.destination);
}

void Game::make_attack(Attack &attack)
{
    auto &attacker = board.machine_at(attack.source).value();
    attacker.direction = attack.attack_direction_from_source;
    switch (attacker.machine.get().machine_type)
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

    attacker.machine_state = attack.causes_state;
    if (attack.counts_as_touch)
        ++machines_touched;

    if (attacker.machine_state == MachineState::Overcharged)
        modify_machine_health(attacker, -2);
}

void Game::pre_turn()
{
    machines_touched = 0;
    for (auto &machine : board)
    {
        auto &machine_ref = machine.value();
        machine_ref.machine_state = MachineState::Ready;
        machine_ref.attack_power_modifier = 0;

        switch (machine_ref.machine.get().skill)
        {
        case MachineSkill::Spray:
        {
            for (auto &other_machine : board)
            {
                auto &other_machine_ref = other_machine.value();
                if (&machine_ref == &other_machine_ref)
                    continue;

                if (is_in_attack_range(machine_ref, other_machine_ref))
                    modify_machine_health(other_machine_ref, -1);
            }
            break;
        }
        case MachineSkill::Whiplash:
        {
            for (auto &other_machine : board)
            {
                auto &other_machine_ref = other_machine.value();
                if (&machine_ref == &other_machine_ref)
                    continue;

                if (is_in_attack_range(machine_ref, other_machine_ref))
                    other_machine_ref.direction = opposite_direction(other_machine_ref.direction);
            }
            break;
        }
        case MachineSkill::Empower:
        {
            for (auto &other_machine : board)
            {
                auto &other_machine_ref = other_machine.value();
                if (&machine_ref == &other_machine_ref || other_machine_ref.side != machine_ref.side)
                    continue;

                if (is_in_attack_range(machine_ref, other_machine_ref))
                    ++other_machine_ref.attack_power_modifier;
            }
            break;
        }
        case MachineSkill::Blind:
        {
            for (auto &other_machine : board)
            {
                auto &other_machine_ref = other_machine.value();
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

void Game::modify_machine_health(GameMachine &machine, int32_t health_change)
{
    machine.health += health_change;
    if (!machine.is_alive())
    {
        board.clear_spot(machine.coordinates);

        if (machine.side == Player::Player)
        {
            opponent_victory_points += machine.machine.get().points;
            player_machine_count--;
        }
        else
        {
            player_victory_points += machine.machine.get().points;
            opponent_machine_count--;
        }
    }
}

Winner Game::check_winner()
{
    if (player_victory_points >= 7 && opponent_victory_points < 7)
        return Winner::Player;
    if (opponent_victory_points >= 7 && player_victory_points < 7)
        return Winner::Opponent;
    if (player_victory_points >= 7 && opponent_victory_points >= 7)
        return turn == Player::Player ? Winner::Player : Winner::Opponent;

    return Winner::None;
}

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

std::string get_direction_string(MachineDirection direction)
{
    switch (direction)
    {
    case MachineDirection::North:
        return "|^";
    case MachineDirection::East:
        return "|->";
    case MachineDirection::South:
        return "|v";
    case MachineDirection::West:
        return "|<-";
    }

    throw std::invalid_argument("Invalid direction");
}

void Game::print_board(std::optional<GameMachine> focus_machine, std::optional<std::vector<Move>> moves, std::optional<std::vector<Attack>> attacks)
{
    std::cout << "Turn: " << (turn == Player::Player ? "Player" : "Opponent") << "\t";
    std::cout << "Player VP: " << player_victory_points << "\t";
    std::cout << "Opponent VP: " << opponent_victory_points << std::endl;
    std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

    for (int row = 0; row < 8; ++row)
    {
        // Print terrain
        for (int column = 0; column < 8; ++column)
        {
            Coord coord{row, column};
            auto terrain = board.terrain_at(coord);

            std::cout << "|";
            printf("%24s", to_string(terrain).c_str());
        }

        std::cout << "|" << std::endl;

        // Print machines
        for (int column = 0; column < 8; ++column)
        {
            Coord coord{row, column};
            auto machine = board.machine_at(coord);

            std::cout << "|";
            if (machine.has_value())
            {
                auto &machine_ref = machine.value();
                printf(machine_ref.side == turn ? GRN : RED);
                printf("%24s", ((focus_machine.has_value() && &machine_ref == &focus_machine.value() ? std::string("->") : "") + machine_ref.machine.get().name + get_direction_string(machine_ref.direction)).c_str());
                printf(RESET);
            }
            else
            {
                printf("%24s", "");
            }
        }

        std::cout << "|" << std::endl;

        // Print health
        for (int column = 0; column < 8; ++column)
        {
            Coord coord{row, column};
            auto machine = board.machine_at(coord);

            std::cout << "|";
            if (machine.has_value())
            {
                printf("%24s", std::string("Health: " + std::to_string(machine.value().health)).c_str());
            }
            else
            {
                printf("%24s", "");
            }
        }

        std::cout << "|" << std::endl;

        // Print combat power
        for (int column = 0; column < 8; ++column)
        {
            Coord coord{row, column};
            auto machine = board.machine_at(coord);

            std::cout << "|";
            if (machine.has_value())
            {
                printf("%24s", std::string("Combat: " + std::to_string(calculate_combat_power(machine.value(), std::nullopt))).c_str());
            }
            else
            {
                printf("%24s", "");
            }
        }

        std::cout << "|" << std::endl;

        // Print moves, if applicable
        for (int column = 0; column < 8; ++column)
        {
            if (!moves.has_value())
            {
                std::cout << "|";
                printf("%24s", "");
                continue;
            }

            Coord coord{row, column};
            auto move = std::find_if(moves->begin(), moves->end(), [&coord](const Move &m)
                                     { return m.destination == coord; });

            std::cout << "|";
            if (move != moves->end())
            {
                printf("%24s", (*move).causes_state == MachineState::Sprinted ? "Sprint" : "Move");
            }
            else
            {
                printf("%24s", "");
            }
        }

        std::cout << "|" << std::endl;

        // Print attacks, if applicable
        std::vector<Coord> destinations;
        std::vector<Coord> affected_machines;

        if (attacks.has_value())
        {
            for (const auto &attack : *attacks)
            {
                destinations.push_back(attack.destination);
                affected_machines.insert(affected_machines.end(), attack.affected_machines.begin(), attack.affected_machines.end());
            }
        }

        for (int column = 0; column < 8; ++column)
        {
            if (!attacks.has_value())
            {
                std::cout << "|";
                printf("%24s", "");
                continue;
            }

            Coord coord{row, column};
            auto destination = std::find_if(destinations.begin(), destinations.end(), [&coord](const Coord &attack)
                                            { return attack == coord; });
            auto affected_machine = std::find_if(affected_machines.begin(), affected_machines.end(), [&coord](const Coord &affected_machine)
                                                 { return affected_machine == coord; });

            std::cout << "|";
            if (affected_machine != affected_machines.end())
            {
                printf("%24s", "Attack");
            }
            else if (destination != destinations.end() && focus_machine.value().machine.get().machine_type == MachineType::Dash)
            {
                printf("%24s", "Destination");
            }
            else
            {
                printf("%24s", "");
            }
        }

        std::cout << "|" << std::endl;
        std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
    }
}
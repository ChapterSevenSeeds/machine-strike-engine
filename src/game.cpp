#include <algorithm>
#include <iostream>
#include <cstdint>
#include <ranges>
#include "move.h"
#include "game_machine.h"
#include "game.h"
#include "attack.h"
#include "utils.h"
Game::Game(BoardType<std::optional<GameMachine>> machines, BoardType<Terrain> terrain, Player turn) : turn(turn)
{
    BoardType<GameMachine *> board_machines{nullptr};

    for (int row = 0; row < 8; ++row)
    {
        for (int column = 0; column < 8; ++column)
        {
            auto &machine = machines[{row, column}];
            if (!machine.has_value())
                continue;

            auto owned_machine = new GameMachine(machine.value());
            board_machines[{row, column}] = owned_machine;

            if (owned_machine->side == Player::Player)
                player_machines.push_back(owned_machine);
            else
                opponent_machines.push_back(owned_machine);
        }
    }

    board = new Board(terrain, board_machines);
}

Game::~Game()
{
    for (auto machine : player_machines)
        delete machine;
    for (auto machine : opponent_machines)
        delete machine;

    delete board;
}

Game::Game(const Game &game) : turn(game.turn), player_victory_points(game.player_victory_points), opponent_victory_points(game.opponent_victory_points), state(game.state), last_touched_machine(game.last_touched_machine), must_move_last_touched_machine(game.must_move_last_touched_machine)
{
    BoardType<GameMachine *> board_machines{nullptr};

    for (int row = 0; row < 8; ++row)
    {
        for (int column = 0; column < 8; ++column)
        {
            auto machine = game.board->machine_at({row, column});
            if (machine == nullptr)
                continue;

            auto owned_machine = new GameMachine(machine->machine, machine->direction, machine->coordinates, machine->machine_state, machine->side);
            board_machines[{row, column}] = owned_machine;

            if (owned_machine->side == Player::Player)
                player_machines.push_back(owned_machine);
            else
                opponent_machines.push_back(owned_machine);
        }
    }

    board = new Board(game.board->terrain, board_machines);
}

int Game::get_turn_machine_count() const
{
    return turn == Player::Player ? player_machines.size() : opponent_machines.size();
}

void Game::make_move(Move &m)
{
    auto machine = board->machine_at(m.source);
    machine->machine_state = m.causes_state;
    if (m.causes_state != MachineState::Overcharged)
    {
        must_move_last_touched_machine = false;
        if (state == GameState::TouchFirstMachine)
            state = GameState::TouchSecondMachine;
        else if (state == GameState::TouchSecondMachine)
            state = GameState::MustEndTurn;
    }

    if (machine->machine_state == MachineState::Overcharged)
        modify_machine_health(machine, -2);

    board->move_machine(m.source, m.destination);
}

void Game::make_attack(Attack &attack)
{
    auto attacker = board->machine_at(attack.source);
    last_touched_machine = attacker;
    must_move_last_touched_machine = attacker->machine_state == MachineState::Ready; // If we haven't touched this machine yet and we attack with it, we must then subsequently move it after attacking.
    attacker->direction = attack.attack_direction_from_source;

    switch (attacker->machine.get().machine_type)
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

    attacker->machine_state = attack.causes_state;
    if (attacker->machine_state == MachineState::Overcharged)
        modify_machine_health(attacker, -2);
}

void Game::pre_turn()
{
    state = GameState::TouchFirstMachine;
    for (auto &machine : *board)
    {
        machine->machine_state = MachineState::Ready;
        machine->attack_power_modifier = 0;
    }
    
    for (auto &machine : *board)
    {
        switch (machine->machine.get().skill)
        {
        case MachineSkill::Spray:
        {
            for (auto &other_machine : *board)
            {
                if (machine == other_machine)
                    continue;

                if (is_in_attack_range(machine, other_machine))
                    modify_machine_health(other_machine, -1);
            }
            break;
        }
        case MachineSkill::Whiplash:
        {
            for (auto &other_machine : *board)
            {
                if (machine == other_machine)
                    continue;

                if (is_in_attack_range(machine, other_machine))
                    other_machine->direction = opposite_direction(other_machine->direction);
            }
            break;
        }
        case MachineSkill::Empower:
        {
            for (auto &other_machine : *board)
            {
                if (machine == other_machine || other_machine->side != machine->side)
                    continue;

                if (is_in_attack_range(machine, other_machine))
                    ++other_machine->attack_power_modifier;
            }
            break;
        }
        case MachineSkill::Blind:
        {
            for (auto &other_machine : *board)
            {
                if (machine == other_machine || other_machine->side == machine->side)
                    continue;

                if (is_in_attack_range(machine, other_machine))
                    --other_machine->attack_power_modifier;
            }
            break;
        }
        }
    }
}

void Game::modify_machine_health(GameMachine *machine, int32_t health_change)
{
    machine->health += health_change;
    if (!machine->is_alive())
    {
        board->clear_spot(machine->coordinates);

        if (machine->side == Player::Player)
        {
            opponent_victory_points += machine->machine.get().points;
        }
        else
        {
            player_victory_points += machine->machine.get().points;
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

void Game::end_turn()
{
    turn = turn == Player::Player ? Player::Opponent : Player::Player;
    pre_turn();
}

bool Game::can_end_turn() const
{
    return state == GameState::MustEndTurn;
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

void Game::print_board(GameMachine *focus_machine, std::optional<std::vector<Move>> moves, std::optional<std::vector<Attack>> attacks)
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
            auto terrain = board->terrain_at(coord);

            std::cout << "|";
            printf("%24s", to_string(terrain).c_str());
        }

        std::cout << "|" << std::endl;

        // Print machines
        for (int column = 0; column < 8; ++column)
        {
            Coord coord{row, column};
            auto machine = board->machine_at(coord);

            std::cout << "|";
            if (machine != nullptr)
            {
                printf(machine->side == turn ? GRN : RED);
                printf("%24s", ((focus_machine != nullptr && machine == focus_machine ? std::string("->") : "") + machine->machine.get().name + get_direction_string(machine->direction)).c_str());
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
            auto machine = board->machine_at(coord);

            std::cout << "|";
            if (machine != nullptr)
            {
                printf("%24s", std::string("Health: " + std::to_string(machine->health)).c_str());
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
            auto machine = board->machine_at(coord);

            std::cout << "|";
            if (machine != nullptr)
            {
                printf("%24s", std::string("Combat: " + std::to_string(calculate_combat_power(machine, std::nullopt))).c_str());
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
            auto move_with_overcharge = std::find_if(moves->begin(), moves->end(), [&coord](const Move &m)
                                                     { return m.destination == coord && m.causes_state == MachineState::Overcharged; });

            std::cout << "|";
            if (move != moves->end())
            {
                printf("%24s", (*move).causes_state == MachineState::Sprinted || (move_with_overcharge != moves->end() && (*move_with_overcharge).causes_state == MachineState::Sprinted) ? "Sprint" : "Move");
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
            else if (destination != destinations.end() && focus_machine->machine.get().machine_type == MachineType::Dash)
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
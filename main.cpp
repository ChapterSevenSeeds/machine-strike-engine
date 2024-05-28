#include <locale>
#include <functional>
#include <optional>
#include <iostream>
#include <ranges>
#include <algorithm>
#include <vector>
#include <string>
#include "enums.h"
#include "types.h"
#include "coord.h"
#include "game.h"
#include "game_machine.h"
#include "machine_definitions.h"

BoardType<Terrain> parse_terrain(std::string &str)
{
    BoardType<Terrain> terrain;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            switch (str[i * 8 + j])
            {
            case 'C':
                terrain[{i, j}] = Terrain::Chasm;
                break;
            case 'M':
                terrain[{i, j}] = Terrain::Marsh;
                break;
            case 'G':
                terrain[{i, j}] = Terrain::Grassland;
                break;
            case 'F':
                terrain[{i, j}] = Terrain::Forest;
                break;
            case 'H':
                terrain[{i, j}] = Terrain::Hill;
                break;
            case 'm':
                terrain[{i, j}] = Terrain::Mountain;
                break;
            default:
                throw std::runtime_error("Invalid terrain character");
            }
        }
    }

    return terrain;
}

BoardType<std::optional<std::reference_wrapper<GameMachine>>> parse_machines(std::string str, Player player)
{
    BoardType<std::optional<std::reference_wrapper<GameMachine>>> machines;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            auto index_of_comma = str.find_first_of(",");
            if (index_of_comma == std::string::npos)
            {
                throw std::runtime_error("Invalid machine string");
            }

            auto machine_name = str.substr(0, index_of_comma);

            if (machine_name.empty())
            {
                machines[{i, j}] = std::nullopt;
                continue;
            }

            str = str.substr(index_of_comma + 1);

            auto machine = std::find_if(ALL_MACHINES.begin(), ALL_MACHINES.end(), [&machine_name](const Machine &m)
                                        { return std::tolower(std::string(m.name), std::locale{}) == std::tolower(machine_name, std::locale{}); });

            if (machine == ALL_MACHINES.end())
                throw std::runtime_error("Invalid machine name" + machine_name);

            static GameMachine game_machine(std::ref(*machine), player == Player::Player ? MachineDirection::North : MachineDirection::South, {i, j}, MachineState::Ready, player);
            machines[{i, j}] = std::ref(game_machine);
        }
    }

    return machines;
}

BoardType<std::optional<std::reference_wrapper<GameMachine>>> combine(BoardType<std::optional<std::reference_wrapper<GameMachine>>> &a, BoardType<std::optional<std::reference_wrapper<GameMachine>>> &b)
{
    BoardType<std::optional<std::reference_wrapper<GameMachine>>> combined;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (a[{i, j}].has_value())
            {
                combined[{i, j}] = a[{i, j}];
            }
            else if (b[{i, j}].has_value())
            {
                combined[{i, j}] = b[{i, j}];
            }
            else
            {
                combined[{i, j}] = std::nullopt;
            }
        }
    }

    return combined;
}

int main()
{
    Game *game = nullptr;

    while (true)
    {
        std::string input;
        std::cin >> input;

        if (input == "newgame")
        {
            std::string terrain_str, player_machines_str, opponent_machines_str, first;
            std::cin >> terrain_str >> player_machines_str >> opponent_machines_str >> first;

            auto terrain = parse_terrain(terrain_str);
            auto player_machines = parse_machines(player_machines_str, Player::Player);
            auto opponent_machines = parse_machines(opponent_machines_str, Player::Opponent);
            auto machines = combine(player_machines, opponent_machines);

            Player first_player;
            if (first == "player")
                first_player = Player::Player;
            else if (first == "opponent")
                first_player = Player::Opponent;
            else
                throw std::runtime_error("Invalid first player");

            game = new Game({terrain, machines}, first_player);
        }
        if (input == "moves")
        {
            int row, column;
            std::cin >> row >> column;

            auto machine = game->board.machine_at({row, column});
            if (!machine.has_value())
            {
                std::cout << "No machine at that location" << std::endl;
                continue;
            }
            auto moves = game->calculate_moves(machine.value().get());
            game->print_board(machine, moves);
        }
        else if (input == "print")
        {
            game->print_board();
        }
        else if (input == "attacks")
        {
            int row, column;
            std::cin >> row >> column;

            auto machine = game->board.machine_at({row, column});
            if (!machine.has_value())
            {
                std::cout << "No machine at that location" << std::endl;
                continue;
            }

            auto attacks = game->calculate_attacks(machine.value().get());
            game->print_board(machine, std::nullopt, attacks);
        }
        else if (input == "attack")
        {
            int machine_row, machine_column;
            std::string attack_direction;
            std::cin >> machine_row >> machine_column;
            std::cin >> attack_direction;

            auto machine = game->board.machine_at({machine_row, machine_column});
            if (!machine.has_value())
            {
                std::cout << "No machine at that location" << std::endl;
                continue;
            }

            MachineDirection direction;
            if (attack_direction == "north")
            {
                direction = MachineDirection::North;
            }
            else if (attack_direction == "south")
            {
                direction = MachineDirection::South;
            }
            else if (attack_direction == "east")
            {
                direction = MachineDirection::East;
            }
            else if (attack_direction == "west")
            {
                direction = MachineDirection::West;
            }
            else
            {
                std::cout << "Invalid direction" << std::endl;
                continue;
            }

            auto attacks = game->calculate_attacks(machine.value().get());

            auto attack = std::find_if(attacks.begin(), attacks.end(), [&attack_direction, &direction](const Attack &a)
                                       { return a.attack_direction_from_source == direction; });

            if (attack == attacks.end())
            {
                std::cout << "No attack in that direction" << std::endl;
                continue;
            }

            game->make_attack(*attack);
            game->print_board();
        }
        else if (input == "move")
        {
            int machine_row, machine_column, destination_row, destination_column;
            std::cin >> machine_row >> machine_column >> destination_row >> destination_column;

            auto machine = game->board.machine_at({machine_row, machine_column});
            if (!machine.has_value())
            {
                std::cout << "No machine at that location" << std::endl;
                continue;
            }

            auto moves = game->calculate_moves(machine.value().get());
            auto move = std::find_if(moves.begin(), moves.end(), [&destination_row, &destination_column](const Move &m)
                                     { return m.destination == Coord{destination_row, destination_column}; });

            if (move == moves.end())
            {
                std::cout << "Invalid move" << std::endl;
                continue;
            }

            game->make_move(*move);
            game->print_board();
        }
    }
}
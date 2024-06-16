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
#include "string_utils.h"
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

BoardType<std::optional<GameMachine>> parse_machines(std::string str)
{
    BoardType<std::optional<GameMachine>> machines;

    auto machine_entries = split(str, ';');

    if (machine_entries.size() != 64)
    {
        throw std::runtime_error("Invalid machine string");
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            auto machine_entry = machine_entries[i * 8 + j];

            if (machine_entry.empty())
                continue;

            auto machine_data = split(machine_entry, ',');

            if (machine_data.size() != 3)
                throw std::runtime_error("Invalid machine data");

            auto machine_name = machine_data[0];
            auto machine = std::find_if(ALL_MACHINES.begin(), ALL_MACHINES.end(), [&machine_name](const std::reference_wrapper<const Machine> &m)
                                        { return std::string(m.get().name) == machine_name; });

            if (machine == ALL_MACHINES.end())
                throw std::runtime_error("Invalid machine name" + machine_name);

            auto machine_direction = machine_data[1];
            MachineDirection direction;
            if (machine_direction == "N")
                direction = MachineDirection::North;
            else if (machine_direction == "S")
                direction = MachineDirection::South;
            else if (machine_direction == "E")
                direction = MachineDirection::East;
            else if (machine_direction == "W")
                direction = MachineDirection::West;
            else
                throw std::runtime_error("Invalid machine direction");

            auto machine_side = machine_data[2];
            Player side;
            if (machine_side == "P")
                side = Player::Player;
            else if (machine_side == "O")
                side = Player::Opponent;
            else
                throw std::runtime_error("Invalid machine side");

            machines[{i, j}] = GameMachine(std::ref(*machine), direction, {i, j}, MachineState::Ready, side);
        }
    }

    return machines;
}

int main()
{
    Game *game = nullptr;

    while (true)
    {
        if (game != nullptr && game->check_winner() != Winner::None)
        {
            std::cout << "Game over" << std::endl;
            std::cout << "Winner: " << (game->check_winner() == Winner::Player ? "Player" : "Opponent") << std::endl;
            delete game;
            game = nullptr;
            break;
        }

        std::string input;
        std::getline(std::cin, input);

        auto tokens = split(input, ' ');

        if (tokens[0] == "newgame")
        {
            if (tokens.size() != 4)
            {
                std::cout << "Invalid newgame command" << std::endl;
                continue;
            }

            auto terrain_str = tokens[1];
            auto machines_str = tokens[2];
            auto first = tokens[3];

            auto terrain = parse_terrain(terrain_str);
            auto machines = parse_machines(machines_str);

            Player first_player;
            if (first == "player")
                first_player = Player::Player;
            else if (first == "opponent")
                first_player = Player::Opponent;
            else
                throw std::runtime_error("Invalid first player");

            game = new Game(machines, terrain, first_player);
        }
        else if (tokens[0] == "rotate")
        {
            int row = std::stoi(tokens[1]);
            int column = std::stoi(tokens[2]);
            std::string rotation = tokens[3];

            auto machine = game->board->machine_at({row, column});
            if (machine == nullptr)
            {
                std::cout << "No machine at that location" << std::endl;
                continue;
            }

            MachineDirection direction;
            if (rotation == "N")
                direction = MachineDirection::North;
            else if (rotation == "S")
                direction = MachineDirection::South;
            else if (rotation == "E")
                direction = MachineDirection::East;
            else if (rotation == "W")
                direction = MachineDirection::West;
            else
            {
                std::cout << "Invalid rotation" << std::endl;
                continue;
            }

            machine->direction = direction;
        }
        else if (tokens[0] == "endturn")
        {
            if (!game->can_end_turn())
            {
                std::cout << "Cannot end turn" << std::endl;
                continue;
            }

            game->end_turn();
        }
        else if (tokens[0] == "moves")
        {
            if (tokens.size() != 3)
            {
                std::cout << "Invalid moves command" << std::endl;
                continue;
            }

            int row = std::stoi(tokens[1]);
            int column = std::stoi(tokens[2]);

            auto machine = game->board->machine_at({row, column});
            if (machine == nullptr)
            {
                std::cout << "No machine at that location" << std::endl;
                continue;
            }
            auto moves = game->calculate_moves(machine);
            game->print_board(machine, moves);
        }
        else if (tokens[0] == "print")
        {
            game->print_board();
        }
        else if (tokens[0] == "attacks")
        {
            if (tokens.size() != 3)
            {
                std::cout << "Invalid attacks command" << std::endl;
                continue;
            }

            int row = std::stoi(tokens[1]);
            int column = std::stoi(tokens[2]);

            auto machine = game->board->machine_at({row, column});
            if (machine == nullptr)
            {
                std::cout << "No machine at that location" << std::endl;
                continue;
            }

            auto attacks = game->calculate_attacks(machine);
            game->print_board(machine, std::nullopt, attacks);
        }
        else if (tokens[0] == "attack")
        {
            if (tokens.size() != 5)
            {
                std::cout << "Invalid move command" << std::endl;
                continue;
            }

            int machine_row = std::stoi(tokens[1]);
            int machine_column = std::stoi(tokens[2]);
            std::string attack_direction = tokens[3];
            bool overcharge = tokens[4] == "true";

            auto machine = game->board->machine_at({machine_row, machine_column});
            if (machine == nullptr)
            {
                std::cout << "No machine at that location" << std::endl;
                continue;
            }

            MachineDirection direction;
            if (attack_direction == "north")
                direction = MachineDirection::North;
            else if (attack_direction == "south")
                direction = MachineDirection::South;
            else if (attack_direction == "east")
                direction = MachineDirection::East;
            else if (attack_direction == "west")
                direction = MachineDirection::West;
            else
            {
                std::cout << "Invalid direction" << std::endl;
                continue;
            }

            auto attacks = game->calculate_attacks(machine);

            auto attack = std::find_if(attacks.begin(), attacks.end(), [&attack_direction, &direction, &overcharge](const Attack &a)
                                       { return a.attack_direction_from_source == direction && ((overcharge && a.causes_state == MachineState::Overcharged) || (!overcharge && a.causes_state != MachineState::Overcharged)); });

            if (attack == attacks.end())
            {
                std::cout << "No attack in that direction" << std::endl;
                continue;
            }

            game->make_attack(*attack);
        }
        else if (tokens[0] == "move")
        {
            if (tokens.size() != 6)
            {
                std::cout << "Invalid move command" << std::endl;
                continue;
            }

            int machine_row = std::stoi(tokens[1]);
            int machine_column = std::stoi(tokens[2]);
            int destination_row = std::stoi(tokens[3]);
            int destination_column = std::stoi(tokens[4]);
            bool overcharge = tokens[5] == "true";

            auto machine = game->board->machine_at({machine_row, machine_column});
            if (machine == nullptr)
            {
                std::cout << "No machine at that location" << std::endl;
                continue;
            }

            auto moves = game->calculate_moves(machine);
            auto move = std::find_if(moves.begin(), moves.end(), [&destination_row, &destination_column, &overcharge](const Move &m)
                                     { return m.destination == Coord{destination_row, destination_column} && ((overcharge && m.causes_state == MachineState::Overcharged) || (!overcharge && m.causes_state != MachineState::Overcharged)); });

            if (move == moves.end())
            {
                std::cout << "Invalid move" << std::endl;
                continue;
            }

            game->make_move(*move);
        }
        else if (tokens[0] == "search")
        {
            game->search(5);
        }
    }
}
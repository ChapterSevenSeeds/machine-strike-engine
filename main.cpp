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

int main() {
    BoardType<Terrain> terrain  {{
        { Terrain::Grassland, Terrain::Marsh, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Mountain, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
    }};

    GameMachine game_machine(std::ref(RAVAGER), MachineDirection::East, {0, 0}, MachineState::Ready, Player::Player);
    GameMachine game_machine2(std::ref(SCORCHER), MachineDirection::South, {2, 1}, MachineState::Ready, Player::Player);
    GameMachine opponent_machine(std::ref(BURROWER), MachineDirection::North, {2, 0}, MachineState::Ready, Player::Opponent);

    BoardType<std::optional<std::reference_wrapper<GameMachine>>> machines {{
        {game_machine, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {opponent_machine, game_machine2, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
    }};

    Game game(
        {terrain, machines},
        Player::Player
    );

    while (true) 
    {
        std::string input;
        std::cin >> input;

        if (input == "moves") 
        {
            int row, column;
            std::cin >> row >> column;

            auto machine = game.board.machine_at({row, column});
            if (!machine.has_value()) {
                std::cout << "No machine at that location" << std::endl;
                continue;
            }
            auto moves = game.calculate_moves(machine.value().get());
            game.print_board(machine, moves);
        }
        else if (input == "print")
        {
            game.print_board();
        }
        else if (input == "attacks") 
        {
            int row, column;
            std::cin >> row >> column;

            auto machine = game.board.machine_at({row, column});
            if (!machine.has_value()) {
                std::cout << "No machine at that location" << std::endl;
                continue;
            }

            auto attacks = game.calculate_attacks(machine.value().get());
            game.print_board(machine, std::nullopt, attacks);
        }
        else if (input == "attack")
        {
            int machine_row, machine_column;
            std::string attack_direction;
            std::cin >> machine_row >> machine_column;
            std::cin >> attack_direction;

            auto machine = game.board.machine_at({machine_row, machine_column});
            if (!machine.has_value()) {
                std::cout << "No machine at that location" << std::endl;
                continue;
            }

            MachineDirection direction;
            if (attack_direction == "north") {
                direction = MachineDirection::North;
            } else if (attack_direction == "south") {
                direction = MachineDirection::South;
            } else if (attack_direction == "east") {
                direction = MachineDirection::East;
            } else if (attack_direction == "west") {
                direction = MachineDirection::West;
            } else {
                std::cout << "Invalid direction" << std::endl;
                continue;
            }

            auto attacks = game.calculate_attacks(machine.value().get());

            auto attack = std::find_if(attacks.begin(), attacks.end(), [&attack_direction, &direction](const Attack &a) {
                return a.attack_direction_from_source == direction;
            });

            if (attack == attacks.end()) {
                std::cout << "No attack in that direction" << std::endl;
                continue;
            }

            game.make_attack(*attack);
            game.print_board();
        }
        else if (input == "move")
        {
            int machine_row, machine_column, destination_row, destination_column;
            std::cin >> machine_row >> machine_column >> destination_row >> destination_column;

            auto machine = game.board.machine_at({machine_row, machine_column});
            if (!machine.has_value()) {
                std::cout << "No machine at that location" << std::endl;
                continue;
            }

            auto moves = game.calculate_moves(machine.value().get());
            auto move = std::find_if(moves.begin(), moves.end(), [&destination_row, &destination_column](const Move &m) {
                return m.destination == Coord{destination_row, destination_column};
            });

            if (move == moves.end()) {
                std::cout << "Invalid move" << std::endl;
                continue;
            }

            game.make_move(*move);
            game.print_board();
        }
    }
}
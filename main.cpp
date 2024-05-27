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

void print_board(Game& game) {
    std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

    for (int row : std::views::iota(0, 8)) {
        for (int column : std::views::iota(0, 8)) {
            Coord coord {row, column};
            auto machine = game.board.machine_at(coord);

            std::cout << "|";
            if (machine.has_value()) {
                printf("%24s", machine->get().machine.get().name);
            } else {
                printf("%24s", " ");
            }
        }

        std::cout << "|" << std::endl;

        for (int column : std::views::iota(0, 8)) {
            Coord coord {row, column};
            auto terrain = game.board.terrain_at(coord);

            std::cout << "|";
            printf("%24s", to_string(terrain).c_str());
        }

        std::cout << "|" << std::endl;
        std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
    }
}

void print_moves(std::vector<Move> moves) {
    for (int row : std::views::iota(0, 8)) {
        for (int column : std::views::iota(0, 8)) {
            Coord coord {row, column};
            auto m = std::find_if(moves.begin(), moves.end(), [&coord](const Move& m) {
                return m.destination == coord;
            });
            if (m != moves.end()) {
                std::cout << "|" << ((*m).requires_sprint ? "S" : "X");
            } else {
                std::cout << "| ";
            }
        }

        std::cout << "|" << std::endl;
    }
}

void print_attacks(std::vector<Attack> attacks) {
    std::vector<Coord> destinations;
    std::vector<Coord> affected_machines;

    for (const auto& attack : attacks) {
        destinations.push_back(attack.destination);
        affected_machines.insert(affected_machines.end(), attack.affected_machines.begin(), attack.affected_machines.end());
    }

    for (int row : std::views::iota(0, 8)) {
        for (int column : std::views::iota(0, 8)) {
            Coord coord {row, column};
            auto destination = std::find_if(destinations.begin(), destinations.end(), [&coord](const Coord& attack) {
                return attack == coord;
            });
            auto affected_machine = std::find_if(affected_machines.begin(), affected_machines.end(), [&coord](const Coord& affected_machine) {
                return affected_machine == coord;
            });
            if (affected_machine != affected_machines.end()) {
                std::cout << "|A";
            } else if (destination != destinations.end()) {
                std::cout << "|M";
            } else {
                std::cout << "| ";
            }
        }

        std::cout << "|" << std::endl;
    }
}


int main() {
    BoardType<Terrain> terrain  {{
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
    }};

    GameMachine game_machine(std::ref(RAVAGER), 0, MachineDirection::South, {0, 0}, MachineState::Ready, 0, Player::Player);
    GameMachine game_machine2(std::ref(SCORCHER), 0, MachineDirection::South, {2, 1}, MachineState::Ready, 0, Player::Player);
    GameMachine opponent_machine(std::ref(STALKER), 0, MachineDirection::North, {2, 0}, MachineState::Ready, 0, Player::Opponent);

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
        {std::ref(STALKER)},
        {std::ref(RAVAGER), std::ref(SCORCHER)},
        {terrain, machines},
        Player::Player
    );

    auto attacks = game.calculate_attacks(game_machine);
    print_attacks(attacks);
}



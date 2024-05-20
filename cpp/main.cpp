#include <functional>
#include <optional>
#include <iostream>
#include <ranges>
#include <algorithm>
#include <vector>
#include <string>
#include "moves.h"
#include "enums.h"
#include "types.h"
#include "game.h"
#include "game_machine.h"
#include "machine_definitions.h"

void print_board(Game& game) {
    std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

    for (int row : std::views::iota(0, 8)) {
        for (int column : std::views::iota(0, 8)) {
            auto machine = game.machines[row][column];

            std::cout << "|";
            if (machine.has_value()) {
                printf("%24s", machine->get().machine.get().name);
            } else {
                printf("%24s", " ");
            }
        }

        std::cout << "|" << std::endl;

        for (int column : std::views::iota(0, 8)) {
            auto terrain = game.terrain[row][column];

            std::cout << "|";
            printf("%24s", to_string(terrain).c_str());
        }

        std::cout << "|" << std::endl;
        std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
    }
}

void print_moves(std::vector<Move*> moves) {
    for (int row : std::views::iota(0, 8)) {
        for (int column : std::views::iota(0, 8)) {
            auto m = std::find_if(moves.begin(), moves.end(), [row, column](const Move* m) {
                return m->destination_row == row && m->destination_column == column;
            });
            if (m != moves.end()) {
                std::cout << "|" << ((*m)->requires_sprint ? "S" : "X");
            } else {
                std::cout << "| ";
            }
        }

        std::cout << "|" << std::endl;
    }
}


int main() {
    Board<Terrain> terrain  {{
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
    }};

    GameMachine game_machine(std::ref(BELLOWBACK), 0, MachineDirection::South, 0, 0, MachineState::Ready, 0, 0, Player::Player);
    GameMachine opponent_machine(std::ref(STALKER), 0, MachineDirection::North, 2, 0, MachineState::Ready, 0, 0, Player::Opponent);

    Board<std::optional<std::reference_wrapper<GameMachine>>> machines {{
        {game_machine, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {opponent_machine, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
    }};

    Game game(
        {std::ref(STALKER)},
        {std::ref(BELLOWBACK)},
        terrain,
        machines,
        Player::Player
    );

    auto moves = calculate_moves(game, game_machine);
    print_moves(moves);
}

/* void print_attacks(std::vector<attacks: Vec<Attack>) {
    for row in 0..8 {
        for col in 0..8 {
            let m = attacks
                .iter()
                .find(|m| m.machine_row == row && m.machine_column == col);
            match m {
                Some(m) => {
                    print!("|{:>1}", { "A" });
                }
                None => {
                    print!("|{:>1}", " ");
                }
            }
        }

        println!("|");
    }
}
 */

#include <functional>
#include <optional>
#include <iostream>
#include <ranges>
#include "enums.h"
#include "types.h"
#include "game.h"
#include "game_machine.h"
#include "machine_definitions.h"

int main() {
    Board<Terrain> terrain = {
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
        { Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, },
    };

    GameMachine game_machine(std::ref(BELLOWBACK), 0, MachineDirection::South, 2, 0, MachineState::Ready, 0, 0, Player::Player);
    GameMachine opponent_machine(std::ref(STALKER), 0, MachineDirection::North, 2, 0, MachineState::Ready, 0, 0, Player::Opponent);

    Board<std::optional<std::reference_wrapper<GameMachine>>> machines {
        {game_machine, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {opponent_machine, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt },
    };

    Game game(
        {std::ref(STALKER)},
        {std::ref(BELLOWBACK)},
        terrain,
        machines,
        Player::Player
    );
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
/* fn print_moves(moves: Vec<Move>) {
    for row in 0..8 {
        for col in 0..8 {
            let m = moves.iter().find(|m| m.destination_row == row && m.destination_column == col);
            match m {
                Some(m) => {
                    print!("|{:>1}", if !m.requires_sprint { "X" } else { "S" });
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
void print_board(Game& game) {
    std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
    #include <ranges> // Include the necessary header file

    for (int row : std::views::iota(0, 8)) {
        for (int column : std::views::iota(0, 8)) {
            auto machine = game.machines[row][column];

            std::cout << "|";
            switch (machine.has_value()) {
                case true: {
                    std::cout << machine->get().machine.get().name;
                    break;
                }
                case false: {
                    std::cout << "                        ";
                    break;
                }
            }
        }

        std::cout << "|";

        for (int column : std::views::iota(0, 8)) {
            auto terrain = game.terrain[row][column];

            std::cout << "|";
            std::cout << "{:<24}" << to_string(terrain);
        }

        std::cout << "|";
        std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
    }
}

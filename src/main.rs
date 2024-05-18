use attacks::{calculate_attacks, Attack};
use game_machine::GameMachine;
use machine::Machine;
use machines::{BELLOWBACK, SPIKESNOUT, STALKER, STORMBIRD};
use moves::{calculate_moves, Move};

use crate::enums::{MachineDirection, MachineState, Player, Terrain};

pub mod attacks;
pub mod enums;
pub mod game;
pub mod game_machine;
pub mod machine;
pub mod machines;
pub mod moves;

fn main() {
    let mut board: [[Terrain; 8]; 8] = [
        [ Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, ],
        [ Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, ],
        [ Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, ],
        [ Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, ],
        [ Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, ],
        [ Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, ],
        [ Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, ],
        [ Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, Terrain::Grassland, ],
    ];

    let mut opponent_machine: GameMachine = GameMachine {
        row: 2,
        column: 0,
        machine: STALKER,
        attack_power: 0,
        defense_power: 0,
        direction: MachineDirection::East,
        health: 0,
        machine_state: MachineState::Ready,
        side: Player::Opponent,
    };

    let mut game_machine: GameMachine = GameMachine {
        row: 0,
        column: 0,
        machine: BELLOWBACK,
        attack_power: 0,
        defense_power: 0,
        direction: MachineDirection::East,
        health: 0,
        machine_state: MachineState::Ready,
        side: Player::Player,
    };

    let mut machines: [[Option<GameMachine>; 8]; 8] = [
        [Some(game_machine), None, None, None, None, None, None, None, ],
        [None, None, None, None, None, None, None, None],
        [Some(opponent_machine), None, None, None, None, None, None, None, ],
        [None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None],
    ];

    let game = game::Game {
        board: board,
        machines: machines,
        opponent_starting_pieces: Vec::new(),
        player_starting_pieces: Vec::new(),
        turn: Player::Opponent,
    };

    let attacks = calculate_attacks(&game, game.machines[0][0].as_ref().unwrap());
    print_attacks(attacks);
}

fn print_attacks(attacks: Vec<Attack>) {
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

fn print_moves(moves: Vec<Move>) {
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

fn print_board(board: [[Terrain; 8]; 8], machines: [[Option<Machine>; 8]; 8]) {
    println!("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    for row in 0..8 {
        for col in 0..8 {
            let machine = &machines[row][col];

            print!("|");
            match machine {
                Some(m) => {
                    print!("{name:<24}", name = m.name);
                }
                None => {
                    print!("                        ");
                }
            }
        }

        println!("|");

        for col in 0..8 {
            let terrain = &board[row][col];

            print!("|");
            print!("{:<24}", terrain.to_string());
        }

        println!("|");
        println!("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    }
}

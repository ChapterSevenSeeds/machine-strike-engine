use crate::{game::Game, game_machine::GameMachine};

pub fn calculate_moves(game: Game, machine: GameMachine) {
    let flood_fill_moves: u64 = 0b
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 1 0 0 0 0
    0 0 1 0 1 0 0 0
    0 0 0 1 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0


    0 0 0 0 0 0 0 0
    0 0 0 1 0 0 0 0
    0 0 1 1 1 0 0 0
    0 1 1 0 1 1 0 0
    0 0 1 1 1 0 0 0
    0 0 0 1 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0

    0 0 0 1 0 0 0 0
    0 0 1 1 1 0 0 0
    0 1 1 1 1 1 0 0
    1 1 1 0 1 1 1 0
    0 1 1 1 1 1 0 0
    0 0 1 1 1 0 0 0
    0 0 0 1 0 0 0 0
    0 0 0 0 0 0 0 0

}
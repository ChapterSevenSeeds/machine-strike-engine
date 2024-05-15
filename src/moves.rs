use crate::{
    enums::{MachineType, Terrain},
    game::Game,
    game_machine::GameMachine,
};

pub struct Move {
    pub row: usize,
    pub column: usize,
    pub requires_overcharge: bool,
}

pub fn calculate_moves(game: Game, machine: GameMachine) -> Vec<Move> {
    let mut flood_filled = [[false; 8]; 8];

    let mut all_moves: Vec<Move> = Vec::new();

    let mut remaining_flood_depth = machine.machine.movement - 1;
    all_moves.append(&mut expand_moves(
        remaining_flood_depth,
        machine.row,
        machine.column,
        &machine,
        &game,
        &mut flood_filled,
    ));

    let mut expanded_index = 0;

    loop {
        let current_move = all_moves.get(expanded_index).unwrap();

        expanded_index += 1;

        all_moves.append(&mut expand_moves(
            remaining_flood_depth,
            current_move.row,
            current_move.column,
            &machine,
            &game,
            &mut flood_filled,
        ));

        remaining_flood_depth -= 1;

        if expanded_index >= all_moves.len() {
            break;
        }
    }

    return all_moves;
}

fn expand_moves(
    remaining_movements: i32,
    row: usize,
    column: usize,
    machine: &GameMachine,
    game: &Game,
    flood_filled: &mut [[bool; 8]; 8],
) -> Vec<Move> {
    // If the machine is not a flying machine and we are currently in a marsh, we can't move
    if !machine.machine.is_flying() && game.board[row][column] == Terrain::Marsh {
        return Vec::new();
    }

    // If we have already overcharged, we can't move
    if remaining_movements < 0 {
        return Vec::new();
    }

    let mut moves: Vec<Move> = Vec::new();
    let requires_overcharge = remaining_movements == 0;
    let mut new_row: usize;
    let mut new_column: usize;

    // Go up
    if row > 0 {
        new_row = row - 1;
        if !is_spot_blocked_or_redundant(new_row, column, &game, &machine, flood_filled) {
            moves.push(Move {
                row: new_row,
                column,
                requires_overcharge,
            });
            flood_filled[new_row][column] = true;
        }
    }

    // Go down
    if row < 7 {
        new_row = row + 1;
        if !is_spot_blocked_or_redundant(new_row, column, &game, &machine, flood_filled) {
            moves.push(Move {
                row: new_row,
                column,
                requires_overcharge,
            });
            flood_filled[new_row][column] = true;
        }
    }

    // Go left
    if column > 0 {
        new_column = column - 1;
        if !is_spot_blocked_or_redundant(row, new_column, &game, &machine, flood_filled) {
            moves.push(Move {
                row,
                column: new_column,
                requires_overcharge,
            });
            flood_filled[row][new_column] = true;
        }
    }

    // Go right
    if column < 7 {
        new_column = column + 1;
        if !is_spot_blocked_or_redundant(row, new_column, &game, &machine, flood_filled) {
            moves.push(Move {
                row,
                column: new_column,
                requires_overcharge,
            });
            flood_filled[row][new_column] = true;
        }
    }

    return moves;
}

fn is_spot_blocked_or_redundant(
    x: usize,
    y: usize,
    game: &Game,
    machine: &GameMachine,
    flood_filled: &[[bool; 8]; 8],
) -> bool {
    // Are we out of bounds?
    if x > 7 || y > 7 {
        return true;
    }

    // Have we already been here?
    if flood_filled[x][y] {
        return true;
    }

    // Is a machine already there?
    if game.machines[x][y].is_some() {
        return true;
    }

    // Is the machine not a flying machine and is the terrain a chasm?
    if !machine.machine.is_flying() && game.board[x][y] == Terrain::Chasm {
        return true;
    }

    return false;
}

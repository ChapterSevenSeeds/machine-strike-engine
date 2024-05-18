use crate::{
    enums::{MachineState, MachineType, Terrain},
    game::Game,
    game_machine::GameMachine,
};

pub struct Move {
    pub row: usize,
    pub column: usize,
    pub requires_sprint: bool,
    pub found_at_depth: i32
}

pub struct MoveWithDepth {
    pub m: Move,
    pub depth: i32,
}

pub fn calculate_moves(game: &Game, machine: &GameMachine) -> Vec<Move> {
    let mut visited = [[false; 8]; 8];

    let mut all_moves: Vec<Move> = Vec::new();

    all_moves.append(&mut expand_moves(
        1,
        machine.row,
        machine.column,
        &machine,
        &game,
        &mut visited,
    ));

    if all_moves.is_empty() {
        return all_moves;
    }

    let mut expanded_index = 0;

    loop {
        let current_move = all_moves.get(expanded_index).unwrap();

        expanded_index += 1;

        all_moves.append(&mut expand_moves(
            current_move.found_at_depth + 1,
            current_move.row,
            current_move.column,
            &machine,
            &game,
            &mut visited,
        ));

        if expanded_index >= all_moves.len() {
            break;
        }
    }

    return all_moves;
}

fn expand_moves(
    distance_travelled: i32,
    row: usize,
    column: usize,
    machine: &GameMachine,
    game: &Game,
    visited: &mut [[bool; 8]; 8],
) -> Vec<Move> {
    // If we have already sprinted, we can't move
    if distance_travelled > machine.machine.movement + 1 {
        return Vec::new();
    }

    // If this isn't our first movement (not move) and if the machine is not a flying machine and if the machine is not a swoop type and we are currently in a marsh, we can't move
    if distance_travelled > 1 && !machine.machine.is_flying() && !machine.machine.is_pull() && game.board[row][column] == Terrain::Marsh {
        return Vec::new();
    }

    let mut moves: Vec<Move> = Vec::new();
    let requires_sprint = distance_travelled > machine.machine.movement;
    let mut new_row: usize;
    let mut new_column: usize;

    // Go up
    if row > 0 {
        new_row = row - 1;
        if !is_spot_blocked_or_redundant(new_row, column, &game, &machine, visited) {
            moves.push(Move {
                row: new_row,
                column,
                requires_sprint,
                found_at_depth: distance_travelled
            });
            visited[new_row][column] = true;
        }
    }

    // Go down
    if row < 7 {
        new_row = row + 1;
        if !is_spot_blocked_or_redundant(new_row, column, &game, &machine, visited) {
            moves.push(Move {
                row: new_row,
                column,
                requires_sprint,
                found_at_depth: distance_travelled
            });
            visited[new_row][column] = true;
        }
    }

    // Go left
    if column > 0 {
        new_column = column - 1;
        if !is_spot_blocked_or_redundant(row, new_column, &game, &machine, visited) {
            moves.push(Move {
                row,
                column: new_column,
                requires_sprint,
                found_at_depth: distance_travelled
            });
            visited[row][new_column] = true;
        }
    }

    // Go right
    if column < 7 {
        new_column = column + 1;
        if !is_spot_blocked_or_redundant(row, new_column, &game, &machine, visited) {
            moves.push(Move {
                row,
                column: new_column,
                requires_sprint,
                found_at_depth: distance_travelled
            });
            visited[row][new_column] = true;
        }
    }

    return moves;
}

fn is_spot_blocked_or_redundant(
    row: usize,
    column: usize,
    game: &Game,
    machine: &GameMachine,
    visited: &[[bool; 8]; 8],
) -> bool {
    // Are we out of bounds?
    if row > 7 || column > 7 {
        return true;
    }

    // Have we already been here?
    if visited[row][column] {
        return true;
    }

    // Is a machine already there?
    if game.machines[row][column].is_some() {
        return true;
    }

    // Is the machine not a flying machine and is the terrain a chasm?
    if !machine.machine.is_flying() && game.board[row][column] == Terrain::Chasm {
        return true;
    }

    return false;
}

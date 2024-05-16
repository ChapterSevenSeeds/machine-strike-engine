use crate::{
    enums::{MachineDirection, MachineType},
    game::Game,
    game_machine::GameMachine,
};

pub struct Attack {
    pub direction: MachineDirection,
}

pub fn calculate_attacks(game: &Game, machine: &GameMachine) -> Vec<Attack> {
    let mut attacks: Vec<Attack> = Vec::new();

    for direction in vec![
        MachineDirection::North,
        MachineDirection::East,
        MachineDirection::South,
        MachineDirection::West,
    ] {
        match machine.machine.machine_type {
            MachineType::Melee => {
                let mut row = machine.row;
                let mut column = machine.column;
                for i in 1..machine.machine.range {
                    match direction {
                        MachineDirection::North => {
                            row -= 1;
                        }
                        MachineDirection::East => {
                            column += 1;
                        }
                        MachineDirection::South => {
                            row += 1;
                        }
                        MachineDirection::West => {
                            column -= 1;
                        }
                    }

                    if row >= 8 || column >= 8 {
                        break;
                    }

                    if row < 0 || column < 0 {
                        break;
                    }

                    if game.machines[row][column].is_some()
                        && game.machines[row][column].unwrap().side != machine.side
                    {
                        attacks.push(Attack { direction });
                        break;
                    }
                }
            }
            MachineType::Gunner => {
                let mut row = machine.row as i32;
                let mut column = machine.column as i32;
                match direction {
                    MachineDirection::North => {
                        if row - machine.machine.range < 0 {
                            continue;
                        }

                        row -= machine.machine.range;
                    }
                    MachineDirection::East => {
                        if column + machine.machine.range > 7 {
                            continue;
                        }
                        column += machine.machine.range;
                    }
                    MachineDirection::South => {
                        if row - machine.machine.range > 7 {
                            continue;
                        }
                        row += machine.machine.range;
                    }
                    MachineDirection::West => {
                        if column - machine.machine.range < 0 {
                            continue;
                        }
                        column -= machine.machine.range;
                    }
                }

                if game.machines[row as usize][column as usize].is_some()
                    && game.machines[row as usize][column as usize].unwrap().side != machine.side
                {
                    attacks.push(Attack { direction });
                }
            }
        }
    }

    return attacks;
}

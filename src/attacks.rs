use crate::{
    enums::{MachineDirection, MachineType},
    game::Game,
    game_machine::GameMachine,
};

pub struct Attack {
    pub direction: MachineDirection,
    pub machine_row: usize,
    pub machine_column: usize,
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
            MachineType::Melee | MachineType::Ram | MachineType::Swoop | MachineType::Pull => {
                match first_machine_in_attack_range(direction, game, machine) {
                    Some(attack) => {
                        attacks.push(attack);
                    }
                    None => {}
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
                    attacks.push(Attack {
                        direction,
                        machine_row: row as usize,
                        machine_column: column as usize,
                    });
                }
            },
            MachineType::Dash => {
                todo!()
            }
        }
    }

    return attacks;
}

fn first_machine_in_attack_range(
    direction: MachineDirection,
    game: &Game,
    machine: &GameMachine,
) -> Option<Attack> {
    let mut row = machine.row as i32;
    let mut column = machine.column as i32;
    for _i in 1..machine.machine.range {
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

        if game.machines[row as usize][column as usize].is_some()
            && game.machines[row as usize][column as usize].unwrap().side != machine.side
        {
            return Some(Attack {
                direction,
                machine_row: row as usize,
                machine_column: column as usize,
            });
        }
    }

    return None;
}
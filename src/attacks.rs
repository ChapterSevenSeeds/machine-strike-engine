use crate::{
    enums::{MachineDirection, MachineSide, MachineSkill, MachineType, Player, Terrain},
    game::Game,
    game_machine::GameMachine,
};

pub struct Attack {
    pub attack_direction_from_source: MachineDirection,
    pub machine_row: usize,
    pub machine_column: usize,
    pub source_row: usize,
    pub source_column: usize,
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
                    && game.machines[row as usize][column as usize].as_ref().unwrap().side != machine.side
                {
                    attacks.push(Attack {
                        attack_direction_from_source: direction,
                        machine_row: row as usize,
                        machine_column: column as usize,
                        source_row: machine.row,
                        source_column: machine.column,
                    });
                }
            }
            MachineType::Dash => {
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

                // Must be able to land on an empty space
                if game.machines[row as usize][column as usize].is_some() {
                    continue;
                }

                // Does there exist at least one enemy machine in the path?
                match first_machine_in_attack_range(direction, game, machine) {
                    Some(_) => {
                        attacks.push(Attack {
                            attack_direction_from_source: direction,
                            machine_row: row as usize,
                            machine_column: column as usize,
                            source_row: machine.row,
                            source_column: machine.column,
                        });
                    }
                    None => {}
                }
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
    for _i in 1..=machine.machine.range {
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
            && game.machines[row as usize][column as usize].as_ref().unwrap().side != machine.side
        {
            return Some(Attack {
                attack_direction_from_source: direction,
                machine_row: row as usize,
                machine_column: column as usize,
                source_row: machine.row,
                source_column: machine.column,
            });
        }
    }

    return None;
}

pub fn calculate_combat_power(game: &Game, machine: &GameMachine, attack: &Attack) -> i32 {
    // A defending machine's combat power is only the terrain it is standing on, plus any modifiers.
    // An attacking machine's combat power is the terrain it is standing on, plus any modifiers, plus its attack power.
    let mut combat_power = game.board[machine.row][machine.column] as i32;

    // If the machine is a pull machine and the terrain is marsh, add 1 combat power.
    if machine.machine.is_pull()
        && game.board[machine.row][machine.column] == Terrain::Marsh
    {
        combat_power += 1;
    }

    // All swoop machines get +1 combat power.
    if machine.machine.is_flying() {
        combat_power += 1;
    }

    if machine.side == game.turn {
        combat_power += machine.machine.attack;
        combat_power += get_skill_combat_power_modifier_when_attacking(game, machine);
        return combat_power;
    } else {
        let side_being_attacked = side_tangent_to_direction(attack.attack_direction_from_source, machine.direction);
        if side_being_attacked as i32 & machine.machine.armored_sides as i32 != 0 {
            combat_power += 1;
        } else if side_being_attacked as i32 & machine.machine.weak_sides as i32 != 0 {
            combat_power -= 1;
        }
        combat_power += get_skill_combat_power_modifier_when_defending(game, machine);
        return combat_power;
    }
}

fn side_tangent_to_direction(source_direction: MachineDirection, target_direction: MachineDirection) -> MachineSide {
    match source_direction {
        MachineDirection::North => {
            match target_direction {
                MachineDirection::North => MachineSide::Rear,
                MachineDirection::East => MachineSide::Right,
                MachineDirection::South => MachineSide::Front,
                MachineDirection::West => MachineSide::Left,
            }
        },
        MachineDirection::East => {
            match target_direction {
                MachineDirection::North => MachineSide::Left,
                MachineDirection::East => MachineSide::Rear,
                MachineDirection::South => MachineSide::Right,
                MachineDirection::West => MachineSide::Front,
            }
        },
        MachineDirection::South => {
            match target_direction {
                MachineDirection::North => MachineSide::Front,
                MachineDirection::East => MachineSide::Left,
                MachineDirection::South => MachineSide::Rear,
                MachineDirection::West => MachineSide::Right,
            }
        },
        MachineDirection::West => {
            match target_direction {
                MachineDirection::North => MachineSide::Right,
                MachineDirection::East => MachineSide::Front,
                MachineDirection::South => MachineSide::Left,
                MachineDirection::West => MachineSide::Rear,
            }
        },
    }
}

pub fn get_skill_combat_power_modifier_when_defending(game: &Game, machine: &GameMachine) -> i32 {
    match machine.machine.skill {
        MachineSkill::Shield => 1,
        _ => 0,
    }
}

pub fn get_skill_combat_power_modifier_when_attacking(game: &Game, machine: &GameMachine) -> i32 {
    let mut combat_power = 0;
    match machine.machine.skill {
        MachineSkill::Gallop => {
            if game.board[machine.row][machine.column] == Terrain::Grassland {
                combat_power = 1;
            }
        }
        MachineSkill::Stalk => {
            if game.board[machine.row][machine.column] == Terrain::Forest {
                combat_power = 1;
            }
        }
        MachineSkill::HighGround => {
            if game.board[machine.row][machine.column] == Terrain::Mountain {
                combat_power = 1;
            }
        }
        MachineSkill::Climb => {
            if game.board[machine.row][machine.column] == Terrain::Hill {
                combat_power = 1;
            }
        }
        _ => {}
    }

    // Apply any attack power gains from friendly empower machines and subtract any attack power lost from enemy blinding machines.
    let modifier = game.count_machines_with_skill_able_to_attack_target_machine(
        machine,
        MachineSkill::Empower,
        Player::Player,
    ) - game.count_machines_with_skill_able_to_attack_target_machine(
        machine,
        MachineSkill::Blind,
        Player::Opponent,
    );

    return combat_power + modifier;
}

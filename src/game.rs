use crate::{
    attacks::{calculate_attacks, calculate_combat_power, Attack},
    enums::{MachineDirection, MachineSkill, MachineState, MachineType, Player, Terrain},
    game_machine::GameMachine,
    machine::Machine,
    moves::Move,
};

macro_rules! get_mut_ref {
    ($arr:expr, $row:expr, $column:expr) => {{
        $arr.split_at_mut($row).1[0].split_at_mut($column).1[0].as_mut().unwrap()
    }};
}

macro_rules! is_some {
    ($arr:expr, $row:expr, $column:expr) => {{
        $arr.split_at_mut($row).1[0].split_at_mut($column).1[0].is_some()
    }};
}

macro_rules! get_mut_refs {
    ($arr:expr, $idx1:expr, $idx2:expr) => {{
        let len = $arr.len();
        if $idx1 >= len || $idx2 >= len {
            panic!("Index out of bounds");
        }
        if $idx1 == $idx2 {
            panic!("Indices must be different");
        }
        let (first, second) = if $idx1 < $idx2 {
            $arr.split_at_mut($idx2)
        } else {
            $arr.split_at_mut($idx1)
        };
        if $idx1 < $idx2 {
            (&mut first[$idx1], &mut second[0])
        } else {
            (&mut second[0], &mut first[$idx2])
        }
    }};
}

pub struct Game {
    pub opponent_starting_pieces: Vec<Machine>,
    pub player_starting_pieces: Vec<Machine>,
    pub board: [[Terrain; 8]; 8],
    pub machines: [[Option<GameMachine>; 8]; 8],
    pub turn: Player,
}

impl Game {
    pub fn count_machines_with_skill_able_to_attack_target_machine(
        &self,
        target: &GameMachine,
        attacking_machine_skill: MachineSkill,
        attacking_side: Player,
    ) -> i32 {
        for machine in self.machines.iter().flatten().filter(|m| {
            m.is_some()
                && m.as_ref().unwrap().machine.skill == attacking_machine_skill
                && m.as_ref().unwrap().side == attacking_side
        }) {
            let attacks = calculate_attacks(self, machine.as_ref().unwrap());
            return attacks
                .iter()
                .filter(|a| a.machine_row == target.row && a.machine_column == target.column)
                .count() as i32;
        }

        return 0;
    }

    pub fn make_move(&mut self, m: &Move) {
        let machine = self.get_machine_mut(m.source_row, m.source_column);
        if m.requires_sprint {
            machine.machine_state = MachineState::Sprinted;
        } else {
            machine.machine_state = MachineState::Moved;
        }
        self.unsafe_move_machine(
            m.source_row,
            m.source_column,
            m.destination_row,
            m.destination_column,
        );
    }

    pub fn make_attack(&mut self, attack: &Attack) {
        let attacker = get_mut_ref!(self.machines, attack.source_row, attack.source_column);
        let attacker_combat_power = 0;/* calculate_combat_power(self, attacker, attack); */

        if attacker.machine.machine_type == MachineType::Dash {
            // When attacking everything in its path, is the combat power of friendly machines used as if they were enemy machines?
            todo!("Dash attacks are not implemented yet");
        }

        let defender = get_mut_ref!(self.machines, attack.machine_row, attack.machine_column);
        let defender_combat_power = 0;/* calculate_combat_power(
            // This is weird with dash types
            self, defender, attack,
        ); */

        if attacker_combat_power <= defender_combat_power {
            todo!("Perform defense break"); // Can defense break trigger a knockback?
        }

        // Apply the attack
        defender.health -= attacker_combat_power - defender_combat_power;

        match attacker.machine.machine_type {
            MachineType::Gunner | MachineType::Melee => {
                // Attack happens and nothing else.
                attacker.machine_state = MachineState::Attacked; // Attacked and moved?
            }
            MachineType::Pull => {
                // Can a pull ram knock a machine into a chasm? If so, does anything happen?
                // Pull the enemy one terrain closer to it.
                let mut pulled_to_row = 0;
                let mut pulled_to_column = 0;
                match attack.attack_direction_from_source {
                    MachineDirection::North => {
                        pulled_to_row = attack.machine_row + 1;
                        pulled_to_column = attack.machine_column;
                    }
                    MachineDirection::East => {
                        pulled_to_row = attack.machine_row;
                        pulled_to_column = attack.machine_column - 1;
                    }
                    MachineDirection::South => {
                        pulled_to_row = attack.machine_row - 1;
                        pulled_to_column = attack.machine_column;
                    }
                    MachineDirection::West => {
                        pulled_to_row = attack.machine_row;
                        pulled_to_column = attack.machine_column + 1;
                    }
                }

                if is_some!(self.machines, pulled_to_row, pulled_to_column) {
                    // Apply one damage point to the machine that was pulled and to the machine occupying the space it was pulled to.
                    get_mut_ref!(self.machines, pulled_to_row, pulled_to_column).health -= 1;
                    defender.health -= 1;
                } else {
                    self.unsafe_move_machine(
                        attack.machine_row,
                        attack.machine_column,
                        pulled_to_row,
                        pulled_to_column,
                    );
                }
            }
            MachineType::Ram => {
                // Push the enemy one terrain away from it and move into the spot it was pushed from.
                let mut pushed_to_row = 0;
                let mut pushed_to_column = 0;
                let mut fallback_row = 0;
                let mut fallback_column = 0;
                match attack.attack_direction_from_source {
                    MachineDirection::North => {
                        pushed_to_row = attack.machine_row - 1;
                        pushed_to_column = attack.machine_column;
                        fallback_row = attack.machine_row + 1;
                        fallback_column = attack.machine_column;
                    }
                    MachineDirection::East => {
                        pushed_to_row = attack.machine_row;
                        pushed_to_column = attack.machine_column + 1;
                        fallback_row = attack.machine_row;
                        fallback_column = attack.machine_column - 1;
                    }
                    MachineDirection::South => {
                        pushed_to_row = attack.machine_row + 1;
                        pushed_to_column = attack.machine_column;
                        fallback_row = attack.machine_row - 1;
                        fallback_column = attack.machine_column;
                    }
                    MachineDirection::West => {
                        pushed_to_row = attack.machine_row;
                        pushed_to_column = attack.machine_column - 1;
                        fallback_row = attack.machine_row;
                        fallback_column = attack.machine_column + 1;
                    }
                }

                if is_some!(self.machines, pushed_to_row, pushed_to_column) {
                    // Apply one damage point to the machine that was pushed and to the machine occupying the space it was pushed to.
                    get_mut_ref!(self.machines, pushed_to_row, pushed_to_column).health -= 1;
                    defender.health -= 1;

                    // Move the machine to the next spot.
                    // Unless the attack was malformed, the fallback spot should always be empty.
                    self.unsafe_move_machine(
                        attack.source_row,
                        attack.source_column,
                        fallback_row,
                        fallback_column,
                    );
                } else {
                    self.unsafe_move_machine(
                        attack.machine_row,
                        attack.machine_column,
                        pushed_to_row,
                        pushed_to_column,
                    );
                }
            }
            MachineType::Dash => {
                todo!("Dash attacks are not implemented yet");
            }
        }
    }

    pub fn is_space_occupied(&self, row: usize, column: usize) -> bool {
        self.machines[row][column].is_some()
    }

    fn unsafe_move_machine(
        &mut self,
        source_row: usize,
        source_column: usize,
        destination_row: usize,
        destination_column: usize,
    ) {
        self.machines[destination_row][destination_column] =
            self.machines[source_row][source_column].take();
    }

    pub fn get_machine_mut(&mut self, row: usize, column: usize) -> &mut GameMachine {
        return self.machines.split_at_mut(row).1[0].split_at_mut(column).1[0].as_mut().unwrap();
    }
}


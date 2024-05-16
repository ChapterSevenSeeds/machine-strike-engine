use crate::{
    enums::{MachineDirection, MachineState, Player},
    machine::Machine,
};

pub struct GameMachine {
    pub machine: Machine,
    pub health: i32,
    pub direction: MachineDirection,
    pub row: usize,
    pub column: usize,
    pub machine_state: MachineState,
    pub attack_power: i32,
    pub defense_power: i32,
    pub side: Player
}

use crate::{enums::{MachineDirection, MachineState}, machine::Machine};

pub struct GameMachine {
    pub machine: Machine,
    pub health: i32,
    pub direction: MachineDirection,
    pub x: usize,
    pub y: usize,
    pub machine_state: MachineState,
    pub attack_power: i32,
    pub defense_power: i32,
}
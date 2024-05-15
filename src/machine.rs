use crate::enums::{MachineSkill, MachineType};

pub struct Machine {
    pub name: &'static str,
    pub skill: MachineSkill,
    pub machine_type: MachineType,
    pub armored_sides: i32,
    pub weak_sides: i32,
    pub health: i32,
    pub attack: i32,
    pub range: i32,
    pub movement: i32,
    pub points: i32,
}

impl Machine {
    pub fn is_flying(&self) -> bool {
        return self.machine_type == MachineType::Swoop;
    }
}

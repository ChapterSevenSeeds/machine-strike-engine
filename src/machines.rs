use crate::enums::{MachineSkill, MachineType, MachineSide};
use crate::machine::Machine;

pub const BEHEMOTH: Machine = Machine {
    name: "Behemoth",
    machine_type: MachineType::Gunner,
    skill: MachineSkill::Shield,
    health: 10,
    attack: 3,
    range: 2,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Left as i32 | MachineSide::Right as i32,
    points: 5,
};

pub const BELLOWBACK: Machine = Machine {
    name: "Bellowback",
    machine_type: MachineType::Gunner,
    skill: MachineSkill::Spray,
    health: 7,
    attack: 3,
    range: 2,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Left as i32 | MachineSide::Right as i32 | MachineSide::Rear as i32,
    points: 3,
};

pub const BILEGUT: Machine = Machine {
    name: "Bilegut",
    machine_type: MachineType::Pull,
    skill: MachineSkill::AlterTerrain,
    health: 9,
    attack: 3,
    range: 3,
    movement: 2,
    armored_sides: MachineSide::Left as i32 | MachineSide::Right as i32,
    weak_sides: MachineSide::Front as i32,
    points: 5,
};

pub const BRISTLEBACK: Machine = Machine {
    name: "Bristleback",
    machine_type: MachineType::Ram,
    skill: MachineSkill::Spray,
    health: 4,
    attack: 2,
    range: 1,
    movement: 3,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 2,
};

pub const BURROWER: Machine = Machine {
    name: "Burrower",
    machine_type: MachineType::Melee,
    skill: MachineSkill::None,
    health: 4,
    attack: 2,
    range: 1,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 1,
};

pub const TRACKERBURROWER: Machine = Machine {
    name: "TrackerBurrower",
    machine_type: MachineType::Melee,
    skill: MachineSkill::AlterTerrain,
    health: 4,
    attack: 2,
    range: 1,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 2,
};

pub const CHARGER: Machine = Machine {
    name: "Charger",
    machine_type: MachineType::Dash,
    skill: MachineSkill::Gallop,
    health: 4,
    attack: 2,
    range: 2,
    movement: 3,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 2,
};

pub const CLAMBERJAW: Machine = Machine {
    name: "Clamberjaw",
    machine_type: MachineType::Melee,
    skill: MachineSkill::Stalk,
    health: 8,
    attack: 3,
    range: 1,
    movement: 3,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 4,
};

pub const CLAWSTRIDER: Machine = Machine {
    name: "Clawstrider",
    machine_type: MachineType::Melee,
    skill: MachineSkill::None,
    health: 8,
    attack: 3,
    range: 2,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 3,
};

pub const ELEMENTALCLAWSTRIDER: Machine = Machine {
    name: "ElementalClawstrider",
    machine_type: MachineType::Gunner,
    skill: MachineSkill::Burn,
    health: 8,
    attack: 3,
    range: 2,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 4,
};

pub const APEXCLAWSTRIDER: Machine = Machine {
    name: "ApexClawstrider",
    machine_type: MachineType::Melee,
    skill: MachineSkill::Retaliate,
    health: 8,
    attack: 3,
    range: 1,
    movement: 2,
    armored_sides: MachineSide::Front as i32 | MachineSide::Left as i32 | MachineSide::Right as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 5,
};

pub const DREADWING: Machine = Machine {
    name: "Dreadwing",
    machine_type: MachineType::Swoop,
    skill: MachineSkill::Whiplash,
    health: 9,
    attack: 3,
    range: 3,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 5,
};

pub const FANGHORN: Machine = Machine {
    name: "Fanghorn",
    machine_type: MachineType::Ram,
    skill: MachineSkill::HighGround,
    health: 5,
    attack: 2,
    range: 2,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Left as i32 | MachineSide::Right as i32,
    points: 2,
};
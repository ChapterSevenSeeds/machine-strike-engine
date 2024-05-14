use crate::enums::{MachineSide, MachineSkill, MachineType};
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
    name: "Tracker Burrower",
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
    name: "Elemental Clawstrider",
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
    name: "Apex Clawstrider",
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

pub const FIRECLAW: Machine = Machine {
    name: "Fireclaw",
    machine_type: MachineType::Melee,
    skill: MachineSkill::Burn,
    health: 10,
    attack: 4,
    range: 2,
    movement: 3,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 7,
};

pub const FROSTCLAW: Machine = Machine {
    name: "Frostclaw",
    machine_type: MachineType::Melee,
    skill: MachineSkill::Freeze,
    health: 10,
    attack: 4,
    range: 2,
    movement: 2,
    armored_sides: MachineSide::Left as i32 | MachineSide::Right as i32,
    weak_sides: MachineSide::Front as i32,
    points: 7,
};

pub const GLINTHAWK: Machine = Machine {
    name: "Glinthawk",
    machine_type: MachineType::Swoop,
    skill: MachineSkill::None,
    health: 5,
    attack: 2,
    range: 3,
    movement: 3,
    armored_sides: MachineSide::Rear as i32,
    weak_sides: MachineSide::Front as i32,
    points: 7,
};

pub const GRAZER: Machine = Machine {
    name: "Grazer",
    machine_type: MachineType::Ram,
    skill: MachineSkill::Gallop,
    health: 4,
    attack: 1,
    range: 1,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Left as i32 | MachineSide::Right as i32,
    points: 1,
};

pub const LANCEHORN: Machine = Machine {
    name: "Lancehorn",
    machine_type: MachineType::Ram,
    skill: MachineSkill::Climb,
    health: 5,
    attack: 2,
    range: 2,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Left as i32 | MachineSide::Right as i32,
    points: 2,
};

pub const LEAPLASHER: Machine = Machine {
    name: "Leaplasher",
    machine_type: MachineType::Melee,
    skill: MachineSkill::Empower,
    health: 3,
    attack: 1,
    range: 1,
    movement: 4,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 1,
};

pub const LONGLEG: Machine = Machine {
    name: "Longleg",
    machine_type: MachineType::Gunner,
    skill: MachineSkill::Empower,
    health: 6,
    attack: 1,
    range: 2,
    movement: 4,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 2,
};

pub const PLOWHORN: Machine = Machine {
    name: "Plowhorn",
    machine_type: MachineType::Ram,
    skill: MachineSkill::Growth,
    health: 5,
    attack: 2,
    range: 1,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 1,
};

pub const RAVAGER: Machine = Machine {
    name: "Ravager",
    machine_type: MachineType::Gunner,
    skill: MachineSkill::Sweep,
    health: 9,
    attack: 2,
    range: 2,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 4,
};

pub const REDEYEWATCHER: Machine = Machine {
    name: "Redeye Watcher",
    machine_type: MachineType::Gunner,
    skill: MachineSkill::Blind,
    health: 5,
    attack: 2,
    range: 2,
    movement: 2,
    armored_sides: MachineSide::Left as i32 | MachineSide::Right as i32,
    weak_sides: MachineSide::Front as i32,
    points: 3,
};

pub const ROCKBREAKER: Machine = Machine {
    name: "Rockbreaker",
    machine_type: MachineType::Gunner,
    skill: MachineSkill::AlterTerrain,
    health: 9,
    attack: 3,
    range: 2,
    movement: 3,
    armored_sides: MachineSide::Left as i32 | MachineSide::Right as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 6,
};

pub const ROLLERBACK: Machine = Machine {
    name: "Rollerback",
    machine_type: MachineType::Melee,
    skill: MachineSkill::Retaliate,
    health: 5,
    attack: 3,
    range: 2,
    movement: 3,
    armored_sides: MachineSide::Front as i32 | MachineSide::Left as i32 | MachineSide::Right as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 4,
};

pub const SCORCHER: Machine = Machine {
    name: "Scorcher",
    machine_type: MachineType::Dash,
    skill: MachineSkill::Burn,
    health: 12,
    attack: 4,
    range: 2,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 8,
};

pub const SCRAPPER: Machine = Machine {
    name: "Scrapper",
    machine_type: MachineType::Gunner,
    skill: MachineSkill::None,
    health: 5,
    attack: 3,
    range: 2,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 2,
};

pub const SCROUNGER: Machine = Machine {
    name: "Scrounger",
    machine_type: MachineType::Melee,
    skill: MachineSkill::None,
    health: 4,
    attack: 2,
    range: 1,
    movement: 3,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 1,
};

pub const SHELLWALKER: Machine = Machine {
    name: "Shell-Walker",
    machine_type: MachineType::Melee,
    skill: MachineSkill::Shield,
    health: 7,
    attack: 2,
    range: 1,
    movement: 2,
    armored_sides: MachineSide::Front as i32 | MachineSide::Right as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 3,
};

pub const SHELLSNAPPER: Machine = Machine {
    name: "Shellsnapper",
    machine_type: MachineType::Pull,
    skill: MachineSkill::None,
    health: 10,
    attack: 3,
    range: 3,
    movement: 2,
    armored_sides: MachineSide::Left as i32 | MachineSide::Right as i32 | MachineSide::Rear as i32,
    weak_sides: MachineSide::Front as i32,
    points: 6,
};

pub const SKYDRIFTER: Machine = Machine {
    name: "Skydrifter",
    machine_type: MachineType::Swoop,
    skill: MachineSkill::None,
    health: 6,
    attack: 2,
    range: 3,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 2,
};

pub const SLAUGHTERSPINE: Machine = Machine {
    name: "Slaughterspine",
    machine_type: MachineType::Melee,
    skill: MachineSkill::Spray,
    health: 15,
    attack: 4,
    range: 1,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Left as i32 | MachineSide::Right as i32,
    points: 10,
};

pub const SLITHERFANG: Machine = Machine {
    name: "Slitherfang",
    machine_type: MachineType::Dash,
    skill: MachineSkill::AlterTerrain,
    health: 12,
    attack: 4,
    range: 3,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 9,
};

pub const SNAPMAW: Machine = Machine {
    name: "Snapmaw",
    machine_type: MachineType::Pull,
    skill: MachineSkill::None,
    health: 7,
    attack: 3,
    range: 3,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 3,
};

pub const SPIKESNOUT: Machine = Machine {
    name: "Spikesnout",
    machine_type: MachineType::Melee,
    skill: MachineSkill::None,
    health: 5,
    attack: 2,
    range: 1,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 1,
};

pub const STALKER: Machine = Machine {
    name: "Stalker",
    machine_type: MachineType::Melee,
    skill: MachineSkill::Stalk,
    health: 5,
    attack: 4,
    range: 2,
    movement: 3,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 4,
};

pub const STORMBIRD: Machine = Machine {
    name: "Stormbird",
    machine_type: MachineType::Swoop,
    skill: MachineSkill::Sweep,
    health: 9,
    attack: 3,
    range: 3,
    movement: 3,
    armored_sides: MachineSide::Rear as i32,
    weak_sides: MachineSide::Front as i32,
    points: 6,
};

pub const SUNWING: Machine = Machine {
    name: "Sunwing",
    machine_type: MachineType::Swoop,
    skill: MachineSkill::None,
    health: 7,
    attack: 3,
    range: 2,
    movement: 3,
    armored_sides: MachineSide::Rear as i32,
    weak_sides: MachineSide::Front as i32,
    points: 3,
};

pub const THUNDERJAW: Machine = Machine {
    name: "Thunderjaw",
    machine_type: MachineType::Dash,
    skill: MachineSkill::Sweep,
    health: 10,
    attack: 3,
    range: 2,
    movement: 3,
    armored_sides: MachineSide::Front as i32 | MachineSide::Rear as i32,
    weak_sides: MachineSide::Left as i32 | MachineSide::Right as i32,
    points: 6,
};

pub const TIDERIPPER: Machine = Machine {
    name: "Tideripper",
    machine_type: MachineType::Pull,
    skill: MachineSkill::None,
    health: 10,
    attack: 4,
    range: 3,
    movement: 2,
    armored_sides: MachineSide::Left as i32 | MachineSide::Right as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 6,
};

pub const TREMORTUSK: Machine = Machine {
    name: "Tremortusk",
    machine_type: MachineType::Dash,
    skill: MachineSkill::Sweep,
    health: 10,
    attack: 3,
    range: 2,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 5,
};

pub const WATERWING: Machine = Machine {
    name: "Waterwing",
    machine_type: MachineType::Pull,
    skill: MachineSkill::Whiplash,
    health: 8,
    attack: 2,
    range: 2,
    movement: 3,
    armored_sides: MachineSide::Rear as i32,
    weak_sides: MachineSide::Front as i32,
    points: 4,
};

pub const WIDEMAW: Machine = Machine {
    name: "Widemaw",
    machine_type: MachineType::Pull,
    skill: MachineSkill::None,
    health: 7,
    attack: 3,
    range: 2,
    movement: 2,
    armored_sides: MachineSide::Front as i32,
    weak_sides: MachineSide::Rear as i32,
    points: 3,
};

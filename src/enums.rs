use std::fmt;

#[derive(PartialEq, Debug)]
pub enum Terrain {
    Chasm = -2,
    Marsh = -1,
    Grassland = 0,
    Forest = 1,
    Hill = 2,
    Mountain = 3,
}

impl fmt::Display for Terrain {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{:?}", self)
    }
}

pub enum MachineSkill {
    None,
    Shield,
    Spray,
    AlterTerrain,
    Gallop,
    Stalk,
    Burn,
    Retaliate,
    Whiplash,
    HighGround,
    Freeze,
    Climb,
    Empower,
    Growth,
    Sweep,
    Blind,
}

#[derive(PartialEq)]
pub enum MachineType {
    Gunner,
    Pull,
    Ram,
    Melee,
    Dash,
    Swoop,
}

pub enum MachineSide {
    Front = 1 << 0,
    Left = 1 << 1,
    Right = 1 << 2,
    Rear = 1 << 3,
}

pub enum MachineDirection {
    North,
    East,
    South,
    West,
}

#[derive(PartialEq)]
pub enum Player {
    Player,
    Opponent,
}

pub enum MachineState {
    Ready,            // Can move and/or attack
    Moved,            // Has moved, can attack and rotate
    Sprinted,         // Has sprinted, can only overcharge
    Attacked,         // Has attacked, can move and rotate
    MovedAndAttacked, // Has moved and attacked, can rotate
    Overcharged,      // Can't do anything
}

pub enum Terrain {
    Chasm = -2,
    Marsh = -1,
    Grassland = 0,
    Forest = 1,
    Hill = 2,
    Mountain = 3,
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

pub enum MachineType {
    Gunner,
    Pull,
    Ram,
    Melee,
    Dash,
    Swoop
}

pub enum MachineSide {
    Front = 1 << 0,
    Left = 1 << 1,
    Right = 1 << 2,
    Rear = 1 << 3,
}
#pragma once
#include <string>

enum class Terrain
{
    Chasm = -2,
    Marsh = -1,
    Grassland = 0,
    Forest = 1,
    Hill = 2,
    Mountain = 3,
};

inline std::string to_string(Terrain terrain)
{
    switch (terrain)
    {
    case Terrain::Chasm:
        return "Chasm";
    case Terrain::Marsh:
        return "Marsh";
    case Terrain::Grassland:
        return "Grassland";
    case Terrain::Forest:
        return "Forest";
    case Terrain::Hill:
        return "Hill";
    case Terrain::Mountain:
        return "Mountain";
    default:
        return "Unknown";
    }
}

inline Terrain operator++(Terrain &terrain)
{
    auto value = static_cast<int>(terrain) + 1;
    if (value > static_cast<int>(Terrain::Mountain))
        return terrain = Terrain::Mountain;
    return terrain = static_cast<Terrain>(value);
}

inline Terrain operator--(Terrain &terrain)
{
    auto value = static_cast<int>(terrain) - 1;
    if (value < static_cast<int>(Terrain::Chasm))
        return terrain = Terrain::Chasm;
    return terrain = static_cast<Terrain>(value);
}

enum class MachineSkill
{
    None,
    /**
     * Gains +1 Combat Power when defending from an attack.
     */
    Shield,
    /**
     * At the start of each turn, all pieces within Attack Range lose -1 health.
     */
    Spray,
    /**
     * After inflicting damage, the terrain below your machine will lower and the terrain below the opposing machine will raise.
     */
    AlterTerrain,
    /**
     * When attacking from Grassland terrain, gain +1 Combat Power.
     */
    Gallop,
    /**
     * When attacking from Forest terrain, gain +1 Combat Power.
     */
    Stalk,
    /**
     * Attacking a machine on Forest terrain will turn that terrain into Grassland.
     */
    Burn,
    /**
     * Rotates towards its Attacker and inflicts 1 damage if it is in range.
     */
    Retaliate,
    /**
     * At the start of each turn, all pieces within Attack Range rotate 180 degrees.
     */
    Whiplash,
    /**
     * When attacking from Mountain terrain, gain +1 Combat Power.
     */
    HighGround,
    /**
     * Attacking a machine on Marsh terrain will turn that terrain into Grassland.
     */
    Freeze,
    /**
     * When attacking from Hill terrain, gain +1 Combat Power.
     */
    Climb,
    /**
     * At the start of each turn, all friendly machines within Attack Range gain +1 Attack Power. Effects can stack. *Just attack power and not defense power?*
     */
    Empower,
    /**
     * Attacking a machine on Grassland terrain will turn that terrain into Forest.
     */
    Growth,
    /**
     * Attacking hits your target and any machines perpendicular to it.
     */
    Sweep,
    /**
     * Enemy machines within Attack Range lose -1 Attack Power for the rest of the turn. Effects can stack.
     */
    Blind,
};

enum class MachineType
{
    /**
     * Always attacks at the maximum of its Attack Range.
     */
    Gunner,
    /**
     * Always attacks the first machine in its Attack Range and pulls the enemy one terrain closer to it. Gains +1 Combat Power while on Marsh terrain and can traverse through it without hindering its movement.
     */
    Pull,
    /**
     * Always attacks the first machine in its Attack Range and knocks it backwards. It will then move onto the terrain left behind by the opposing machine.
     */
    Ram,
    /**
     * Always attacks the first machine in its Attack Range.
     */
    Melee,
    /**
     * Always moves to the end of its Attack Range and damages every machine in its path, including your own, and rotates them 180 degrees. It must have an empty terrain to land on at the end of its Attack Range in order to attack.
     */
    Dash,
    /**
     * Always attacks the first machine within its Attack Range and moves next to it. Gains +1 Combat Power on all terrains and can ignore all terrain penalties.
     */
    Swoop,
};

enum class MachineSide
{
    Front = 1 << 0,
    Left = 1 << 1,
    Right = 1 << 2,
    Rear = 1 << 3,
};

inline MachineSide operator|(MachineSide a, MachineSide b)
{
    return static_cast<MachineSide>(static_cast<int>(a) | static_cast<int>(b));
}

inline MachineSide operator&(MachineSide a, MachineSide b)
{
    return static_cast<MachineSide>(static_cast<int>(a) & static_cast<int>(b));
}

enum class MachineDirection
{
    North,
    East,
    South,
    West,
};

enum class Player
{
    Player,
    Opponent,
};

enum class MachineState
{
    Ready,            // Can move and/or attack
    Moved,            // Has moved, can attack and rotate
    Sprinted,         // Has sprinted, can only overcharge
    Attacked,         // Has attacked, can move and rotate
    MovedAndAttacked, // Has moved and attacked, can rotate
    Overcharged,      // Can't do anything
};

enum class Rotation
{
    Clockwise,
    CounterClockwise,
};

enum class SpotState
{
    Empty,
    Occupied,
    BlockedOrRedundant
};

enum class Winner
{
    Player,
    Opponent,
    None
};
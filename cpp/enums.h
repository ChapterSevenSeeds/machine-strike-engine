#pragma once
#include <string>

#define ENUM_TO_STRING_CASE(name) case name: return #name

enum class Terrain {
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
        ENUM_TO_STRING_CASE(Terrain::Chasm);
        ENUM_TO_STRING_CASE(Terrain::Marsh);
        ENUM_TO_STRING_CASE(Terrain::Grassland);
        ENUM_TO_STRING_CASE(Terrain::Forest);
        ENUM_TO_STRING_CASE(Terrain::Hill);
        ENUM_TO_STRING_CASE(Terrain::Mountain);
    }
}

enum class MachineSkill {
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
     * At the start of each turn, all friendly machines within Attack Range gain +1 Attack Power. Effects can stack. *Do the friendly machines that get the increase retain that increase even if they move out of attack range?*
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

enum class MachineType {
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

enum class MachineSide {
    Front = 1 << 0,
    Left = 1 << 1,
    Right = 1 << 2,
    Rear = 1 << 3,
};

inline MachineSide operator|(MachineSide a, MachineSide b)
{
    return static_cast<MachineSide>(static_cast<int>(a) | static_cast<int>(b));
}

enum class MachineDirection {
    North,
    East,
    South,
    West,
};

enum class Player {
    Player,
    Opponent,
};

enum class MachineState {
    Ready,            // Can move and/or attack
    Moved,            // Has moved, can attack and rotate
    Sprinted,         // Has sprinted, can only overcharge
    Attacked,         // Has attacked, can move and rotate
    MovedAndAttacked, // Has moved and attacked, can rotate
    Overcharged,      // Can't do anything
};
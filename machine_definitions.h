#pragma once
#include <memory>
#include <vector>
#include <functional>
#include "machine.h"
#include "enums.h"

const static Machine BEHEMOTH(
    "Behemoth",
    MachineType::Gunner,
    MachineSkill::Shield,
    10,
    3,
    2,
    2,
    MachineSide::Front,
    MachineSide::Left | MachineSide::Right,
    5);

const static Machine BELLOWBACK(
    "Bellowback",
    MachineType::Gunner,
    MachineSkill::Spray,
    7,
    3,
    2,
    2,
    MachineSide::Front,
    MachineSide::Left | MachineSide::Right | MachineSide::Rear,
    3);

const static Machine BILEGUT(
    "Bilegut",
    MachineType::Pull,
    MachineSkill::AlterTerrain,
    9,
    3,
    3,
    2,
    MachineSide::Left | MachineSide::Right,
    MachineSide::Front,
    5);

const static Machine BRISTLEBACK(
    "Bristleback",
    MachineType::Ram,
    MachineSkill::Spray,
    4,
    2,
    1,
    3,
    MachineSide::Front,
    MachineSide::Rear,
    2);

const static Machine BURROWER(
    "Burrower",
    MachineType::Melee,
    MachineSkill::None,
    4,
    2,
    1,
    2,
    MachineSide::Front,
    MachineSide::Rear,
    1);

const static Machine TRACKERBURROWER(
    "TrackerBurrower",
    MachineType::Melee,
    MachineSkill::AlterTerrain,
    4,
    2,
    1,
    2,
    MachineSide::Front,
    MachineSide::Rear,
    2);

const static Machine CHARGER(
    "Charger",
    MachineType::Dash,
    MachineSkill::Gallop,
    4,
    2,
    2,
    3,
    MachineSide::Front,
    MachineSide::Rear,
    2);

const static Machine CLAMBERJAW(
    "Clamberjaw",
    MachineType::Melee,
    MachineSkill::Stalk,
    8,
    3,
    1,
    3,
    MachineSide::Front,
    MachineSide::Rear,
    4);

const static Machine CLAWSTRIDER(
    "Clawstrider",
    MachineType::Melee,
    MachineSkill::None,
    8,
    3,
    2,
    2,
    MachineSide::Front,
    MachineSide::Rear,
    3);

const static Machine ELEMENTALCLAWSTRIDER(
    "ElementalClawstrider",
    MachineType::Gunner,
    MachineSkill::Burn,
    8,
    3,
    2,
    2,
    MachineSide::Front,
    MachineSide::Rear,
    4);

const static Machine APEXCLAWSTRIDER(
    "ApexClawstrider",
    MachineType::Melee,
    MachineSkill::Retaliate,
    8,
    3,
    1,
    2,
    MachineSide::Front | MachineSide::Left | MachineSide::Right,
    MachineSide::Rear,
    5);

const static Machine DREADWING(
    "Dreadwing",
    MachineType::Swoop,
    MachineSkill::Whiplash,
    9,
    3,
    3,
    2,
    MachineSide::Front,
    MachineSide::Rear,
    5);

const static Machine FANGHORN(
    "Fanghorn",
    MachineType::Ram,
    MachineSkill::HighGround,
    5,
    2,
    2,
    2,
    MachineSide::Front,
    MachineSide::Left | MachineSide::Right,
    2);

const static Machine FIRECLAW(
    "Fireclaw",
    MachineType::Melee,
    MachineSkill::Burn,
    10,
    4,
    2,
    3,
    MachineSide::Front,
    MachineSide::Rear,
    7);

const static Machine FROSTCLAW(
    "Frostclaw",
    MachineType::Melee,
    MachineSkill::Freeze,
    10,
    4,
    2,
    2,
    MachineSide::Left | MachineSide::Right,
    MachineSide::Front,
    7);

const static Machine GLINTHAWK(
    "Glinthawk",
    MachineType::Swoop,
    MachineSkill::None,
    5,
    2,
    3,
    3,
    MachineSide::Rear,
    MachineSide::Front,
    7);

const static Machine GRAZER(
    "Grazer",
    MachineType::Ram,
    MachineSkill::Gallop,
    4,
    1,
    1,
    2,
    MachineSide::Front,
    MachineSide::Left | MachineSide::Right,
    1);

const static Machine LANCEHORN(
    "Lancehorn",
    MachineType::Ram,
    MachineSkill::Climb,
    5,
    2,
    2,
    2,
    MachineSide::Front,
    MachineSide::Left | MachineSide::Right,
    2);

const static Machine LEAPLASHER(
    "Leaplasher",
    MachineType::Melee,
    MachineSkill::Empower,
    3,
    1,
    1,
    4,
    MachineSide::Front,
    MachineSide::Rear,
    1);

const static Machine LONGLEG(
    "Longleg",
    MachineType::Gunner,
    MachineSkill::Empower,
    6,
    1,
    2,
    4,
    MachineSide::Front,
    MachineSide::Rear,
    2);

const static Machine PLOWHORN(
    "Plowhorn",
    MachineType::Ram,
    MachineSkill::Growth,
    5,
    2,
    1,
    2,
    MachineSide::Front,
    MachineSide::Rear,
    1);

const static Machine RAVAGER(
    "Ravager",
    MachineType::Gunner,
    MachineSkill::Sweep,
    9,
    2,
    2,
    2,
    MachineSide::Front,
    MachineSide::Rear,
    4);

const static Machine REDEYEWATCHER(
    "RedeyeWatcher",
    MachineType::Gunner,
    MachineSkill::Blind,
    5,
    2,
    2,
    2,
    MachineSide::Left | MachineSide::Right,
    MachineSide::Front,
    3);

const static Machine ROCKBREAKER(
    "Rockbreaker",
    MachineType::Gunner,
    MachineSkill::AlterTerrain,
    9,
    3,
    2,
    3,
    MachineSide::Left | MachineSide::Right,
    MachineSide::Rear,
    6);

const static Machine ROLLERBACK(
    "Rollerback",
    MachineType::Melee,
    MachineSkill::Retaliate,
    5,
    3,
    2,
    3,
    MachineSide::Front | MachineSide::Left | MachineSide::Right,
    MachineSide::Rear,
    4);

const static Machine SCORCHER(
    "Scorcher",
    MachineType::Dash,
    MachineSkill::Burn,
    12,
    4,
    2,
    2,
    MachineSide::Front,
    MachineSide::Rear,
    8);

const static Machine SCRAPPER(
    "Scrapper",
    MachineType::Gunner,
    MachineSkill::None,
    5,
    3,
    2,
    2,
    MachineSide::Front,
    MachineSide::Rear,
    2);

const static Machine SCROUNGER(
    "Scrounger",
    MachineType::Melee,
    MachineSkill::None,
    4,
    2,
    1,
    3,
    MachineSide::Front,
    MachineSide::Rear,
    1);

const static Machine SHELLWALKER(
    "Shell-Walker",
    MachineType::Melee,
    MachineSkill::Shield,
    7,
    2,
    1,
    2,
    MachineSide::Front | MachineSide::Right,
    MachineSide::Rear,
    3);

const static Machine SHELLSNAPPER(
    "Shellsnapper",
    MachineType::Pull,
    MachineSkill::None,
    10,
    3,
    3,
    2,
    MachineSide::Left | MachineSide::Right | MachineSide::Rear,
    MachineSide::Front,
    6);

const static Machine SKYDRIFTER(
    "Skydrifter",
    MachineType::Swoop,
    MachineSkill::None,
    6,
    2,
    3,
    2,
    MachineSide::Front,
    MachineSide::Rear,
    2);

const static Machine SLAUGHTERSPINE(
    "Slaughterspine",
    MachineType::Melee,
    MachineSkill::Spray,
    15,
    4,
    1,
    2,
    MachineSide::Front,
    MachineSide::Left | MachineSide::Right,
    10);

const static Machine SLITHERFANG(
    "Slitherfang",
    MachineType::Dash,
    MachineSkill::AlterTerrain,
    12,
    4,
    3,
    2,
    MachineSide::Front,
    MachineSide::Rear,
    9);

const static Machine SNAPMAW(
    "Snapmaw",
    MachineType::Pull,
    MachineSkill::None,
    7,
    3,
    3,
    2,
    MachineSide::Front,
    MachineSide::Rear,
    3);

const static Machine SPIKESNOUT(
    "Spikesnout",
    MachineType::Melee,
    MachineSkill::None,
    5,
    2,
    1,
    2,
    MachineSide::Front,
    MachineSide::Rear,
    1);

const static Machine STALKER(
    "Stalker",
    MachineType::Melee,
    MachineSkill::Stalk,
    5,
    4,
    2,
    3,
    MachineSide::Front,
    MachineSide::Rear,
    4);

const static Machine STORMBIRD(
    "Stormbird",
    MachineType::Swoop,
    MachineSkill::Sweep,
    9,
    3,
    3,
    3,
    MachineSide::Rear,
    MachineSide::Front,
    6);

const static Machine SUNWING(
    "Sunwing",
    MachineType::Swoop,
    MachineSkill::None,
    7,
    3,
    2,
    3,
    MachineSide::Rear,
    MachineSide::Front,
    3);

const static Machine THUNDERJAW(
    "Thunderjaw",
    MachineType::Dash,
    MachineSkill::Sweep,
    10,
    3,
    2,
    3,
    MachineSide::Front | MachineSide::Rear,
    MachineSide::Left | MachineSide::Right,
    6);

const static Machine TIDERIPPER(
    "Tideripper",
    MachineType::Pull,
    MachineSkill::None,
    10,
    4,
    3,
    2,
    MachineSide::Left | MachineSide::Right,
    MachineSide::Rear,
    6);

const static Machine TREMORTUSK(
    "Tremortusk",
    MachineType::Dash,
    MachineSkill::Sweep,
    10,
    3,
    2,
    2,
    MachineSide::Front,
    MachineSide::Rear,
    5);

const static Machine WATERWING(
    "Waterwing",
    MachineType::Pull,
    MachineSkill::Whiplash,
    8,
    2,
    2,
    3,
    MachineSide::Rear,
    MachineSide::Front,
    4);

const static Machine WIDEMAW(
    "Widemaw",
    MachineType::Pull,
    MachineSkill::None,
    7,
    3,
    2,
    2,
    MachineSide::Front,
    MachineSide::Rear,
    3);

const static std::vector<std::reference_wrapper<const Machine>> ALL_MACHINES = {
    std::ref(BEHEMOTH),
    std::ref(BELLOWBACK),
    std::ref(BILEGUT),
    std::ref(BRISTLEBACK),
    std::ref(BURROWER),
    std::ref(TRACKERBURROWER),
    std::ref(CHARGER),
    std::ref(CLAMBERJAW),
    std::ref(CLAWSTRIDER),
    std::ref(ELEMENTALCLAWSTRIDER),
    std::ref(APEXCLAWSTRIDER),
    std::ref(DREADWING),
    std::ref(FANGHORN),
    std::ref(FIRECLAW),
    std::ref(FROSTCLAW),
    std::ref(GLINTHAWK),
    std::ref(GRAZER),
    std::ref(LANCEHORN),
    std::ref(LEAPLASHER),
    std::ref(LONGLEG),
    std::ref(PLOWHORN),
    std::ref(RAVAGER),
    std::ref(REDEYEWATCHER),
    std::ref(ROCKBREAKER),
    std::ref(ROLLERBACK),
    std::ref(SCORCHER),
    std::ref(SCRAPPER),
    std::ref(SCROUNGER),
    std::ref(SHELLWALKER),
    std::ref(SHELLSNAPPER),
    std::ref(SKYDRIFTER),
    std::ref(SLAUGHTERSPINE),
    std::ref(SLITHERFANG),
    std::ref(SNAPMAW),
    std::ref(SPIKESNOUT),
    std::ref(STALKER),
    std::ref(STORMBIRD),
    std::ref(SUNWING),
    std::ref(THUNDERJAW),
    std::ref(TIDERIPPER),
    std::ref(TREMORTUSK),
    std::ref(WATERWING),
    std::ref(WIDEMAW),
};
#pragma once
#include <cstdint>
#include "enums.h"

class Machine {
public:
    const char* name;
    MachineSkill skill;
    MachineType machine_type;
    MachineSide armored_sides;
    MachineSide weak_sides;
    int32_t health;
    int32_t attack;
    int32_t range;
    int32_t movement;
    int32_t points;

    Machine(const char* name, MachineType machine_type, MachineSkill skill, int32_t health, int32_t attack, int32_t range, int32_t movement, MachineSide armored_sides, MachineSide weak_sides, int32_t points);
    bool is_flying() const;
    bool is_pull() const;
};
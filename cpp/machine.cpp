#include "machine.h"
#include "enums.h"

bool Machine::is_flying() const
{
    return machine_type == MachineType::Swoop;
}

bool Machine::is_pull() const
{
    return machine_type == MachineType::Pull;
}

Machine::Machine(const char *name, MachineType machine_type, MachineSkill skill, int32_t health, int32_t attack, int32_t range, int32_t movement, MachineSide armored_sides, MachineSide weak_sides, int32_t points)
    : name(name), machine_type(machine_type), skill(skill), health(health), attack(attack), range(range), movement(movement), armored_sides(armored_sides), weak_sides(weak_sides), points(points)
{
}
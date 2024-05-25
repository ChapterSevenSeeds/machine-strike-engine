#pragma once

#include <cstdint>
#include <functional>
#include "machine.h"
#include "enums.h"

class GameMachine
{
public:
    std::reference_wrapper<const Machine> machine;
    int32_t health;
    MachineDirection direction;
    int32_t row;
    int32_t column;
    MachineState machine_state;
    int32_t attack_power;
    int32_t defense_power;
    Player side;

    GameMachine(
        std::reference_wrapper<const Machine> machine,
        int32_t health,
        MachineDirection direction,
        int32_t row,
        int32_t column,
        MachineState machine_state,
        int32_t attack_power,
        int32_t defense_power,
        Player side
    ) : machine(machine),
        health(health),
        direction(direction),
        row(row),
        column(column),
        machine_state(machine_state),
        attack_power(attack_power),
        defense_power(defense_power),
        side(side) {}
};
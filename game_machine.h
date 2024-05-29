#pragma once

#include <cstdint>
#include <functional>
#include "machine.h"
#include "coord.h"
#include "enums.h"

class GameMachine
{
public:
    std::reference_wrapper<const Machine> machine;
    int32_t health;
    MachineDirection direction;
    Coord coordinates;
    MachineState machine_state;
    int32_t attack_power_modifier = 0;
    Player side;

    GameMachine(std::reference_wrapper<const Machine> machine, MachineDirection direction, Coord coordinates, MachineState machine_state, Player side);

    bool is_alive() const;
};
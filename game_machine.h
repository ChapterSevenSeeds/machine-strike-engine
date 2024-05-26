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
    int32_t attack_power_modifier;
    Player side;

    GameMachine(
        std::reference_wrapper<const Machine> machine,
        int32_t health,
        MachineDirection direction,
        Coord coordinates,
        MachineState machine_state,
        int32_t attack_power_modifier,
        Player side) : machine(machine),
                       health(health),
                       direction(direction),
                       coordinates(coordinates),
                       machine_state(machine_state),
                       attack_power_modifier(attack_power_modifier),
                       side(side) {}
};
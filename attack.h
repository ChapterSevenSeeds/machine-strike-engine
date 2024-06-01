#pragma once

#include "coord.h"
#include "enums.h"
#include <vector>

class Attack
{
public:
    // The attack direction.
    MachineDirection attack_direction_from_source;
    // The attack destination. Can point to an empty space or a machine.
    Coord destination;
    // The source of the attack. Always points to the attacking machine.
    Coord source;
    // These coordinates should always point to machines on the board.
    std::vector<Coord> affected_machines;
    MachineState causes_state;
    bool counts_as_touch;

    Attack(
        MachineDirection attack_direction_from_source,
        Coord destination,
        Coord source,
        MachineState causes_state,
        bool counts_as_touch) : attack_direction_from_source(attack_direction_from_source),
                        destination(destination),
                        source(source),
                        causes_state(causes_state),
                        counts_as_touch(counts_as_touch) {}
};
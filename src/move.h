#pragma once

#include <cstdint>
#include "enums.h"
#include "coord.h"

class Move
{
public:
    Coord source;
    Coord destination;
    int32_t found_at_depth;
    MachineState causes_state;
    // If true, this move is only used to expand the search tree and is not a valid move.
    bool expand_only;

    Move(
        Coord destination,
        int32_t found_at_depth,
        Coord source,
        MachineState causes_state,
        bool expand_only) : destination(destination),
                            found_at_depth(found_at_depth),
                            source(source),
                            causes_state(causes_state),
                            expand_only(expand_only) {}
};
#pragma once

#include <cstdint>
#include "coord.h"

class Move
{
public:
    Coord source;
    Coord destination;
    bool requires_sprint;
    int32_t found_at_depth;
    // If true, this move is only used to expand the search tree and is not a valid move.
    bool expand_only;

    Move(
        Coord destination,
        bool requires_sprint,
        int32_t found_at_depth,
        Coord source,
        bool expand_only) : destination(destination),
                            requires_sprint(requires_sprint),
                            found_at_depth(found_at_depth),
                            source(source),
                            expand_only(expand_only) {}
};
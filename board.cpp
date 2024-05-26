#pragma once

#include <optional>
#include "board.h"

void Board::unsafe_move_machine(Coord source, Coord destination)
{
    machines[destination.row][destination.column] = machines[source.row][source.column];
    machines[source.row][source.column] = std::nullopt;
    machines[destination.row][destination.column].value().get().coordinates = destination;
}

bool Board::is_space_occupied(int32_t row, int32_t column) const
{
    return machines[row][column].has_value();
}

BoardIterator Board::begin()
{
    return BoardIterator(this);
}

BoardIterator Board::end()
{
    return BoardIterator(this, 7, 7);
}

Terrain &Board::terrain_at(Coord coordinates)
{
    return terrain[coordinates.row][coordinates.column];
}

std::optional<std::reference_wrapper<GameMachine>> &Board::machine_at(Coord coordinates)
{
    return machines[coordinates.row][coordinates.column];
}
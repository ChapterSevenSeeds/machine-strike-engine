#pragma once

#include <optional>
#include "board.h"

void Board::unsafe_move_machine(int32_t source_row, int32_t source_column, int32_t destination_row, int32_t destination_column)
{
    machines[destination_row][destination_column] = machines[source_row][source_column];
    machines[source_row][source_column] = std::nullopt;
}

bool Board::is_space_occupied(int32_t row, int32_t column) const
{
    return machines[row][column].has_value();
}

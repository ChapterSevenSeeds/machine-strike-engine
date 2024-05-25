#pragma once

#include <cstdint>
#include <vector>
#include "enums.h"
#include "game_machine.h"
#include "types.h"
#include "game.h"
#include "board.h"

class Move
{
public:
    int32_t destination_row;
    int32_t destination_column;
    bool requires_sprint;
    int32_t found_at_depth;
    int32_t source_row;
    int32_t source_column;

    Move(
        int32_t destination_row,
        int32_t destination_column,
        bool requires_sprint,
        int32_t found_at_depth,
        int32_t source_row,
        int32_t source_column) : destination_row(destination_row),
                                 destination_column(destination_column),
                                 requires_sprint(requires_sprint),
                                 found_at_depth(found_at_depth),
                                 source_row(source_row),
                                 source_column(source_column) {}
};

inline bool is_spot_blocked_or_redundant(
    int32_t row,
    int32_t column,
    Board &board,
    GameMachine &machine,
    BoardType<bool> &visited)
{
    // Are we out of bounds?
    if (row < 0 || column < 0 || row > 7 || column > 7)
    {
        return true;
    }

    // Have we already been here?
    if (visited[row][column])
    {
        return true;
    }

    // Is a machine already there?
    if (board.machines[row][column].has_value())
    {
        return true;
    }

    // Is the machine not a flying machine and is the terrain a chasm?
    if (!machine.machine.get().is_flying() && board.terrain[row][column] == Terrain::Chasm)
    {
        return true;
    }

    return false;
}

std::vector<Move *> expand_moves(
    int32_t distance_travelled,
    int32_t row,
    int32_t column,
    GameMachine &machine,
    Board &board,
    BoardType<bool> &visited)
{
    // If we have already sprinted, we can't move
    if (distance_travelled > machine.machine.get().movement + 1)
    {
        return std::vector<Move *>{};
    }

    // If this isn't our first movement (not move) and if the machine is not a flying machine and if the machine is not a pull type and we are currently in a marsh, we can't move
    if (distance_travelled > 1 && !machine.machine.get().is_flying() && !machine.machine.get().is_pull() && board.terrain[row][column] == Terrain::Marsh)
    {
        return std::vector<Move *>{};
    }

    std::vector<Move *> moves;
    bool requires_sprint = distance_travelled > machine.machine.get().movement;
    int32_t new_row;
    int32_t new_column;

    for (const auto &pair : {std::make_pair(-1, 0), std::make_pair(1, 0), std::make_pair(0, -1), std::make_pair(0, 1)})
    {
        new_row = row + pair.first;
        new_column = column + pair.second;

        if (is_spot_blocked_or_redundant(new_row, new_column, board, machine, visited))
            continue;

        moves.push_back(new Move(new_row, new_column, requires_sprint, distance_travelled, machine.row, machine.column));
        visited[new_row][new_column] = true;
    }

    return moves;
}

std::vector<Move *> calculate_moves(Board &board, GameMachine &machine)
{
    BoardType<bool> visited = {false};
    std::vector<Move *> all_moves = expand_moves(1, machine.row, machine.column, machine, board, visited);

    auto expanded_index = 0;
    do
    {
        auto current_move = all_moves[expanded_index++];

        auto moves = expand_moves(
            current_move->found_at_depth + 1,
            current_move->destination_row,
            current_move->destination_column,
            machine,
            board,
            visited);

        all_moves.insert(all_moves.end(), moves.begin(), moves.end());
    } while (expanded_index < all_moves.size());

    return all_moves;
}
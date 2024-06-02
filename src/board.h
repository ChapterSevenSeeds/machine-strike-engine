#pragma once

#include "types.h"
#include <optional>
#include <vector>
#include <functional>
#include <memory>
#include "enums.h"
#include "game_machine.h"
#include "coord.h"

class BoardIterator;

class Board
{
public:
    BoardType<Terrain> terrain;
    BoardType<std::optional<GameMachine>> machines;

    Board(BoardType<Terrain> terrain, BoardType<std::optional<GameMachine>> machines);
    void move_machine(Coord source, Coord destination);
    bool is_space_occupied(Coord coord);
    BoardIterator begin();
    BoardIterator end();
    Terrain &terrain_at(Coord coordinates);
    std::optional<GameMachine> &machine_at(Coord coordinates);
    void clear_spot(Coord coord);
};

class BoardIterator
{
    Coord coord;
    Board *board;

public:
    BoardIterator(Board *board, Coord start_coords = {0, 0}) : board(board), coord(start_coords) {}

    std::optional<GameMachine> &operator*()
    {
        return board->machines[coord];
    }

    BoardIterator &operator++()
    {
        do
        {
            coord.column++;
            if (coord.column >= 8)
            {
                coord.column = 0;
                coord.row++;
            }
        } while (!board->is_space_occupied(coord) && !coord.out_of_bounds());
        return *this;
    }

    bool operator==(const BoardIterator &other)
    {
        return coord.row == other.coord.row && coord.column == other.coord.column;
    }

    bool operator!=(const BoardIterator &other)
    {
        return !(*this == other);
    }
};
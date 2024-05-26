#include "types.h"
#include <optional>
#include <functional>
#include "enums.h"
#include "game_machine.h"
#include "coord.h"

class Board
{
public:
    BoardType<Terrain> terrain;
    BoardType<std::optional<std::reference_wrapper<GameMachine>>> machines;

    void unsafe_move_machine(Coord source, Coord destination);
    bool is_space_occupied(int32_t row, int32_t column) const;
    BoardIterator begin();
    BoardIterator end();
    Terrain &terrain_at(Coord coordinates);
    std::optional<std::reference_wrapper<GameMachine>> &machine_at(Coord coordinates);
};

class BoardIterator
{
    int row = 0;
    int column = 0;
    Board *board;

public:
    BoardIterator(Board *board, int start_row = 0, int start_column = 0) : board(board), row(start_row), column(start_column) {}

    std::optional<std::reference_wrapper<GameMachine>> operator*()
    {
        if (board->machines[row][column].has_value())
        {
            return board->machines[row][column].value();
        }
        return std::nullopt;
    }

    BoardIterator &operator++()
    {
        do
        {
            column++;
            if (column >= 8)
            {
                column = 0;
                row++;
            }
        } while (!board->is_space_occupied(row, column));
        return *this;
    }

    bool operator==(const BoardIterator &other)
    {
        return row == other.row && column == other.column;
    }

    bool operator!=(const BoardIterator &other)
    {
        return !(*this == other);
    }
};
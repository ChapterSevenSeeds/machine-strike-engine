#pragma once

class Coord
{
public:
    Coord(int row, int column) : row(row), column(column) {}
    int row;
    int column;

    bool operator==(const Coord &other) const
    {
        return row == other.row && column == other.column;
    }

    bool operator!=(const Coord &other) const
    {
        return !(*this == other);
    }

    bool out_of_bounds() const
    {
        return row < 0 || row >= 8 || column < 0 || column >= 8;
    }
};
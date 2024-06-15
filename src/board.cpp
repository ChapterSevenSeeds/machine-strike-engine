#include <optional>
#include <memory>
#include "board.h"

Board::Board(BoardType<Terrain> terrain, BoardType<GameMachine*> machines) : terrain(terrain), machines(machines) {}

void Board::move_machine(Coord source, Coord destination)
{
    if (!machines[source] || machines[destination])
        return;

    machines[destination] = machines[source];
    machines[source] = nullptr;
    machines[destination]->coordinates = destination;
}

bool Board::is_space_occupied(Coord coord)
{
    return machine_at(coord) != nullptr;
}

BoardIterator Board::begin()
{
    for (int row = 0; row <= 7; ++row)
    {
        for (int column = 0; column <= 7; ++column)
        {
            Coord coord = {row, column};
            if (machine_at(coord))
            {
                return BoardIterator(this, coord);
            }
        }
    }

    return end();
}

BoardIterator Board::end()
{
    return BoardIterator(this, {8, 0});
}

Terrain &Board::terrain_at(Coord coordinates)
{
    return terrain[coordinates];
}

GameMachine* &Board::machine_at(Coord coordinates)
{
    return machines[coordinates];
}

void Board::clear_spot(Coord coord)
{
    machines[coord] = nullptr;
}
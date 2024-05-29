#include <optional>
#include <memory>
#include "board.h"

Board::Board(BoardType<Terrain> terrain, BoardType<std::optional<GameMachine>> machines) : terrain(terrain), machines(machines) {}

void Board::move_machine(Coord source, Coord destination)
{
    if (!machines[source].has_value() || machines[destination].has_value())
        return;

    machines[destination] = std::move(machines[source]);
    machines[source] = std::nullopt;
    machines[destination].value().coordinates = destination;
}

bool Board::is_space_occupied(Coord coord)
{
    return machine_at(coord).has_value();
}

BoardIterator Board::begin()
{
    return BoardIterator(this);
}

BoardIterator Board::end()
{
    return BoardIterator(this, {7, 7});
}

Terrain &Board::terrain_at(Coord coordinates)
{
    return terrain[coordinates];
}

std::optional<GameMachine> &Board::machine_at(Coord coordinates)
{
    return machines[coordinates];
}

void Board::clear_spot(Coord coord)
{
    machines[coord] = std::nullopt;
}
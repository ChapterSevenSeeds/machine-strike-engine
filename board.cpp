#include <optional>
#include "board.h"

Board::Board(BoardType<Terrain> terrain, BoardType<std::optional<std::reference_wrapper<GameMachine>>> machines) : terrain(terrain), machines(machines) {}

void Board::unsafe_move_machine(Coord source, Coord destination)
{
    machines[destination] = machines[source];
    machines[source] = std::nullopt;
    machines[destination].value().get().coordinates = destination;
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

std::optional<std::reference_wrapper<GameMachine>> &Board::machine_at(Coord coordinates)
{
    return machines[coordinates];
}
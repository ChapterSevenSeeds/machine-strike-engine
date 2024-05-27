#include <cstdint>
#include <vector>
#include <algorithm>
#include "enums.h"
#include "game_machine.h"
#include "types.h"
#include "game.h"
#include "board.h"
#include "coord.h"
#include "move.h"

inline SpotState Game::is_spot_blocked_or_redundant(Coord coord, GameMachine &machine, BoardType<bool> &visited)
{
    // Are we out of bounds?
    if (coord.out_of_bounds())
    {
        return SpotState::BlockedOrRedundant;
    }

    // Have we already been here?
    if (visited[coord])
    {
        return SpotState::BlockedOrRedundant;
    }

    // Is a machine already there?
    if (board.machine_at(coord).has_value())
    {
        return SpotState::Occupied;
    }

    // Is the machine not a flying machine and is the terrain a chasm?
    if (!machine.machine.get().is_flying() && board.terrain_at(coord) == Terrain::Chasm)
    {
        return SpotState::BlockedOrRedundant;
    }

    return SpotState::Empty;
}

std::vector<Move> Game::expand_moves(int32_t distance_travelled, Coord coord, GameMachine &machine, BoardType<bool> &visited)
{
    // If we have already sprinted, we can't move
    if (distance_travelled > machine.machine.get().movement + 1)
    {
        return std::vector<Move>{};
    }

    // If this isn't our first movement (not move) and if the machine is not a flying machine and if the machine is not a pull type and we are currently in a marsh, we can't move
    if (distance_travelled > 1 && !machine.machine.get().is_flying() && !machine.machine.get().is_pull() && board.terrain_at(coord) == Terrain::Marsh)
    {
        return std::vector<Move>{};
    }

    std::vector<Move> moves;
    bool requires_sprint = distance_travelled > machine.machine.get().movement;

    for (const auto &pair : {std::make_pair(-1, 0), std::make_pair(1, 0), std::make_pair(0, -1), std::make_pair(0, 1)})
    {
        Coord new_coord{coord.row + pair.first, coord.column + pair.second};

        auto spot_state = is_spot_blocked_or_redundant(new_coord, machine, visited);
        if (spot_state == SpotState::BlockedOrRedundant)
            continue;

        moves.emplace_back(new_coord, requires_sprint, distance_travelled, machine.coordinates, spot_state == SpotState::Occupied);
        visited[new_coord] = true;
    }

    return moves;
}

std::vector<Move> Game::calculate_moves(GameMachine &machine)
{
    BoardType<bool> visited{false};
    std::vector<Move> all_moves = expand_moves(1, machine.coordinates, machine, visited);

    auto expanded_index = 0;
    do
    {
        auto &current_move = all_moves[expanded_index++];

        auto moves = expand_moves(
            current_move.found_at_depth + 1,
            current_move.destination,
            machine,
            visited);

        all_moves.insert(all_moves.end(), moves.begin(), moves.end());
    } while (expanded_index < all_moves.size());

    auto applicable_moves_iterator = std::remove_if(all_moves.begin(), all_moves.end(), [](const Move &move)
                                                    { return move.expand_only; });
    all_moves.erase(applicable_moves_iterator, all_moves.end());
    return all_moves;
}
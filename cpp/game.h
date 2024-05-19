#pragma once

#include <vector>
#include <functional>
#include <optional>
#include "game_machine.h"
#include "enums.h"
#include "types.h"

class Game
{
public:
    std::vector<std::reference_wrapper<const Machine>> opponent_starting_pieces;
    std::vector<std::reference_wrapper<const Machine>> player_starting_pieces;
    Board<Terrain> terrain;
    Board<std::optional<std::reference_wrapper<GameMachine>>> machines;
    Player turn;

    Game(
        std::vector<std::reference_wrapper<const Machine>> opponent_starting_pieces,
        std::vector<std::reference_wrapper<const Machine>> player_starting_pieces,
        Board<Terrain> terrain,
        Board<std::optional<std::reference_wrapper<GameMachine>>> machines,
        Player turn
    ) : opponent_starting_pieces(opponent_starting_pieces),
        player_starting_pieces(player_starting_pieces),
        terrain(),
        machines(),
        turn(turn) {}
};
#pragma once

#include <vector>
#include <functional>
#include <optional>
#include "moves.h"
#include "game_machine.h"
#include "enums.h"
#include "types.h"
#include "board.h"

class Game
{
public:
    std::vector<std::reference_wrapper<const Machine>> opponent_starting_pieces;
    std::vector<std::reference_wrapper<const Machine>> player_starting_pieces;
    Board board;
    Player turn;

    Game(
        std::vector<std::reference_wrapper<const Machine>> opponent_starting_pieces,
        std::vector<std::reference_wrapper<const Machine>> player_starting_pieces,
        Board board,
        Player turn) : opponent_starting_pieces(opponent_starting_pieces),
                       player_starting_pieces(player_starting_pieces),
                       board(board),
                       turn(turn)
    {
    }

    int32_t count_machines_with_skill_able_to_attack_target_machine(
        GameMachine &target,
        MachineSkill attacking_machine_skill,
        Player attacking_side);

    void unsafe_move_machine(int32_t source_row, int32_t source_column, int32_t destination_row, int32_t destination_column);
    void make_move(Move& m);
    void make_attack(Attack& attack);
};
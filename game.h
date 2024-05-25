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

    void make_move(Move &m);
    void make_attack(Attack &attack);
    void perform_dash_attack(Attack &attack);
    void perform_gunner_attack(Attack &attack);
    void perform_melee_attack(Attack &attack);
    void perform_pull_attack(Attack &attack);
    void perform_ram_attack(Attack &attack);
    void perform_swoop_attack(Attack &attack);
    bool knock_machine(GameMachine &machine, MachineDirection direction);
    void perform_post_attack_skills(GameMachine &attacker, GameMachine &defender, Attack &attack);
    void pre_turn();
};
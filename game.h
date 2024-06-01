#pragma once

#include <vector>
#include <functional>
#include <optional>
#include "move.h"
#include "game_machine.h"
#include "enums.h"
#include "types.h"
#include "board.h"
#include "attack.h"

class Game
{
public:
    Board board;
    Player turn;
    int player_victory_points = 0;
    int opponent_victory_points = 0;
    int machines_touched = 0;
    int player_machine_count = 0;
    int opponent_machine_count = 0;

    Game(Board board, Player turn);

    // Debug
    void print_board(std::optional<GameMachine> focus_machine = std::nullopt, std::optional<std::vector<Move>> moves = std::nullopt, std::optional<std::vector<Attack>> attacks = std::nullopt);

    // Gameplay
    bool player_touched_required_machines() const;
    int get_turn_machine_count() const;
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
    void apply_attack(Attack &attack, GameMachine &attacker, uint32_t attacker_combat_power);
    GameMachine &pre_apply_attack(Attack &attack);
    void modify_machine_health(GameMachine &machine, int32_t health_change);
    Winner check_winner();
    void end_turn();
    bool can_end_turn() const;

    // Attack generation
    void populate_adjacent_attacks(GameMachine &machine, MachineDirection direction, Coord source_coodinates, std::optional<std::pair<Attack, std::optional<Attack>>> &attack, std::vector<Coord> &affected_machines);
    std::optional<std::pair<Attack, std::optional<Attack>>> first_machine_in_attack_range(MachineDirection direction, GameMachine &machine);
    std::vector<Attack> calculate_attacks(GameMachine &machine);
    int32_t get_skill_combat_power_modifier_when_defending(GameMachine &machine);
    int32_t get_skill_combat_power_modifier_when_attacking(GameMachine &machine);
    int32_t calculate_combat_power(GameMachine &machine, std::optional<MachineDirection> attack_direction);
    bool is_in_attack_range(GameMachine &attacker, GameMachine &defender);

    // Move generation
    inline SpotState is_spot_blocked_or_redundant(Coord coord, GameMachine &machine, BoardType<bool> &visited);
    std::vector<Move> expand_moves(int32_t distance_travelled, Coord coord, GameMachine &machine, BoardType<bool> &visited);
    std::vector<Move> calculate_moves(GameMachine &machine);
};
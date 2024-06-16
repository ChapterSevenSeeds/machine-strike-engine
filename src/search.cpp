#include "game.h"
#include <chrono>

inline int32_t get_score(Game &game, Player playing_as)
{
    auto winner = game.check_winner();
    if (winner == Winner::None)
        return playing_as == Player::Player ? game.player_victory_points - game.opponent_victory_points : game.opponent_victory_points - game.player_victory_points;
    if (winner == Winner::Player)
        return 1000 * (playing_as == Player::Player ? 1 : -1);
    if (winner == Winner::Opponent)
        return 1000 * (playing_as == Player::Opponent ? 1 : -1);
}

uint32_t search_helper(Game &game, int min, int max, Player playing_as, int depth, int max_depth, std::optional<Attack> &best_attack, std::optional<Move> &best_move)
{
    if (depth >= max_depth)
        return get_score(game, playing_as);

    for (auto &machine : game.turn == Player::Player ? game.player_machines : game.opponent_machines)
    {
        if (!machine->is_alive())
            continue;

        auto moves = game.calculate_moves(machine);
        auto attacks = game.calculate_attacks(machine);

        if (game.can_end_turn() && (moves.empty() && attacks.empty()))
        {
            game.end_turn();
        }

        for (bool should_end_turn : {true && game.can_end_turn(), false})
        {
            if (should_end_turn)
            {
                game.end_turn();
            }

            int best_score = Player::Player == game.turn ? max : min;
            for (auto &attack : attacks)
            {
                Game new_game(game);
                new_game.make_attack(attack);
                auto new_score = search_helper(new_game, min, max, playing_as, depth + 1, max_depth, best_attack, best_move);
                if (game.turn == Player::Player)
                {
                    if (new_score > best_score)
                    {
                        best_score = new_score;
                        best_attack = attack;
                    }
                    max = std::max(max, best_score);
                }
                else
                {
                    if (new_score < best_score)
                    {
                        best_score = new_score;
                        best_attack = attack;
                    }
                    min = std::min(min, best_score);
                }
            }

            for (auto &move : moves)
            {
                Game new_game(game);
                new_game.make_move(move);
                auto new_score = search_helper(new_game, min, max, playing_as, depth + 1, max_depth, best_attack, best_move);

                if (game.turn == Player::Player)
                {
                    if (new_score > best_score)
                    {
                        best_score = new_score;
                        best_move = move;
                        best_attack = std::nullopt;
                    }
                    max = std::max(max, best_score);
                }
                else
                {
                    if (new_score < best_score)
                    {
                        best_score = new_score;
                        best_move = move;
                        best_attack = std::nullopt;
                    }
                    min = std::min(min, best_score);
                }
            }
        }
    }

    return get_score(game, playing_as);
}

void Game::search(uint32_t seconds)
{
    std::optional<Attack> best_attack = std::nullopt;
    std::optional<Move> best_move = std::nullopt;

    auto score = search_helper(*this, INT32_MIN, INT32_MAX, turn, 0, 6, best_attack, best_move);
    printf("Score: %d\n", score);
}
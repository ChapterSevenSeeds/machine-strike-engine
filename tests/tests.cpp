#include <gtest/gtest.h>
#include "../src/game.h"
#include "../src/board.h"
#include "../src/enums.h"
#include "../src/types.h"
#include <optional>
#include "../src/game_machine.h"
#include "../src/machine_definitions.h"
#include "../src/attack.h"
#include "../src/machine.h"

auto all_grassland = BoardType{Terrain::Grassland};

Game create_game(BoardType<Terrain> terrain, Player turn, std::initializer_list<GameMachine> machines)
{
  BoardType<std::optional<GameMachine>> machine_map{std::nullopt};
  for (auto machine : machines)
  {
    machine_map[machine.coordinates] = machine;
  }
  return Game(machine_map, terrain, turn);
}

#define MAKE_FIRST_ATTACK(game, machine) game.make_attack(game.calculate_attacks(machine)[0])
#define MAKE_FIRST_MOVE(game, machine) game.make_move(game.calculate_moves(machine)[0])

std::vector<Move> moves_that_cause_state(Game &game, GameMachine *machine, MachineState state)
{
  auto all_moves = game.calculate_moves(machine);
  auto applicable_moves_iterator = std::remove_if(all_moves.begin(), all_moves.end(), [state](const Move &move)
                                                  { return move.causes_state != state; });
  all_moves.erase(applicable_moves_iterator, all_moves.end());
  return all_moves;
}

Move get_move_with_destination_coords(Game &game, GameMachine *machine, Coord destination, bool causes_overcharge = false)
{
  auto all_moves = game.calculate_moves(machine);
  return *std::find_if(all_moves.begin(), all_moves.end(), [&destination, &causes_overcharge](const Move &move)
                       { return move.destination == destination && ((causes_overcharge && move.causes_state == MachineState::Overcharged) || !causes_overcharge); });
}

TEST(machine_strike_engine_test, Melee_knockback_into_wall_damages_attacker_once_defender_twice)
{
  auto game = create_game(all_grassland, Player::Player,
                          {GameMachine(std::ref(BURROWER), MachineDirection::South, {6, 0}, MachineState::Ready, Player::Player),
                           GameMachine(std::ref(BURROWER), MachineDirection::North, {7, 0}, MachineState::Ready, Player::Opponent)});
  auto friendly_burrower = game.board->machine_at({6, 0});
  auto enemy_burrower = game.board->machine_at({7, 0});

  game.board->terrain_at({7, 0}) = Terrain::Hill; // Give the enemy a hill so that the combat power matches the attacker

  MAKE_FIRST_ATTACK(game, friendly_burrower);

  EXPECT_EQ(friendly_burrower->health, BURROWER.health - 1);           // Defense break caused us 1 point of damage
  EXPECT_EQ(enemy_burrower->health, BURROWER.health - 2);              // Defense break plus knockback into wall caused the enemy 2 points of damage
  EXPECT_EQ(friendly_burrower->machine_state, MachineState::Attacked); // We attacked
}

TEST(machine_strike_engine_test, Melee_knockback_damages_both_once_moves_defender_back)
{
  auto game = create_game(all_grassland, Player::Player,
                          {GameMachine(std::ref(BURROWER), MachineDirection::South, {5, 0}, MachineState::Ready, Player::Player),
                           GameMachine(std::ref(BURROWER), MachineDirection::North, {6, 0}, MachineState::Ready, Player::Opponent)});
  auto friendly_burrower = game.board->machine_at({5, 0});
  auto enemy_burrower = game.board->machine_at({6, 0});
  game.board->terrain_at({6, 0}) = Terrain::Hill; // Give the enemy a hill so that the combat power matches the attacker

  MAKE_FIRST_ATTACK(game, friendly_burrower);

  EXPECT_EQ(friendly_burrower->health, BURROWER.health - 1);           // Defense break caused us 1 point of damage
  EXPECT_EQ(friendly_burrower->machine_state, MachineState::Attacked); // We attacked
  EXPECT_TRUE(enemy_burrower->coordinates == Coord(7, 0));             // The enemy was knocked back
  EXPECT_EQ(enemy_burrower->health, BURROWER.health - 1);              // Defense break caused the enemy 1 point of damage
}

TEST(machine_strike_engine_test, Pull_machine_next_to_defender_pulls_into_attacker)
{
  auto game = create_game(all_grassland, Player::Player,
                          {GameMachine(std::ref(SNAPMAW), MachineDirection::South, {5, 0}, MachineState::Ready, Player::Player),
                           GameMachine(std::ref(CLAMBERJAW), MachineDirection::North, {6, 0}, MachineState::Ready, Player::Opponent)});

  auto friendly = game.board->machine_at({5, 0});
  auto enemy = game.board->machine_at({6, 0});

  MAKE_FIRST_ATTACK(game, friendly);

  EXPECT_EQ(friendly->health, SNAPMAW.health - 1);            // Pulling the enemy into us caused us 1 point of damage
  EXPECT_EQ(friendly->machine_state, MachineState::Attacked); // We attacked
  EXPECT_EQ(enemy->health, CLAMBERJAW.health - 3);            // Attack power of 3 + 0 for grassland - 1 for attacking the armored front = 2 points of damage. Plus one more for pulling into us.
}

TEST(machine_strke_engine_test, Attacking_forces_machine_to_move)
{
  auto game = create_game(all_grassland, Player::Player,
                          {GameMachine(std::ref(BURROWER), MachineDirection::South, {0, 0}, MachineState::Ready, Player::Player),
                           GameMachine(std::ref(BURROWER), MachineDirection::South, {6, 0}, MachineState::Ready, Player::Player),
                           GameMachine(std::ref(BURROWER), MachineDirection::North, {7, 0}, MachineState::Ready, Player::Opponent)});
  auto friendly_burrower = game.board->machine_at({6, 0});
  auto friendly_burrower2 = game.board->machine_at({0, 0});
  auto enemy_burrower = game.board->machine_at({7, 0});

  MAKE_FIRST_ATTACK(game, friendly_burrower);

  EXPECT_EQ(game.state, GameState::TouchFirstMachine);                                             // We haven't moved yet.
  EXPECT_TRUE(game.must_move_last_touched_machine);                                                // We must move the last touched machine
  EXPECT_EQ(game.last_touched_machine, friendly_burrower);                                         // The last touched machine is the one that attacked
  EXPECT_EQ(game.calculate_moves(friendly_burrower2).size(), 0);                                   // We can't move the other machine
  EXPECT_NE(game.calculate_moves(friendly_burrower).size(), 0);                                    // We can move the attacking machine
  EXPECT_EQ(game.calculate_attacks(friendly_burrower).size(), 0);                                  // We can't overcharge attack until we move
  EXPECT_EQ(moves_that_cause_state(game, friendly_burrower, MachineState::Overcharged).size(), 0); // We also can't overcharge move the attacker until it has moved like normal

  MAKE_FIRST_MOVE(game, friendly_burrower);

  EXPECT_EQ(game.state, GameState::TouchSecondMachine);                                                            // We must now move the second machine
  EXPECT_FALSE(game.must_move_last_touched_machine);                                                               // We don't have to move the last touched machine anymore
  EXPECT_NE(game.calculate_moves(friendly_burrower2).size(), 0);                                                   // We can move the other machine
  EXPECT_NE(game.calculate_moves(friendly_burrower).size() + game.calculate_attacks(friendly_burrower).size(), 0); // We can overcharge the attacker if we want
  EXPECT_NE(moves_that_cause_state(game, friendly_burrower, MachineState::Overcharged).size(), 0);                 // We can now overcharge move the attacker
}

TEST(machine_strke_engine_test, Single_friendly_moving_logic)
{
  auto game = create_game(all_grassland, Player::Player,
                          {GameMachine(std::ref(SNAPMAW), MachineDirection::South, {0, 0}, MachineState::Ready, Player::Player)});
  auto friendly = game.board->machine_at({0, 0});

  auto move = get_move_with_destination_coords(game, friendly, {3, 0}); // Sprint 3 places
  game.make_move(move);

  EXPECT_EQ(friendly->coordinates, Coord(3, 0));
  EXPECT_EQ(friendly->machine_state, MachineState::Sprinted);
  EXPECT_EQ(game.state, GameState::TouchSecondMachine); // We must now move the second machine

  move = get_move_with_destination_coords(game, friendly, {6, 0}, true); // Overcharge sprint 3 places again
  game.make_move(move);

  EXPECT_EQ(friendly->coordinates, Coord(6, 0));
  EXPECT_EQ(friendly->machine_state, MachineState::Overcharged);
  EXPECT_EQ(game.state, GameState::TouchSecondMachine); // We must now move the second machine
  EXPECT_EQ(friendly->health, SNAPMAW.health - 2);      // We took 2 points of damage for overcharging

  move = get_move_with_destination_coords(game, friendly, {6, 3}); // Sprint 3 places
  game.make_move(move);

  EXPECT_EQ(friendly->coordinates, Coord(6, 3));
  EXPECT_EQ(friendly->machine_state, MachineState::Sprinted);
  EXPECT_EQ(game.state, GameState::MustEndTurn); // We have moved two machines

  move = get_move_with_destination_coords(game, friendly, {6, 6}, true); // Overcharge sprint 3 places again
  game.make_move(move);

  EXPECT_EQ(friendly->coordinates, Coord(6, 6));
  EXPECT_EQ(friendly->machine_state, MachineState::Overcharged);
  EXPECT_EQ(game.state, GameState::MustEndTurn);   // We have moved two machines
  EXPECT_EQ(friendly->health, SNAPMAW.health - 4); // We took 2 points of damage for overcharging again
}
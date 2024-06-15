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

std::vector<Attack> non_overcharge_attacks(Game &game, GameMachine *machine)
{
  auto all_attacks = game.calculate_attacks(machine);
  auto applicable_attacks_iterator = std::remove_if(all_attacks.begin(), all_attacks.end(), [](const Attack &attack)
                                                    { return attack.causes_state == MachineState::Overcharged; });
  all_attacks.erase(applicable_attacks_iterator, all_attacks.end());
  return all_attacks;
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

TEST(machine_strike_engine_test, Dual_friendly_moving_logic)
{
  auto game = create_game(all_grassland, Player::Player,
                          {GameMachine(std::ref(SNAPMAW), MachineDirection::South, {0, 0}, MachineState::Ready, Player::Player),
                           GameMachine(std::ref(SNAPMAW), MachineDirection::South, {0, 1}, MachineState::Ready, Player::Player)});

  auto friendly1 = game.board->machine_at({0, 0});
  auto friendly2 = game.board->machine_at({0, 1});

  MAKE_FIRST_MOVE(game, friendly1);

  EXPECT_EQ(game.state, GameState::TouchSecondMachine); // We must now move the second machine
  EXPECT_EQ(friendly1->machine_state, MachineState::Moved);
  EXPECT_EQ(friendly2->machine_state, MachineState::Ready);
  EXPECT_NE(game.calculate_moves(friendly1).size(), 0); // We can overcharge the first machine

  MAKE_FIRST_MOVE(game, friendly1); // Overcharge the first machine

  EXPECT_EQ(game.state, GameState::TouchSecondMachine); // We must now move the second machine
  EXPECT_EQ(friendly1->machine_state, MachineState::Overcharged);
  EXPECT_EQ(friendly2->machine_state, MachineState::Ready);
  EXPECT_EQ(friendly1->health, SNAPMAW.health - 2);     // We took 2 points of damage for overcharging
  EXPECT_EQ(game.calculate_moves(friendly1).size(), 0); // We can no longer move the first machine since we have a second machine
  EXPECT_NE(game.calculate_moves(friendly2).size(), 0); // We can move the second machine

  MAKE_FIRST_MOVE(game, friendly2);

  EXPECT_EQ(game.state, GameState::MustEndTurn); // We have moved two machines
  EXPECT_EQ(friendly2->machine_state, MachineState::Moved);
  EXPECT_EQ(friendly1->machine_state, MachineState::Overcharged);
  EXPECT_NE(game.calculate_moves(friendly2).size(), 0); // We can overcharge the second machine

  MAKE_FIRST_MOVE(game, friendly2); // Overcharge the second machine

  EXPECT_EQ(game.state, GameState::MustEndTurn); // We have moved two machines
  EXPECT_EQ(friendly2->machine_state, MachineState::Overcharged);
  EXPECT_EQ(friendly1->machine_state, MachineState::Overcharged);
  EXPECT_EQ(friendly2->health, SNAPMAW.health - 2); // We took 2 points of damage for overcharging
}

TEST(machine_strike_engine_test, Machine_can_no_longer_attack_after_sprinting)
{
  auto game = create_game(all_grassland, Player::Player,
                          {GameMachine(std::ref(SNAPMAW), MachineDirection::South, {0, 0}, MachineState::Ready, Player::Player),
                           GameMachine(std::ref(SNAPMAW), MachineDirection::South, {0, 1}, MachineState::Ready, Player::Player),
                           GameMachine(std::ref(SNAPMAW), MachineDirection::South, {4, 0}, MachineState::Ready, Player::Opponent),
                           GameMachine(std::ref(SNAPMAW), MachineDirection::South, {0, 3}, MachineState::Ready, Player::Opponent)});
  auto friendly1 = game.board->machine_at({0, 0});
  auto friendly2 = game.board->machine_at({0, 1});

  auto move = get_move_with_destination_coords(game, friendly1, {3, 0}); // Sprint 3 places
  game.make_move(move);

  EXPECT_EQ(friendly1->coordinates, Coord(3, 0));
  EXPECT_EQ(friendly1->machine_state, MachineState::Sprinted);
  EXPECT_EQ(game.state, GameState::TouchSecondMachine);         // We must now move the second machine
  EXPECT_EQ(non_overcharge_attacks(game, friendly1).size(), 0); // We can no longer attack without overcharging

  move = get_move_with_destination_coords(game, friendly2, {0, 2}); // Move the second machine just one space
  game.make_move(move);

  EXPECT_EQ(friendly2->coordinates, Coord(0, 2));
  EXPECT_EQ(friendly2->machine_state, MachineState::Moved);
  EXPECT_EQ(game.state, GameState::MustEndTurn);
  EXPECT_NE(non_overcharge_attacks(game, friendly2).size(), 0); // We can attack with the second machine without overcharging
}

TEST(machine_strike_engine_test, Move_before_attack_doesnt_require_subsequent_attack)
{
  auto game = create_game(all_grassland, Player::Player,
                          {GameMachine(std::ref(LONGLEG), MachineDirection::North, {6, 1}, MachineState::Ready, Player::Player),
                           GameMachine(std::ref(GRAZER), MachineDirection::North, {7, 3}, MachineState::Ready, Player::Player),
                           GameMachine(std::ref(SCRAPPER), MachineDirection::South, {2, 2}, MachineState::Ready, Player::Opponent)});

  auto friendly = game.board->machine_at({6, 1});
  auto friendly2 = game.board->machine_at({7, 3});
  auto move = get_move_with_destination_coords(game, friendly, {4, 2});
  game.make_move(move);
  MAKE_FIRST_ATTACK(game, friendly);

  EXPECT_TRUE(friendly->has_moved());
  EXPECT_TRUE(friendly->has_attacked());                         // We moved and attacked
  EXPECT_NE(friendly->machine_state, MachineState::Overcharged); // But we did not overcharge

  EXPECT_NE(game.calculate_moves(friendly2).size(), 0); // We should be able to move the second machine.
}

TEST(machine_strike_engine_test, Normal_ram_attack_knocks_machine_and_attacker_takes_place)
{
  auto game = create_game(all_grassland, Player::Player,
                          {GameMachine(std::ref(GRAZER), MachineDirection::North, {2, 3}, MachineState::Ready, Player::Player),
                           GameMachine(std::ref(BURROWER), MachineDirection::North, {1, 3}, MachineState::Ready, Player::Opponent)});

  auto friendly = game.board->machine_at({2, 3});
  auto enemy = game.board->machine_at({1, 3});

  MAKE_FIRST_ATTACK(game, friendly);

  EXPECT_TRUE(enemy->coordinates == Coord(0, 3));    // The enemy was knocked back
  EXPECT_TRUE(friendly->coordinates == Coord(1, 3)); // The attacker took the enemy's place
}

TEST(machine_strike_engine_test, Ram_attack_blocked_by_machine_causes_damage_to_both_machines)
{
  auto game = create_game(all_grassland, Player::Player,
                          {GameMachine(std::ref(GRAZER), MachineDirection::North, {2, 3}, MachineState::Ready, Player::Player),
                           GameMachine(std::ref(BURROWER), MachineDirection::North, {1, 3}, MachineState::Ready, Player::Opponent),
                           GameMachine(std::ref(BURROWER), MachineDirection::North, {0, 3}, MachineState::Ready, Player::Opponent)});

  auto friendly = game.board->machine_at({2, 3});
  auto enemy1 = game.board->machine_at({1, 3});
  auto enemy2 = game.board->machine_at({0, 3});

  MAKE_FIRST_ATTACK(game, friendly);

  EXPECT_TRUE(enemy1->coordinates == Coord(1, 3));
  EXPECT_TRUE(enemy2->coordinates == Coord(0, 3));
  EXPECT_EQ(enemy1->health, GRAZER.health - 2);      // One point of health for the attack and one for getting rammed into another machine
  EXPECT_EQ(enemy2->health, GRAZER.health - 1);      // One point of health for getting rammed into by the defender
  EXPECT_TRUE(friendly->coordinates == Coord(2, 3)); // We couldn't move
}
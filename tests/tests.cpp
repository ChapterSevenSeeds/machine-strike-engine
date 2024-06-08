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
  return Game(Board(terrain, machine_map), turn);
}

#define MAKE_FIRST_ATTACK(game, machine) game.make_attack(game.calculate_attacks(machine)[0])

TEST(machine_strike_engine_test, Melee_knockback_into_wall_damages_attacker_once_defender_twice)
{
  auto game = create_game(all_grassland, Player::Player,
                          {GameMachine(std::ref(BURROWER), MachineDirection::South, {6, 0}, MachineState::Ready, Player::Player),
                           GameMachine(std::ref(BURROWER), MachineDirection::North, {7, 0}, MachineState::Ready, Player::Opponent)});
  auto &friendly_burrower = game.board.machine_at({6, 0}).value();
  auto &enemy_burrower = game.board.machine_at({7, 0}).value();

  game.board.terrain_at({7, 0}) = Terrain::Hill; // Give the enemy a hill so that the combat power matches the attacker

  MAKE_FIRST_ATTACK(game, friendly_burrower);

  EXPECT_EQ(friendly_burrower.health, BURROWER.health - 1);           // Defense break caused us 1 point of damage
  EXPECT_EQ(enemy_burrower.health, BURROWER.health - 2);              // Defense break plus knockback into wall caused the enemy 2 points of damage
  EXPECT_EQ(friendly_burrower.machine_state, MachineState::Attacked); // We attacked
}

TEST(machine_strike_engine_test, Melee_knockback_damages_both_once_moves_defender_back)
{
  auto game = create_game(all_grassland, Player::Player,
                          {GameMachine(std::ref(BURROWER), MachineDirection::South, {5, 0}, MachineState::Ready, Player::Player),
                           GameMachine(std::ref(BURROWER), MachineDirection::North, {6, 0}, MachineState::Ready, Player::Opponent)});
  auto &friendly_burrower = game.board.machine_at({5, 0}).value();
  auto &enemy_burrower = game.board.machine_at({6, 0}).value();
  game.board.terrain_at({6, 0}) = Terrain::Hill; // Give the enemy a hill so that the combat power matches the attacker

  MAKE_FIRST_ATTACK(game, friendly_burrower);

  EXPECT_EQ(friendly_burrower.health, BURROWER.health - 1);           // Defense break caused us 1 point of damage
  EXPECT_EQ(friendly_burrower.machine_state, MachineState::Attacked); // We attacked
  EXPECT_TRUE(enemy_burrower.coordinates == Coord(7, 0));             // The enemy was knocked back
  EXPECT_EQ(enemy_burrower.health, BURROWER.health - 1);              // Defense break caused the enemy 1 point of damage
}

TEST(machine_strike_engine_test, Pull_machine_next_to_defender_pulls_into_attacker)
{
  auto game = create_game(all_grassland, Player::Player,
                          {GameMachine(std::ref(SNAPMAW), MachineDirection::South, {5, 0}, MachineState::Ready, Player::Player),
                           GameMachine(std::ref(CLAMBERJAW), MachineDirection::North, {6, 0}, MachineState::Ready, Player::Opponent)});

  auto &friendly = game.board.machine_at({5, 0}).value();
  auto &enemy = game.board.machine_at({6, 0}).value();

  MAKE_FIRST_ATTACK(game, friendly);

  EXPECT_EQ(friendly.health, SNAPMAW.health - 1);            // Pulling the enemy into us caused us 1 point of damage
  EXPECT_EQ(friendly.machine_state, MachineState::Attacked); // We attacked
  EXPECT_EQ(enemy.health, CLAMBERJAW.health - 3);            // Attack power of 3 + 0 for grassland - 1 for attacking the armored front = 2 points of damage. Plus one more for pulling into us.
}
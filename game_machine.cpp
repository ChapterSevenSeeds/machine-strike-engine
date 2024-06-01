#include "game_machine.h"

GameMachine::GameMachine(
    std::reference_wrapper<const Machine> machine,
    MachineDirection direction,
    Coord coordinates,
    MachineState machine_state,
    Player side) : machine(machine),
                   health(machine.get().health),
                   direction(direction),
                   coordinates(coordinates),
                   machine_state(machine_state),
                   side(side) {}

bool GameMachine::is_alive() const
{
    return health > 0;
}

bool GameMachine::has_moved() const
{
    return machine_state == MachineState::Moved || machine_state == MachineState::MovedAndAttacked || machine_state == MachineState::Sprinted;
}

bool GameMachine::has_attacked() const
{
    return machine_state == MachineState::Attacked || machine_state == MachineState::MovedAndAttacked;
}

bool GameMachine::has_been_touched() const
{
    return has_moved() || has_attacked() || machine_state == MachineState::Overcharged;
}
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
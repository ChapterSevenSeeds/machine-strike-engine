#include "enums.h"
#include <utility>

MachineDirection opposite_direction(MachineDirection direction)
{
    switch (direction)
    {
    case MachineDirection::North:
        return MachineDirection::South;
    case MachineDirection::South:
        return MachineDirection::North;
    case MachineDirection::West:
        return MachineDirection::East;
    case MachineDirection::East:
        return MachineDirection::West;
    }
}

std::pair<int32_t, int32_t> traverse_direction(int32_t row, int32_t column, MachineDirection direction, int32_t distance = 1)
{
    switch (direction)
    {
    case MachineDirection::North:
        return {row - distance, column};
    case MachineDirection::South:
        return {row + distance, column};
    case MachineDirection::West:
        return {row, column - distance};
    case MachineDirection::East:
        return {row, column + distance};
    }
}

MachineDirection rotate_direction(MachineDirection direction, Rotation rotation)
{
    switch (direction)
    {
    case MachineDirection::North:
        return rotation == Rotation::Clockwise ? MachineDirection::East : MachineDirection::West;
    case MachineDirection::East:
        return rotation == Rotation::Clockwise ? MachineDirection::South : MachineDirection::North;
    case MachineDirection::South:
        return rotation == Rotation::Clockwise ? MachineDirection::West : MachineDirection::East;
    case MachineDirection::West:
        return rotation == Rotation::Clockwise ? MachineDirection::North : MachineDirection::South;
    }
}

void rotate_machine(MachineDirection &direction, Rotation rotation, int32_t turns)
{
    while (turns-- > 0)
    {
        direction = rotate_direction(direction, rotation);
    }
}
#include "enums.h"
#include "coord.h"
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

Coord traverse_direction(Coord source, MachineDirection direction, int32_t distance = 1)
{
    switch (direction)
    {
    case MachineDirection::North:
        return {source.row - distance, source.column};
    case MachineDirection::South:
        return {source.row + distance, source.column};
    case MachineDirection::West:
        return {source.row, source.column - distance};
    case MachineDirection::East:
        return {source.row, source.column + distance};
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
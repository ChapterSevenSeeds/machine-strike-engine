#pragma once

#include "enums.h"
#include "coord.h"
#include <utility>
#include <stdexcept>

inline MachineDirection opposite_direction(MachineDirection direction)
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

    throw std::invalid_argument("Invalid direction");
}

inline Coord traverse_direction(Coord source, MachineDirection direction, int32_t distance = 1)
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

    throw std::invalid_argument("Invalid direction");
}

inline MachineDirection rotate_direction(MachineDirection direction, Rotation rotation)
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

    throw std::invalid_argument("Invalid direction");
}

inline void rotate_machine(MachineDirection &direction, Rotation rotation, int32_t turns)
{
    while (turns-- > 0)
    {
        direction = rotate_direction(direction, rotation);
    }
}

// Returns the direction from source to destination.
// Assumes that source and destination are on either the same row or column.
inline MachineDirection get_direction(Coord source, Coord destination)
{
    if (source.row == destination.row)
    {
        if (source.column < destination.column)
        {
            return MachineDirection::East;
        }
        else
        {
            return MachineDirection::West;
        }
    }
    else
    {
        if (source.row < destination.row)
        {
            return MachineDirection::South;
        }
        else
        {
            return MachineDirection::North;
        }
    }
}

inline MachineSide side_tangent_to_direction(MachineDirection source_direction, MachineDirection target_direction)
{
    switch (source_direction)
    {
    case MachineDirection::North:
    {
        switch (target_direction)
        {
        case MachineDirection::North:
            return MachineSide::Rear;
        case MachineDirection::East:
            return MachineSide::Right;
        case MachineDirection::South:
            return MachineSide::Front;
        case MachineDirection::West:
            return MachineSide::Left;
        }
    }
    case MachineDirection::East:
    {
        switch (target_direction)
        {
        case MachineDirection::North:
            return MachineSide::Left;
        case MachineDirection::East:
            return MachineSide::Rear;
        case MachineDirection::South:
            return MachineSide::Right;
        case MachineDirection::West:
            return MachineSide::Front;
        }
    }
    case MachineDirection::South:
    {
        switch (target_direction)
        {
        case MachineDirection::North:
            return MachineSide::Front;
        case MachineDirection::East:
            return MachineSide::Left;
        case MachineDirection::South:
            return MachineSide::Rear;
        case MachineDirection::West:
            return MachineSide::Right;
        }
    }
    case MachineDirection::West:
    {
        switch (target_direction)
        {
        case MachineDirection::North:
            return MachineSide::Right;
        case MachineDirection::East:
            return MachineSide::Front;
        case MachineDirection::South:
            return MachineSide::Left;
        case MachineDirection::West:
            return MachineSide::Rear;
        }
    }
    }

    throw std::invalid_argument("Invalid direction");
}
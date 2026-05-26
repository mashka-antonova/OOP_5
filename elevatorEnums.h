#ifndef ELEVATORENUMS_H
#define ELEVATORENUMS_H

#include <QString>
#include <QMap>

enum class Direction {
    Up,
    Down,
    None,
};

enum class CabinState {
    Moving,
    Stopped,
    PreparingToMove,
    Blocked,
    Unblocked,
};

enum class DoorState {
    Opening,
    Opened,
    Closing,
    Closed,
};

enum class ControllerState {
    Idle,
    Moving,
    UpdatingTarget,
    ReachedTarget,
};

enum class RequestType {
    ExternalCall,
    CabinCommand,
};

constexpr bool operator<(Direction a, Direction b)
{
    return static_cast<int>(a) < static_cast<int>(b);
}

class ElevatorEnums {
public:
    ElevatorEnums() = delete;

    static QString directionName(Direction d)
    {
        static const QMap<Direction, QString> names = {
            {Direction::Up, "Up"},
            {Direction::Down, "Down"},
            {Direction::None, "None"},
        };
        return names.value(d, "Unknown");
    }

    static QString cabinStateName(CabinState s)
    {
        static const QMap<CabinState, QString> names = {
            {CabinState::Moving, "Moving"},
            {CabinState::Stopped, "Stopped"},
            {CabinState::PreparingToMove, "PreparingToMove"},
            {CabinState::Blocked, "Blocked"},
            {CabinState::Unblocked, "Unblocked"},
        };
        return names.value(s, "Unknown");
    }

    static QString doorStateName(DoorState s)
    {
        static const QMap<DoorState, QString> names = {
            {DoorState::Opening, "Opening"},
            {DoorState::Opened, "Opened"},
            {DoorState::Closing, "Closing"},
            {DoorState::Closed, "Closed"},
        };
        return names.value(s, "Unknown");
    }

    static QString controllerStateName(ControllerState s)
    {
        static const QMap<ControllerState, QString> names = {
            {ControllerState::Idle, "Idle"},
            {ControllerState::Moving, "Moving"},
            {ControllerState::UpdatingTarget, "UpdatingTarget"},
            {ControllerState::ReachedTarget, "ReachedTarget"},
        };
        return names.value(s, "Unknown");
    }
};

#endif

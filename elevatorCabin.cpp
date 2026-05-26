#include "elevatorCabin.h"

ElevatorCabin::ElevatorCabin(int initialFloor, QObject* parent)
    : QObject(parent), floor(initialFloor) , direction(Direction::None), state(CabinState::Stopped)
{
    connect(&movementTimer, &QTimer::timeout, this, &ElevatorCabin::onMovementTick);
    movementTimer.setInterval(movementIntervalMs);
}

void ElevatorCabin::move(Direction moveDirection)
{
    if (state == CabinState::Blocked) {
        emit logMessage("Cabin: move request ignored — cabin is blocked");
        return;
    }
    setState(CabinState::PreparingToMove);
    direction = moveDirection;
    setState(CabinState::Moving);
    movementTimer.start();
}

void ElevatorCabin::stop()
{
    movementTimer.stop();
    direction = Direction::None;
    setState(CabinState::Stopped);
}

void ElevatorCabin::lock()
{
    movementTimer.stop();
    setState(CabinState::Blocked);
}

void ElevatorCabin::unlock()
{
    setState(CabinState::Stopped);
}

void ElevatorCabin::stopAllTimers()
{
    movementTimer.stop();
}

void ElevatorCabin::onMovementTick()
{
    int step = (direction == Direction::Up) ? 1 : -1;
    int nextFloor = floor + step;
    bool withinBounds = (nextFloor >= minFloor && nextFloor <= maxFloor);

    if (!withinBounds) {
        emit logMessage("Cabin: safety boundary reached, stopping");
        stop();
        return;
    }

    floor = nextFloor;
    emit floorChanged(floor);
}

void ElevatorCabin::setState(CabinState newState)
{
    if (state == newState) return;

    QString from = ElevatorEnums::cabinStateName(state);
    QString to = ElevatorEnums::cabinStateName(newState);
    state = newState;
    emit cabinStateChanged(state);
    emit logMessage(QString("Cabin: %1 → %2").arg(from, to));
}

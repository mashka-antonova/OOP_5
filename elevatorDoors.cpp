#include "elevatorDoors.h"

ElevatorDoors::ElevatorDoors(QObject* parent): QObject(parent), doorState(DoorState::Closed)
{
    openingTimer.setSingleShot(true);
    waitTimer.setSingleShot(true);
    closingTimer.setSingleShot(true);

    connect(&openingTimer, &QTimer::timeout, this, &ElevatorDoors::onOpeningFinished);
    connect(&waitTimer, &QTimer::timeout, this, &ElevatorDoors::onWaitFinished);
    connect(&closingTimer, &QTimer::timeout, this, &ElevatorDoors::onClosingFinished);
}

void ElevatorDoors::open()
{
    if (doorState == DoorState::Opened || doorState == DoorState::Opening) return;

    closingTimer.stop();
    waitTimer.stop();
    setState(DoorState::Opening);
    openingTimer.start(doorOpeningTimeMs);
}

void ElevatorDoors::close()
{
    if (doorState == DoorState::Closed || doorState == DoorState::Closing) return;

    openingTimer.stop();
    waitTimer.stop();
    setState(DoorState::Closing);
    closingTimer.start(doorClosingTimeMs);
}

void ElevatorDoors::stopAllTimers()
{
    openingTimer.stop();
    waitTimer.stop();
    closingTimer.stop();
}

void ElevatorDoors::forceClose()
{
    stopAllTimers();
    setState(DoorState::Closed);
}

void ElevatorDoors::onOpeningFinished()
{
    setState(DoorState::Opened);
    emit doorsOpened();
    waitTimer.start(doorOpenWaitMs);
}

void ElevatorDoors::onWaitFinished()
{
    setState(DoorState::Closing);
    closingTimer.start(doorClosingTimeMs);
}

void ElevatorDoors::onClosingFinished()
{
    setState(DoorState::Closed);
    emit doorsClosed();
}

void ElevatorDoors::setState(DoorState newState)
{
    if (doorState == newState) return;

    QString from = ElevatorEnums::doorStateName(doorState);
    QString to = ElevatorEnums::doorStateName(newState);
    doorState = newState;
    emit doorStateChanged(doorState);
    emit logMessage(QString("Doors: %1 → %2").arg(from, to));
}

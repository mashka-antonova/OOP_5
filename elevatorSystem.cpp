#include "elevatorSystem.h"
#include "elevatorCabin.h"
#include "elevatorDoors.h"
#include "elevatorController.h"

ElevatorSystem::ElevatorSystem(int initialFloor, QObject* parent)
    : QObject(parent), cabin(initialFloor, this), doors(this), controller(&cabin, &doors, &scheduler, this)
{
    connectSignals();
}

void ElevatorSystem::connectStateSignals()
{
    connect(&cabin, &ElevatorCabin::floorChanged, this, &ElevatorSystem::currentFloorChanged);
    connect(&cabin, &ElevatorCabin::cabinStateChanged, this, &ElevatorSystem::cabinStateChanged);
    connect(&doors, &ElevatorDoors::doorStateChanged, this, &ElevatorSystem::doorStateChanged);
    connect(&controller, &ElevatorController::targetFloorChanged, this, &ElevatorSystem::targetFloorChanged);
    connect(&controller, &ElevatorController::directionChanged, this, &ElevatorSystem::directionChanged);
    connect(&controller, &ElevatorController::controllerStateChanged, this, &ElevatorSystem::controllerStateChanged);
}

void ElevatorSystem::connectLogSignals()
{
    connect(&cabin, &ElevatorCabin::logMessage, this, &ElevatorSystem::logMessage);
    connect(&doors, &ElevatorDoors::logMessage, this, &ElevatorSystem::logMessage);
    connect(&controller, &ElevatorController::logMessage, this, &ElevatorSystem::logMessage);
}

void ElevatorSystem::connectCommandSignals()
{
    connect(this, &ElevatorSystem::callFromFloor, &controller, &ElevatorController::onFloorCallRequested);
    connect(this, &ElevatorSystem::selectFloorFromCabin, &controller, &ElevatorController::onCabinFloorSelected);
    connect(this, &ElevatorSystem::emergencyStop, &controller, &ElevatorController::onEmergencyStop);
    connect(this, &ElevatorSystem::unlockElevator, &controller, &ElevatorController::onUnlockRequested);
    connect(this, &ElevatorSystem::openDoors, &controller, &ElevatorController::onOpenDoorsRequested);
    connect(this, &ElevatorSystem::closeDoors, &controller, &ElevatorController::onCloseDoorsRequested);


    connect(&controller, &ElevatorController::moveCabinRequested, &cabin, &ElevatorCabin::move);
    connect(&controller, &ElevatorController::stopCabinRequested, &cabin, &ElevatorCabin::stop);
    connect(&controller, &ElevatorController::lockCabinRequested, &cabin, &ElevatorCabin::lock);
    connect(&controller, &ElevatorController::unlockCabinRequested, &cabin, &ElevatorCabin::unlock);
    connect(&controller, &ElevatorController::stopCabinTimersRequested, &cabin, &ElevatorCabin::stopAllTimers);


    connect(&controller, &ElevatorController::openDoorsRequested, &doors, &ElevatorDoors::open);
    connect(&controller, &ElevatorController::closeDoorsRequested, &doors, &ElevatorDoors::close);
    connect(&controller, &ElevatorController::forceCloseDoorsRequested, &doors, &ElevatorDoors::forceClose);
    connect(&controller, &ElevatorController::stopDoorTimersRequested, &doors, &ElevatorDoors::stopAllTimers);
}

void ElevatorSystem::connectSignals()
{
    connectStateSignals();
    connectLogSignals();
    connectCommandSignals();
}

void ElevatorSystem::alarm()
{
    emit logMessage("ALARM — emergency bell activated");
}

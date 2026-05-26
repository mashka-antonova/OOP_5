#include "elevatorController.h"
#include "elevatorCabin.h"
#include "elevatorDoors.h"
#include "elevatorScheduler.h"
#include <QMap>
#include <functional>

ElevatorController::ElevatorController(ElevatorCabin* cabin, ElevatorDoors* doors, ElevatorScheduler* scheduler,
                                       QObject* parent)
    : QObject(parent), cabin(cabin), doors(doors), scheduler(scheduler), currentFloor(cabin->currentFloor()),
    targetFloor(cabin->currentFloor()), travelDirection(Direction::None), controllerState(ControllerState::Idle),
    isBlocked(false)
{
    connect(cabin, &ElevatorCabin::floorChanged, this, &ElevatorController::onCabinFloorChanged);
    connect(doors, &ElevatorDoors::doorsOpened, this, &ElevatorController::onDoorsOpened);
    connect(doors, &ElevatorDoors::doorsClosed, this, &ElevatorController::onDoorsClosed);
}

void ElevatorController::onFloorCallRequested(int floor, Direction direction)
{
    if (isBlocked) {
        emit logMessage(QString("Floor call floor %1 ignored: system is blocked").arg(floor));
        return;
    }
    emit logMessage(QString("Floor call: floor %1, direction %2").arg(floor).arg(ElevatorEnums::directionName(direction)));
    addRequest({floor, RequestType::ExternalCall, direction});
}

void ElevatorController::onCabinFloorSelected(int floor)
{
    if (isBlocked) {
        emit logMessage(QString("Cabin floor %1 ignored: system is blocked").arg(floor));
        return;
    }
    emit logMessage(QString("Cabin command: floor %1").arg(floor));
    addRequest({floor, RequestType::CabinCommand, Direction::None});
}

void ElevatorController::onEmergencyStop()
{
    isBlocked = true;
    emit lockCabinRequested();
    emit stopDoorTimersRequested();
    scheduler->clearAll();
    updateDirection(Direction::None);
    updateControllerState(ControllerState::Idle);
    emit logMessage("!!! EMERGENCY STOP activated — cabin locked, queue cleared");
}

void ElevatorController::onUnlockRequested()
{
    isBlocked = false;
    emit forceCloseDoorsRequested();
    emit unlockCabinRequested();
    updateControllerState(ControllerState::Idle);
    updateDirection(Direction::None);
    emit logMessage("System unlocked and reset — ready to accept requests");
}

void ElevatorController::onOpenDoorsRequested()
{
    if (isBlocked) return;
    if (cabin->cabinState() != CabinState::Stopped) {
        emit logMessage("Cannot open doors: cabin is not stopped");
        return;
    }
    openDoorsIfClosed();
}

void ElevatorController::onCloseDoorsRequested()
{
    if (isBlocked) return;
    emit closeDoorsRequested();
}

void ElevatorController::addRequest(const ElevatorRequest& request)
{
    bool added = scheduler->addRequest(request);

    if (!added) {
        emit logMessage(QString("Duplicate request ignored: floor %1, direction %2").arg(request.floor)
                                .arg(ElevatorEnums::directionName(request.direction)));
        return;
    }

    if (controllerState != ControllerState::Idle) {
        if (controllerState == ControllerState::Moving)
            updateTargetIfNeeded();
        return;
    }

    if (request.floor == currentFloor) {
        if (cabin->cabinState() == CabinState::Stopped) {
            scheduler->removeServedRequestsAt(currentFloor, travelDirection);
            openDoorsIfClosed();
        }
        return;
    }

    const QMap<DoorState, std::function<void()>> doorActions = {
        {DoorState::Closed,  [this] { chooseNextTarget(); startMoving(); }},
        {DoorState::Opened,  [this] { emit closeDoorsRequested(); }},
        {DoorState::Opening, [this] { emit closeDoorsRequested(); }},
        {DoorState::Closing, [] {}}
    };

    if (doorActions.contains(doors->state()))
        doorActions.value(doors->state())();
}

void ElevatorController::chooseNextTarget()
{
    auto target = scheduler->nextTarget(currentFloor, travelDirection);
    if (!target.has_value()) {
        updateDirection(Direction::None);
        updateControllerState(ControllerState::Idle);
        return;
    }

    targetFloor = target.value();
    Direction newDirection = (targetFloor > currentFloor) ? Direction::Up
                                 : (targetFloor < currentFloor) ? Direction::Down
                                 : travelDirection;
    updateDirection(newDirection);
    emit targetFloorChanged(targetFloor);
    emit logMessage(QString("New target: floor %1").arg(targetFloor));
    updateControllerState(ControllerState::UpdatingTarget);
}

void ElevatorController::startMoving()
{
    if (!scheduler->hasRequests()) {
        updateControllerState(ControllerState::Idle);
        return;
    }
    updateControllerState(ControllerState::Moving);
    emit logMessage(QString("Moving %1").arg(ElevatorEnums::directionName(travelDirection)));
    emit moveCabinRequested(travelDirection);
}

void ElevatorController::stopAtFloor(int floor)
{
    emit logMessage(QString("Arrived at floor %1").arg(floor));
    updateControllerState(ControllerState::ReachedTarget);
    scheduler->removeServedRequestsAt(floor, travelDirection);
    emit openDoorsRequested();
}

void ElevatorController::onCabinFloorChanged(int floor)
{
    currentFloor = floor;
    emit logMessage(QString("Passing floor %1").arg(floor));

    if (controllerState != ControllerState::Moving) return;

    if (scheduler->shouldStopAt(floor, travelDirection) || floor == targetFloor) {
        emit stopCabinRequested();
        stopAtFloor(floor);
    }
}

void ElevatorController::onDoorsOpened()
{
    emit logMessage("Doors open — waiting for passengers");
}

void ElevatorController::onDoorsClosed()
{
    chooseNextTarget();
    if (controllerState != ControllerState::Idle)
        startMoving();
}

void ElevatorController::updateControllerState(ControllerState newState)
{
    if (controllerState == newState) return;

    QString from = ElevatorEnums::controllerStateName(controllerState);
    QString to = ElevatorEnums::controllerStateName(newState);
    controllerState = newState;
    emit controllerStateChanged(controllerState);
    emit logMessage(QString("Controller: %1 -> %2").arg(from, to));
}

void ElevatorController::updateDirection(Direction newDirection)
{
    if (travelDirection != newDirection) {
        travelDirection = newDirection;
        emit directionChanged(travelDirection);
    }
}

void ElevatorController::updateTargetIfNeeded()
{
    std::optional<int> newTarget = scheduler->nextTarget(currentFloor, travelDirection);
    if (newTarget.has_value() && newTarget.value() != targetFloor) {
        targetFloor = newTarget.value();
        emit targetFloorChanged(targetFloor);
        emit logMessage(QString("Target updated: floor %1").arg(targetFloor));
    }
}

void ElevatorController::openDoorsIfClosed()
{
    if (doors->state() == DoorState::Closed || doors->state() == DoorState::Closing)
        emit openDoorsRequested();
}

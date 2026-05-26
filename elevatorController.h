#ifndef ELEVATORCONTROLLER_H
#define ELEVATORCONTROLLER_H

#include <QObject>
#include "elevatorCabin.h"
#include "elevatorDoors.h"
#include "elevatorEnums.h"
#include "elevatorRequest.h"
#include "elevatorScheduler.h"

class ElevatorController : public QObject {
    Q_OBJECT
public:
    explicit ElevatorController(ElevatorCabin* cabin, ElevatorDoors* doors, ElevatorScheduler* scheduler,
                                QObject* parent = nullptr);

public slots:
    //вызываются в ElevatorSystem
    void onFloorCallRequested(int floor, Direction direction);
    void onCabinFloorSelected(int floor);
    void onEmergencyStop();
    void onUnlockRequested();
    void onOpenDoorsRequested();
    void onCloseDoorsRequested();

signals:
    //идут в GUI
    void targetFloorChanged(int floor);
    void directionChanged(Direction direction);
    void controllerStateChanged(ControllerState state);
    void logMessage(QString message);

    //идут в ElevatorCabin
    void moveCabinRequested(Direction direction);
    void stopCabinRequested();
    void lockCabinRequested();
    void unlockCabinRequested();
    void stopCabinTimersRequested();

    //идут в ElevatorDoors
    void openDoorsRequested();
    void closeDoorsRequested();
    void forceCloseDoorsRequested();
    void stopDoorTimersRequested();

private slots:
    //входящие события от кабины и дверей
    void onCabinFloorChanged(int floor);
    void onDoorsOpened();
    void onDoorsClosed();

private:
    void addRequest(const ElevatorRequest& request);
    void chooseNextTarget();
    void startMoving();
    void stopAtFloor(int floor);
    void updateControllerState(ControllerState newState);
    void updateDirection(Direction newDirection);
    void updateTargetIfNeeded();
    void openDoorsIfClosed();

    ElevatorCabin* cabin;
    ElevatorDoors* doors;
    ElevatorScheduler* scheduler;

    int currentFloor;
    int targetFloor;
    Direction travelDirection;
    ControllerState controllerState;
    bool isBlocked;
};
#endif // ELEVATORCONTROLLER_H

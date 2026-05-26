#ifndef ELEVATORSYSTEM_H
#define ELEVATORSYSTEM_H

#include <QObject>
#include "elevatorCabin.h"
#include "elevatorController.h"
#include "elevatorDoors.h"
#include "elevatorEnums.h"
#include "elevatorScheduler.h"

class ElevatorSystem : public QObject {
    Q_OBJECT
public:
    explicit ElevatorSystem(int initialFloor = 1, QObject* parent = nullptr);
    ~ElevatorSystem() = default;

public slots:
    void alarm();

signals:
    void callFromFloor(int floor, Direction direction);
    void selectFloorFromCabin(int floor);
    void emergencyStop();
    void unlockElevator();
    void openDoors();
    void closeDoors();

    void currentFloorChanged(int floor);
    void targetFloorChanged(int floor);
    void directionChanged(Direction direction);
    void controllerStateChanged(ControllerState state);
    void cabinStateChanged(CabinState state);
    void doorStateChanged(DoorState state);
    void logMessage(QString message);

private:
    void connectSignals();
    void connectStateSignals();
    void connectLogSignals();
    void connectCommandSignals();

    ElevatorScheduler scheduler;
    ElevatorCabin cabin;
    ElevatorDoors doors;
    ElevatorController controller;
};
#endif // ELEVATORSYSTEM_H

#ifndef ELEVATORCABIN_H
#define ELEVATORCABIN_H

#include <QObject>
#include <QTimer>
#include "elevatorEnums.h"

class ElevatorCabin : public QObject {
    Q_OBJECT
public:
    explicit ElevatorCabin(int initialFloor = 1, QObject* parent = nullptr);

    int currentFloor() const {return floor;};
    CabinState cabinState() const {return state;};
    Direction travelDirection() const {return direction;};

public slots:
    void move(Direction direction);
    void stop();
    void lock();
    void unlock();
    void stopAllTimers();

signals:
    void floorChanged(int floor);
    void cabinStateChanged(CabinState state);
    void logMessage(QString message);

private slots:
    void onMovementTick();

private:
    static constexpr int minFloor = 1;
    static constexpr int maxFloor = 5;
    static constexpr int movementIntervalMs = 1500;

    void setState(CabinState newState);

    int floor;
    Direction direction;
    CabinState state;
    QTimer movementTimer;
};
#endif // ELEVATORCABIN_H

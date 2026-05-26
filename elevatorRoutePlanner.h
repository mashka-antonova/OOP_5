#ifndef ELEVATORROUTEPLANNER_H
#define ELEVATORROUTEPLANNER_H
#include <optional>
#include <QList>
#include "elevatorRequest.h"

class ElevatorRoutePlanner {
public:
    std::optional<int> nextTarget(const QList<ElevatorRequest>& requests, int currentFloor, Direction currentDirection) const;

    bool shouldStopAt(const QList<ElevatorRequest>& requests, int floor, Direction direction) const;

private:
    QList<int> floorsAbove(const QList<ElevatorRequest>& requests, int currentFloor) const;
    QList<int> floorsBelow(const QList<ElevatorRequest>& requests, int currentFloor) const;
    
    int closestFloor(const QList<int>& floors, int currentFloor) const;

    std::optional<int> pickGoingUp(const QList<int>& above, const QList<int>& below) const;
    std::optional<int> pickGoingDown(const QList<int>& above, const QList<int>& below) const;
    std::optional<int> pickNearest(const QList<int>& above, const QList<int>& below, int currentFloor) const;
};
#endif // ELEVATORROUTEPLANNER_H

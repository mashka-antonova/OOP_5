#ifndef ELEVATORSCHEDULER_H
#define ELEVATORSCHEDULER_H
#include <optional>
#include <QList>
#include "elevatorRequest.h"
#include "elevatorRoutePlanner.h"


class ElevatorScheduler {
public:
    explicit ElevatorScheduler();

    bool addRequest(const ElevatorRequest& request);

    bool hasRequests() const;
    std::optional<int> nextTarget(int currentFloor, Direction currentDirection) const;
    bool shouldStopAt(int floor, Direction direction) const;

    void removeServedRequestsAt(int floor, Direction direction);

    void clearAll();
    QList<ElevatorRequest> getAllPendingRequests() const;

private:
    bool isDuplicate(const ElevatorRequest& request) const;

    QList<ElevatorRequest> pendingRequests;
    ElevatorRoutePlanner routePlanner;
};
#endif // ELEVATORSCHEDULER_H

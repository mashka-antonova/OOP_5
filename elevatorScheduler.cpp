#include "elevatorScheduler.h"

ElevatorScheduler::ElevatorScheduler() = default;

bool ElevatorScheduler::addRequest(const ElevatorRequest& request)
{
    bool isAdded = false;
    if (!isDuplicate(request)) {
        pendingRequests.append(request);
        isAdded = true;
    }
    return isAdded;
}

bool ElevatorScheduler::hasRequests() const
{
    return !pendingRequests.isEmpty();
}

std::optional<int> ElevatorScheduler::nextTarget(int currentFloor, Direction currentDirection) const
{
    return routePlanner.nextTarget(pendingRequests, currentFloor, currentDirection);
}

bool ElevatorScheduler::shouldStopAt(int floor, Direction direction) const
{
    return routePlanner.shouldStopAt(pendingRequests, floor, direction);
}

void ElevatorScheduler::removeServedRequestsAt(int floor, Direction direction)
{
    auto isServedRequest = [floor, direction](const ElevatorRequest& request) {
        return request.floor == floor &&
               (request.type == RequestType::CabinCommand ||
                direction == Direction::None ||
                request.direction == direction ||
                request.direction == Direction::None);
    };

    pendingRequests.removeIf(isServedRequest);
}

void ElevatorScheduler::clearAll()
{
    pendingRequests.clear();
}

QList<ElevatorRequest> ElevatorScheduler::getAllPendingRequests() const
{
    return pendingRequests;
}

bool ElevatorScheduler::isDuplicate(const ElevatorRequest& request) const
{
    return pendingRequests.contains(request);
}

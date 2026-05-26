#include "elevatorRoutePlanner.h"

#include <algorithm>
#include <functional>
#include <QMap>

std::optional<int> ElevatorRoutePlanner::nextTarget(const QList<ElevatorRequest>& requests, int currentFloor,
                                                    Direction currentDirection) const
{
    std::optional<int> target = std::nullopt;

    if (!requests.isEmpty()) {
        QList<int> above = floorsAbove(requests, currentFloor);
        QList<int> below = floorsBelow(requests, currentFloor);

        const QMap<Direction, std::function<std::optional<int>()>> pickers = {
            {Direction::Up, [&] { return pickGoingUp(above, below); }},
            {Direction::Down, [&] { return pickGoingDown(above, below); }},
            {Direction::None, [&] { return pickNearest(above, below, currentFloor); }}
        };

        target = pickers.value(currentDirection)();
    }

    return target;
}

bool ElevatorRoutePlanner::shouldStopAt(const QList<ElevatorRequest>& requests, int floor, Direction direction) const
{
    return std::any_of(requests.begin(), requests.end(),
        [floor, direction](const ElevatorRequest& req) {
            bool matchesFloor = (req.floor == floor);
            bool isCabinCmd = (req.type == RequestType::CabinCommand);
            bool isCompatible = (req.type == RequestType::ExternalCall && req.direction == direction);
            return matchesFloor && (isCabinCmd || isCompatible);
        });
}

QList<int> ElevatorRoutePlanner::floorsAbove(const QList<ElevatorRequest>& requests, int currentFloor) const
{
    QList<int> floors;
    for (const ElevatorRequest& req : requests) {
        if (req.floor > currentFloor && !floors.contains(req.floor))
            floors.append(req.floor);
    }
    std::sort(floors.begin(), floors.end());
    return floors;
}

QList<int> ElevatorRoutePlanner::floorsBelow(
    const QList<ElevatorRequest>& requests, int currentFloor) const
{
    QList<int> floors;
    for (const ElevatorRequest& req : requests)
        if (req.floor < currentFloor && !floors.contains(req.floor))
            floors.append(req.floor);
    std::sort(floors.begin(), floors.end(), std::greater<int>());
    return floors;
}

int ElevatorRoutePlanner::closestFloor(const QList<int>& floors, int currentFloor) const
{
    return *std::min_element(floors.begin(), floors.end(),
        [currentFloor](int a, int b) {
            return std::abs(a - currentFloor) < std::abs(b - currentFloor);
        });
}

std::optional<int> ElevatorRoutePlanner::pickGoingUp(const QList<int>& above, const QList<int>& below) const
{
    const QList<int>& preferred = above.isEmpty() ? below : above;
    return preferred.isEmpty() ? std::nullopt : std::make_optional(preferred.first());
}

std::optional<int> ElevatorRoutePlanner::pickGoingDown(const QList<int>& above, const QList<int>& below) const
{
    const QList<int>& preferred = below.isEmpty() ? above : below;
    return preferred.isEmpty() ? std::nullopt : std::make_optional(preferred.first());
}

std::optional<int> ElevatorRoutePlanner::pickNearest(const QList<int>& above, const QList<int>& below,
                                                     int currentFloor) const
{
    QList<int> all = above + below;
    return all.isEmpty() ? std::nullopt : std::make_optional(closestFloor(all, currentFloor));
}

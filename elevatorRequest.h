#ifndef ELEVATORREQUEST_H
#define ELEVATORREQUEST_H
#include "elevatorEnums.h"

struct ElevatorRequest {
    int floor;
    RequestType type;
    Direction direction;

    //нужен для использования contains
    bool operator==(const ElevatorRequest& other) const
    {
        return floor == other.floor && type == other.type && direction == other.direction;
    }
};
#endif // ELEVATORREQUEST_H

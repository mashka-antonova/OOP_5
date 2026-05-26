#ifndef ELEVATORVIEW_H
#define ELEVATORVIEW_H
#include <QWidget>
#include <QMap>
#include "elevatorEnums.h"

class FloorWidget;

class ElevatorView : public QWidget {
    Q_OBJECT
public:
    explicit ElevatorView(QWidget* parent = nullptr);

    void setCurrentFloor(int floor);
    void setDoorState(DoorState state);

signals:
    void floorCallRequested(int floor, Direction direction);

private:
    void setupUi();
    void connectFloorButtons(FloorWidget* widget, int floor, bool showUp, bool showDown);

    static constexpr int minFloor = 1;
    static constexpr int maxFloor = 5;
    static constexpr int initialFloor = 1;

    QMap<int, FloorWidget*> floorWidgetMap;
    int currentFloor;
};

#endif // ELEVATORVIEW_H

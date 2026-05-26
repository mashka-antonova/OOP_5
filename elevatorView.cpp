#include "elevatorView.h"
#include "floorWidget.h"

#include <QVBoxLayout>
#include <QMap>

ElevatorView::ElevatorView(QWidget* parent): QWidget(parent), currentFloor(initialFloor)
{
    setupUi();
}

void ElevatorView::setupUi()
{
    static const QMap<int, QString> floorImagePaths = {
        {1, ":/images/floor1.png"},
        {2, ":/images/floor2.png"},
        {3, ":/images/floor3.png"},
        {4, ":/images/floor4.png"},
        {5, ":/images/floor5.png"}
    };

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    for (int floor = maxFloor; floor >= minFloor; floor--) {
        bool showUp = (floor < maxFloor);
        bool showDown = (floor > minFloor);

        QPixmap pixmap(floorImagePaths.value(floor));
        FloorWidget* widget = new FloorWidget(floor, pixmap, showUp, showDown, this);
        floorWidgetMap.insert(floor, widget);
        layout->addWidget(widget);

        connectFloorButtons(widget, floor, showUp, showDown);
    }

    floorWidgetMap.value(currentFloor)->setElevatorPresent(true);
}

void ElevatorView::connectFloorButtons(FloorWidget* widget, int floor, bool showUp, bool showDown)
{
    if (showUp && widget->upButton()) {
        connect(widget->upButton(), &QPushButton::clicked, this, [this, floor] {
            emit floorCallRequested(floor, Direction::Up);
        });
    }
    if (showDown && widget->downButton()) {
        connect(widget->downButton(), &QPushButton::clicked, this, [this, floor] {
            emit floorCallRequested(floor, Direction::Down);
        });
    }
}

void ElevatorView::setCurrentFloor(int floor)
{
    if (floorWidgetMap.contains(currentFloor))
        floorWidgetMap.value(currentFloor)->setElevatorPresent(false);
    currentFloor = floor;
    if (floorWidgetMap.contains(currentFloor))
        floorWidgetMap.value(currentFloor)->setElevatorPresent(true);
}

void ElevatorView::setDoorState(DoorState state)
{
    if (floorWidgetMap.contains(currentFloor))
        floorWidgetMap.value(currentFloor)->setDoorState(state);
}

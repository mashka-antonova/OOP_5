#ifndef FLOORWIDGET_H
#define FLOORWIDGET_H
#include <QFrame>
#include <QPixmap>
#include <QLabel>
#include <QPushButton>
#include "elevatorEnums.h"

class FloorWidget : public QFrame {
    Q_OBJECT
public:
    static constexpr int widgetHeight = 120;

    FloorWidget(int floorNumber, const QPixmap& background, bool showUp, bool showDown, QWidget* parent = nullptr);

    int  floorNum() const {return floorNumber;};
    bool hasElevator() const {return isElevatorPresent;}
    QPushButton* upButton() const {return upCallButton;};
    QPushButton* downButton() const {return downCallButton;};

    void setElevatorPresent(bool present);
    void setDoorState(DoorState state);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    static constexpr int shaftCellWidth = 60;
    static constexpr int callButtonSize = 36;
    static constexpr int bgRed = 20;
    static constexpr int bgGreen = 16;
    static constexpr int bgBlue = 10;

    static constexpr int overlayAlphaActive = 80;
    static constexpr int overlayAlphaDark = 145;

    void setupLayout(bool showUp, bool showDown);
    QLabel* createShaftCell();
    QLayout* createCallButtons(bool showUp, bool showDown);
    void updateElevatorVisual();

    int floorNumber;
    QPixmap backgroundPixmap;
    bool isElevatorPresent;
    DoorState currentDoorState;

    QLabel* shaftCell;
    QPushButton* upCallButton;
    QPushButton* downCallButton;
};
#endif // FLOORWIDGET_H

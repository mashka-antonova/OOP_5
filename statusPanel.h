#ifndef STATUSPANEL_H
#define STATUSPANEL_H
#include <QWidget>
#include "elevatorEnums.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StatusPanel; }
QT_END_NAMESPACE

class StatusPanel : public QWidget {
    Q_OBJECT
public:
    explicit StatusPanel(QWidget* parent = nullptr);
    ~StatusPanel() override;

public slots:
    void onCurrentFloorChanged(int floor);
    void onTargetFloorChanged(int floor);
    void onDirectionChanged(Direction direction);
    void onControllerStateChanged(ControllerState state);
    void onCabinStateChanged(CabinState state);
    void onDoorStateChanged(DoorState state);

private:
    Ui::StatusPanel* ui;
};
#endif // STATUSPANEL_H

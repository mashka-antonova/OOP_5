#include "statusPanel.h"
#include "ui_statusPanel.h"
#include "elevatorEnums.h"

StatusPanel::StatusPanel(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::StatusPanel)
{
    ui->setupUi(this);
}

StatusPanel::~StatusPanel()
{
    delete ui;
}

void StatusPanel::onCurrentFloorChanged(int floor)
{
    ui->currentFloorValue->setText(QString::number(floor));
}

void StatusPanel::onTargetFloorChanged(int floor)
{
    ui->targetValue->setText(QString::number(floor));
}

void StatusPanel::onDirectionChanged(Direction direction)
{
    ui->directionValue->setText(ElevatorEnums::directionName(direction));
}

void StatusPanel::onControllerStateChanged(ControllerState state)
{
    ui->controllerValue->setText(ElevatorEnums::controllerStateName(state));
}

void StatusPanel::onCabinStateChanged(CabinState state)
{
    ui->cabinValue->setText(ElevatorEnums::cabinStateName(state));
}

void StatusPanel::onDoorStateChanged(DoorState state)
{
    ui->doorsValue->setText(ElevatorEnums::doorStateName(state));
}

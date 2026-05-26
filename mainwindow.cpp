#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "elevatorView.h"
#include "cabinPanel.h"
#include "statusPanel.h"
#include "logPanel.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), elevatorSystem(new ElevatorSystem(initialFloor, this))
{
    ui->setupUi(this);
    connectSignals();
    setMinimumSize(minWindowWidth, minWindowHeight);
    resize(defaultWindowWidth, defaultWindowHeight);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSignals()
{
    connectViewToSystem();
    connectSystemToStatusPanel();
    connectSystemToView();
    connectSystemToLog();
}

void MainWindow::connectViewToSystem()
{
    connect(ui->elevatorView, &ElevatorView::floorCallRequested,
            elevatorSystem,   &ElevatorSystem::callFromFloor);

    connect(ui->cabinPanel, &CabinPanel::floorButtonClicked,
            elevatorSystem, &ElevatorSystem::selectFloorFromCabin);
    connect(ui->cabinPanel, &CabinPanel::openDoorsClicked,
            elevatorSystem, &ElevatorSystem::openDoors);
    connect(ui->cabinPanel, &CabinPanel::closeDoorsClicked,
            elevatorSystem, &ElevatorSystem::closeDoors);
    connect(ui->cabinPanel, &CabinPanel::emergencyStopClicked,
            elevatorSystem, &ElevatorSystem::emergencyStop);
    connect(ui->cabinPanel, &CabinPanel::unlockClicked,
            elevatorSystem, &ElevatorSystem::unlockElevator);
    connect(ui->cabinPanel, &CabinPanel::alarmClicked,
            elevatorSystem, &ElevatorSystem::alarm);
}

void MainWindow::connectSystemToStatusPanel()
{
    connect(elevatorSystem, &ElevatorSystem::currentFloorChanged,
            ui->statusPanel, &StatusPanel::onCurrentFloorChanged);
    connect(elevatorSystem, &ElevatorSystem::targetFloorChanged,
            ui->statusPanel, &StatusPanel::onTargetFloorChanged);
    connect(elevatorSystem, &ElevatorSystem::directionChanged,
            ui->statusPanel, &StatusPanel::onDirectionChanged);
    connect(elevatorSystem, &ElevatorSystem::controllerStateChanged,
            ui->statusPanel, &StatusPanel::onControllerStateChanged);
    connect(elevatorSystem, &ElevatorSystem::cabinStateChanged,
            ui->statusPanel, &StatusPanel::onCabinStateChanged);
    connect(elevatorSystem, &ElevatorSystem::doorStateChanged,
            ui->statusPanel, &StatusPanel::onDoorStateChanged);
}

void MainWindow::connectSystemToView()
{
    connect(elevatorSystem, &ElevatorSystem::currentFloorChanged,
            ui->elevatorView, &ElevatorView::setCurrentFloor);
    connect(elevatorSystem, &ElevatorSystem::doorStateChanged,
            ui->elevatorView, &ElevatorView::setDoorState);
    connect(elevatorSystem, &ElevatorSystem::currentFloorChanged,
            ui->cabinPanel, &CabinPanel::setActiveFloor);
}

void MainWindow::connectSystemToLog()
{
    connect(elevatorSystem, &ElevatorSystem::logMessage,
            ui->logPanel, &LogPanel::appendLogMessage);
}

#include "cabinPanel.h"
#include "ui_cabinPanel.h"
#include <QStyle>

CabinPanel::CabinPanel(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::CabinPanel)
{
    ui->setupUi(this);

    floorButtonMap = {
        {5, ui->floorButton_5},
        {4, ui->floorButton_4},
        {3, ui->floorButton_3},
        {2, ui->floorButton_2},
        {1, ui->floorButton_1}
    };

    connectSignals();
}

CabinPanel::~CabinPanel()
{
    delete ui;
}

void CabinPanel::connectSignals()
{
    for (auto it = floorButtonMap.constBegin(); it != floorButtonMap.constEnd(); ++it) {
        const int floor = it.key();
        connect(it.value(), &QPushButton::clicked, this, [this, floor] {
            emit floorButtonClicked(floor);
        });
    }

    connect(ui->openDoorsButton, &QPushButton::clicked, this, &CabinPanel::openDoorsClicked);
    connect(ui->closeDoorsButton, &QPushButton::clicked, this, &CabinPanel::closeDoorsClicked);
    connect(ui->emergencyStopButton, &QPushButton::clicked, this, &CabinPanel::emergencyStopClicked);
    connect(ui->unlockButton, &QPushButton::clicked, this, &CabinPanel::unlockClicked);
    connect(ui->alarmButton, &QPushButton::clicked, this, &CabinPanel::alarmClicked);
}

void CabinPanel::setActiveFloor(int floor)
{
    for (auto it = floorButtonMap.constBegin(); it != floorButtonMap.constEnd(); ++it) {
        QPushButton* btn = it.value();
        bool isActive = (it.key() == floor);
        btn->setProperty("active", isActive);
        btn->style()->unpolish(btn);
        btn->style()->polish(btn);
    }
}

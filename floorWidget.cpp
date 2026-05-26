#include "floorWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QMap>
#include <QStyle>

FloorWidget::FloorWidget(int floorNumber, const QPixmap& background, bool showUp, bool showDown, QWidget* parent)
    : QFrame(parent), floorNumber(floorNumber), backgroundPixmap(background), isElevatorPresent(false), currentDoorState(DoorState::Closed)
{
    setFixedHeight(widgetHeight);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setupLayout(showUp, showDown);
}

void FloorWidget::setElevatorPresent(bool present)
{
    isElevatorPresent = present;
    if (!present)
        currentDoorState = DoorState::Closed;
    updateElevatorVisual();
    update();
}

void FloorWidget::setDoorState(DoorState state)
{
    currentDoorState = state;
    if (isElevatorPresent)
        updateElevatorVisual();
}

QLabel* FloorWidget::createShaftCell()
{
    QLabel* cell = new QLabel(this);
    cell->setObjectName("shaftCell");
    cell->setFixedWidth(shaftCellWidth);
    cell->setAlignment(Qt::AlignCenter);
    cell->setText("  │  ");
    return cell;
}

QLayout* FloorWidget::createCallButtons(bool showUp, bool showDown)
{
    QVBoxLayout* col = new QVBoxLayout();
    col->setSpacing(5);
    col->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    upCallButton = nullptr;
    downCallButton = nullptr;

    if (showUp) {
        upCallButton = new QPushButton("▲", this);
        upCallButton->setObjectName("callButton");
        upCallButton->setFixedSize(callButtonSize, callButtonSize);
        col->addWidget(upCallButton);
    }
    if (showDown) {
        downCallButton = new QPushButton("▼", this);
        downCallButton->setObjectName("callButton");
        downCallButton->setFixedSize(callButtonSize, callButtonSize);
        col->addWidget(downCallButton);
    }
    if (!showUp && !showDown)
        col->addSpacing(callButtonSize);

    return col;
}

void FloorWidget::setupLayout(bool showUp, bool showDown)
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(8, 4, 8, 4);
    mainLayout->setSpacing(8);

    shaftCell = createShaftCell();
    mainLayout->addWidget(shaftCell);
    mainLayout->addLayout(createCallButtons(showUp, showDown));
}

void FloorWidget::updateElevatorVisual()
{
    static const QMap<DoorState, QString> doorSymbols = {
        {DoorState::Closed,  "  ▐▌  "},
        {DoorState::Opening, " ▐  ▌ "},
        {DoorState::Opened,  "▐    ▌"},
        {DoorState::Closing, " ▐  ▌ "}
    };

    if (isElevatorPresent) {
        shaftCell->setText(doorSymbols.value(currentDoorState, "▐▌"));
        shaftCell->setProperty("active", true);
    } else {
        shaftCell->setText("  │  ");
        shaftCell->setProperty("active", false);
    }
    shaftCell->style()->unpolish(shaftCell);
    shaftCell->style()->polish(shaftCell);
}

void FloorWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    if (!backgroundPixmap.isNull()) {
        const QPixmap scaled = backgroundPixmap.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        int xOff = (scaled.width() - width()) / 2;
        painter.drawPixmap(-xOff, 0, scaled);
    } else
        painter.fillRect(rect(), QColor(bgRed, bgGreen, bgBlue));

    int alpha = isElevatorPresent ? overlayAlphaActive : overlayAlphaDark;
    painter.fillRect(rect(), QColor(0, 0, 0, alpha));

    QFrame::paintEvent(event);
}

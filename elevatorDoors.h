#ifndef ELEVATORDOORS_H
#define ELEVATORDOORS_H

#include <QObject>
#include <QTimer>
#include "elevatorEnums.h"


class ElevatorDoors : public QObject {
    Q_OBJECT
public:
    explicit ElevatorDoors(QObject* parent = nullptr);

    DoorState state() const {return doorState;};

public slots:
    void open();
    void close();
    void stopAllTimers();
    void forceClose();

signals:
    void doorsOpened();
    void doorsClosed();
    void doorStateChanged(DoorState state);
    void logMessage(QString message);

private slots:
    void onOpeningFinished();
    void onWaitFinished();
    void onClosingFinished();

private:
    static constexpr int doorOpeningTimeMs = 800;
    static constexpr int doorOpenWaitMs = 2500;
    static constexpr int doorClosingTimeMs = 800;

    void setState(DoorState newState);

    DoorState doorState;
    QTimer openingTimer;
    QTimer waitTimer;
    QTimer closingTimer;
};
#endif // ELEVATORDOORS_H

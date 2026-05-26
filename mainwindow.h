#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "elevatorSystem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private:
    void connectSignals();
    void connectViewToSystem();
    void connectSystemToStatusPanel();
    void connectSystemToView();
    void connectSystemToLog();

    static constexpr int minWindowWidth = 1100;
    static constexpr int minWindowHeight = 700;
    static constexpr int defaultWindowWidth = 1280;
    static constexpr int defaultWindowHeight = 780;

    static constexpr int initialFloor = 1;

    Ui::MainWindow* ui;
    ElevatorSystem* elevatorSystem;
};
#endif // MAINWINDOW_H

#ifndef CABINPANEL_H
#define CABINPANEL_H
#include <QWidget>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class CabinPanel; }
QT_END_NAMESPACE

class QPushButton;

class CabinPanel : public QWidget {
    Q_OBJECT
public:
    static constexpr int floorCount = 5;
    static constexpr int minFloor = 1;

    explicit CabinPanel(QWidget* parent = nullptr);
    ~CabinPanel() override;

    void setActiveFloor(int floor);

signals:
    void floorButtonClicked(int floor);
    void openDoorsClicked();
    void closeDoorsClicked();
    void emergencyStopClicked();
    void unlockClicked();
    void alarmClicked();

private:
    void connectSignals();

    Ui::CabinPanel* ui;
    QMap<int, QPushButton*> floorButtonMap;
};
#endif // CABINPANEL_H

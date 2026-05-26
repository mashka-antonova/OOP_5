#ifndef LOGPANEL_H
#define LOGPANEL_H
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class LogPanel; }
QT_END_NAMESPACE

class LogPanel : public QWidget {
    Q_OBJECT
public:
    explicit LogPanel(QWidget* parent = nullptr);
    ~LogPanel() override;

public slots:
    void appendLogMessage(const QString& message);
    void clearLog();

private:
    static constexpr int logMaxLines = 500;

    Ui::LogPanel* ui;
};
#endif // LOGPANEL_H

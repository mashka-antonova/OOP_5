#include "logPanel.h"
#include "ui_logPanel.h"

#include <QDateTime>
#include <QScrollBar>
#include <QDebug>

LogPanel::LogPanel(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LogPanel)
{
    ui->setupUi(this);
    ui->logOutput->setObjectName("logOutput");
    ui->logOutput->document()->setMaximumBlockCount(logMaxLines);

    connect(ui->clearButton, &QPushButton::clicked, this, &LogPanel::clearLog);
}

LogPanel::~LogPanel()
{
    delete ui;
}

void LogPanel::appendLogMessage(const QString& message)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString line = QString("<span style='color:#556677;font-family:Courier New'>[%1]</span>"
                                 " <span style='color:#b8aa72'>%2</span>").arg(time, message.toHtmlEscaped());

    ui->logOutput->append(line);
    ui->logOutput->verticalScrollBar()->setValue(ui->logOutput->verticalScrollBar()->maximum());
}

void LogPanel::clearLog()
{
    ui->logOutput->clear();
}

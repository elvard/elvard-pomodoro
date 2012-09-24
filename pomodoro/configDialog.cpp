#include <QtGui>

#include "configDialog.h"

ConfigDialog::ConfigDialog(const QString &title, QWidget *parent)
    : QDialog(parent)
{
    pomodoroIntervalEdit = new QTimeEdit(QTime(0, 25));
    pomodoroIntervalEdit->setDisplayFormat("mm:ss");
    pomodoroBreakEdit = new QTimeEdit(QTime(0, 5));
    pomodoroBreakEdit->setDisplayFormat("mm:ss");
    pomodoroLongBreakEdit = new QTimeEdit(QTime(0, 15));
    pomodoroLongBreakEdit->setDisplayFormat("mm:ss");
    pomodoroIntervalLabel = new QLabel(tr("Pomodoro interval:"));
    pomodoroBreakLabel = new QLabel(tr("Break interval:"));
    pomodoroLongBreakLabel = new QLabel(tr("Long break interval:"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                  | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(pomodoroIntervalLabel);
    mainLayout->addWidget(pomodoroIntervalEdit);
    mainLayout->addWidget(pomodoroBreakLabel);
    mainLayout->addWidget(pomodoroBreakEdit);
    mainLayout->addWidget(pomodoroLongBreakLabel);
    mainLayout->addWidget(pomodoroLongBreakEdit);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(title);
}

QTime ConfigDialog::getPomodoroInterval()
{
    return pomodoroIntervalEdit->time();
}

QTime ConfigDialog::getPomodoroBreak()
{
    return pomodoroIntervalEdit->time();
}

QTime ConfigDialog::getPomodoroLongBreak()
{
    return pomodoroIntervalEdit->time();
}

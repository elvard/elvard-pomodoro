#ifndef TIMER_H
#define TIMER_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QAction;
class QTimeEdit;
class QLabel;
class QWidget;
QT_END_NAMESPACE

class Timer
{

public:
    ConfigDialog(const QString &title, QWidget *parent);

    QTime getPomodoroInterval();
    QTime getPomodoroBreak();
    QTime getPomodoroLongBreak();

private:
    QAction *quitAction;
    QTimeEdit *pomodoroIntervalEdit;
    QTimeEdit *pomodoroBreakEdit;
    QTimeEdit *pomodoroLongBreakEdit;

    QLabel *pomodoroIntervalLabel;
    QLabel *pomodoroBreakLabel;
    QLabel *pomodoroLongBreakLabel;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif

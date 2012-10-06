#ifndef STATUS_H
#define STATUS_H

#include <QSystemTrayIcon>
#include <QDialog>
#include <QDBusInterface>

#include "log.h"

QT_BEGIN_NAMESPACE
class QAction;
class QLCDNumber;
class QLabel;
class QProgressBar;
class QGroupBox;
class QStackedWidget;
class QDBusInterface;
QT_END_NAMESPACE

class Status: public QDialog
{
    Q_OBJECT

    static const int RUN = 2;
    static const int PAUSE = 1;
    static const int STOP = 0;

public:
    Status();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void pomodoroConfirm();
    void openConfigurationDialog();
    void openLogDialog();
    void run();
    void pause();
    void stop();
    void timerUpdate();

private:
    void createLog();
    void createActions();
    void createTrayIcon();
    void createStatusArea();
    void createDefaultIntervals();
    void createTimer();
    void createDBusConnection();
    void setPomodoroInterval(QTime time);
    void setPomodoroBreak(QTime time);
    void setPomodoroLongBreak(QTime time);
    void setKopeteStatus(bool breakTime);
    void setIcon();
    void updateStatusArea();

    void timerTick();

    void setStageTitle(QString title);
    void stageFinished();
    void cycleReset();
    void cycleStep();

    void showPomodoroTime();

    QAction *quitAction;
    QAction *configureAction;
    QAction *logAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QLCDNumber *activeTime;
    QProgressBar *activeProgress;
    QLabel *stageLabel;
    QPushButton *pomodoroRun;
    QPushButton *pomodoroPause;
    QPushButton *pomodoroStop;
    QStackedWidget *statusArea;
    QGroupBox *statusTime;
    QGroupBox *statusPaused;
    QGroupBox *statusStopped;

    QTime *pomodoroInterval;
    QTime *pomodoroBreak;
    QTime *pomodoroLongBreak;

    QTimer *timer;
    QTime  *pomodoroTime;

    QDBusInterface *kopeteApp;

    Log *log;

    int pomodoroActive;
    int elapsed;

    QString cycle;
    int cycleIndex;
};

#endif

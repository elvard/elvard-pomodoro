#ifndef STATUS_H
#define STATUS_H

#include <QSystemTrayIcon>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QAction;
class QLCDNumber;
class QLabel;
class QProgressBar;
class QGroupBox;
class QStackedWidget;
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
    void run();
    void pause();
    void stop();
    void timerUpdate();

private:
    void createActions();
    void createTrayIcon();
    void createStatusArea();
    void createDefaultIntervals();
    void createTimer();
    void setPomodoroInterval(QTime time);
    void setPomodoroBreak(QTime time);
    void setPomodoroLongBreak(QTime time);
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
    QTime *pomodoroTime;

    int pomodoroActive;
    int elapsed;

    QString cycle;
    int cycleIndex;
};

#endif

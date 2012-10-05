#include <QDebug>
#include <QtGui>

#include "status.h"
#include "configDialog.h"

Status::Status()
{
    createLog();
    createActions();
    createTrayIcon();
    createDefaultIntervals();
    createStatusArea();
    createTimer();
    stop();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusArea);
    setLayout(mainLayout);

    trayIcon->show();

    setWindowTitle(tr("Systray"));

    cycle = QString("ibibibil");
}

void Status::run()
{
    if (pomodoroActive == STOP) {
        cycleReset();
        showPomodoroTime();
    }
    pomodoroActive = RUN;
    updateStatusArea();
    timerTick();
}

void Status::pause()
{
    pomodoroActive = PAUSE;
    updateStatusArea();
    timer->stop();
}

void Status::stop()
{
    pomodoroActive = STOP;
    cycleReset();
    updateStatusArea();
    timer->stop();
}

void Status::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

void Status::openConfigurationDialog()
{
    ConfigDialog config(tr("test"), this);

    if (config.exec() == QDialog::Accepted) {
        setPomodoroInterval(config.getPomodoroInterval());
        setPomodoroBreak(config.getPomodoroBreak());
        setPomodoroLongBreak(config.getPomodoroLongBreak());
    }
}

void Status::openLogDialog()
{
    log->show();
}

void Status::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        if (isVisible()) hide(); else show();
        break;
    default:
        ;
    }
}

void Status::pomodoroConfirm()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Pomodoro has just finnished.\n"
                      "Have you been distracted?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = msgBox.exec();

    switch(ret) {
    case QMessageBox::Yes:
        log->write(0, "");
        break;
    case QMessageBox::No:
        log->write(1, "");
        break;
    default:
        break;
    }
}

void Status::createLog()
{
    try {
        log = new Log(this);
    } catch (char * msg) {
        QCoreApplication::exit(EXIT_FAILURE);
    }

    log->hide();
}

void Status::createActions()
{
    configureAction = new QAction(tr("&Configure"), this);
    connect(configureAction, SIGNAL(triggered()), this, SLOT(openConfigurationDialog()));

    logAction = new QAction(tr("&Log"), this);
    connect(logAction, SIGNAL(triggered()), this, SLOT(openLogDialog()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Status::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(configureAction);
    trayIconMenu->addAction(logAction);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));

    setIcon();
}

void Status::createStatusArea()
{
    statusTime = new QGroupBox();
    activeTime = new QLCDNumber();
    activeTime->setSegmentStyle(QLCDNumber::Flat);
    showPomodoroTime();
    QPushButton *pauseButton = new QPushButton(tr("Pause"));
    QHBoxLayout *statusActiveLayout = new QHBoxLayout();
    statusActiveLayout->addWidget(activeTime);
    statusActiveLayout->addWidget(pauseButton);
    QGroupBox *statusGroupBox = new QGroupBox();
    statusGroupBox->setLayout(statusActiveLayout);
    stageLabel = new QLabel();
    stageLabel->setAlignment(Qt::AlignHCenter);
    QVBoxLayout *statusVerticalLayout = new QVBoxLayout();
    statusVerticalLayout->addWidget(stageLabel);
    statusVerticalLayout->addWidget(statusGroupBox);
    statusTime->setLayout(statusVerticalLayout);

    statusPaused = new QGroupBox();
    QPushButton *continueButton = new QPushButton(tr("Run"));
    QPushButton *stopButton = new QPushButton(tr("Stop"));
    QHBoxLayout *statusPausedLayout = new QHBoxLayout();
    statusPausedLayout->addWidget(stopButton);
    statusPausedLayout->addWidget(continueButton);
    statusPaused->setLayout(statusPausedLayout);

    statusStopped = new QGroupBox();
    QHBoxLayout *statusStoppedLayout = new QHBoxLayout();
    QPushButton *runButton = new QPushButton(tr("Run"));
    statusStoppedLayout->addWidget(runButton);
    statusStopped->setLayout(statusStoppedLayout);

    connect(runButton, SIGNAL(clicked()), this, SLOT(run()));
    connect(continueButton, SIGNAL(clicked()), this, SLOT(run()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stop()));
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(pause()));

    statusArea = new QStackedWidget();
    statusArea->addWidget(statusStopped);
    statusArea->addWidget(statusPaused);
    statusArea->addWidget(statusTime);
}

void Status::createDefaultIntervals()
{
    pomodoroInterval = new QTime(0, 25);
    pomodoroBreak = new QTime(0, 5);
    pomodoroLongBreak = new QTime(0, 15);
}

void Status::createTimer()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
}

void Status::setIcon()
{
    QIcon icon = QIcon(":/images/appointment-soon.png");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);
}

void Status::setPomodoroInterval(QTime time) {
    pomodoroInterval = &time;
}

void Status::setPomodoroBreak(QTime time) {
    pomodoroBreak = &time;
}

void Status::setPomodoroLongBreak(QTime time) {
    pomodoroLongBreak = &time;
}

void Status::showPomodoroTime() {
    if (pomodoroTime != NULL) {
        QTime time = pomodoroTime->addSecs(elapsed);
        activeTime->display(time.toString("mm:ss"));
    }
}

void Status::updateStatusArea()
{
    statusArea->setCurrentIndex(pomodoroActive);
}

void Status::timerTick()
{
    timer->start(1000);
}

void Status::timerUpdate()
{
    elapsed--;
    if (elapsed <= pomodoroTime->secsTo(QTime())) {
        stageFinished();
    }
    showPomodoroTime();
}

void Status::stageFinished()
{
    elapsed = 0;
    cycleIndex++;
    if (cycleIndex >= cycle.length()) {
        cycleIndex = 0;
    }
    cycleStep();
}

void Status::cycleReset()
{
    elapsed = 0;
    cycleIndex = 0;
    cycleStep();
}

void Status::cycleStep()
{
    bool have_break = 0;
    if (cycle.at(cycleIndex) == QChar('i')) {
        pomodoroTime = pomodoroInterval;
        setStageTitle(tr("Pomodoro"));
    } else if (cycle.at(cycleIndex) == QChar('b')) {
        pomodoroTime = pomodoroBreak;
        setStageTitle(tr("Short break"));
        have_break = 1;
    } else if (cycle.at(cycleIndex) == QChar('l')) {
        pomodoroTime = pomodoroLongBreak;
        setStageTitle(tr("Long break"));
        have_break = 1;
    }

    if (have_break)
    {
        show();
        pause();
        pomodoroConfirm();
        run();
    }
}

void Status::setStageTitle(QString title) {
    stageLabel->setText(title);
}

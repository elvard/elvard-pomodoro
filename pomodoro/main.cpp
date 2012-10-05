#include <QtGui>

#include "connection.h"
#include "status.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(pomodoro);

    QApplication app(argc, argv);
    if (!createConnection())
        return 1;

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    Status status;
    return app.exec();
}

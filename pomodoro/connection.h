#ifndef CONNECTION_H
#define CONNECTION_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("pomodoro.db");
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    QSqlQuery query;
    if (!query.exec(
        "CREATE TABLE IF NOT EXISTS pomodoro_log ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "date INTEGER DEFAULT CURRENT_TIMESTAMP,"
        "success BOOL, note TEXT)")) {
        QMessageBox::critical(0, qApp->tr("Cannot create table"),
            query.lastError().text());
    }

    return true;
}

#endif

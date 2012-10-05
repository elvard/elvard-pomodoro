#ifndef LOG_H
#define LOG_H

#include <QDialog>
#include <QtSql>

QT_BEGIN_NAMESPACE
class QSqlTableModel;
class QDialogButtonBox;
QT_END_NAMESPACE

class Log : public QDialog
{
    Q_OBJECT

public:
    Log(QWidget *parent = 0);

    void write(bool success, QString note);

private slots:
    void submit();

private:
    QPushButton *submitButton;
    QPushButton *revertButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QSqlTableModel *model;
    QString *tableName;
};

#endif

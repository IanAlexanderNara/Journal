#ifndef JOURNAL_H
#define JOURNAL_H

#include <QWidget>
#include <QCoreApplication>
#include "journalsqlmodel.h"
#include <QShortcut>
#include <QDateTime>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QSqlQueryModel>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/kdf.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <QMenu>
#include <QAction>
#include <QModelIndexList>
#include <QAbstractItemView>
#include "journalencrypter.h"
#include <QInputDialog>
#include "mouseclickfilter.h"
#include <QString>
#include <QSettings>
#include "datepicker.h"
#include <QFile>
#include <QTimer>
namespace Ui {
class Journal;
}

class Journal : public QWidget
{
    Q_OBJECT

public:
    explicit Journal(QWidget *parent = nullptr);
    ~Journal();

    static const QString BIRTH_DEATH_GROUP;
    static const QString BIRTH_KEY;
    static const QString DEATH_KEY;
    static const QString DATE_FORMAT;

    static QDateTime calcDeathDate(QDateTime birth, QWidget *parent);

    void initBirthDeath();

signals:
    void sendStatusbarMessage(QString message);

private slots:
    void buttonSaveClicked();
    void buttonLoadClicked();
    void showPasswordChanged(bool checked);
    void showContextMenuDates(const QPoint &pos);
    void treeViewSelectionChanged();
    void treeViewDoubleClicked(const QModelIndex &index);

private:
    const QString SETTINGS_PATH = QCoreApplication::applicationDirPath() + "/settings/settings.INI";
    const QString DB_PATH = QCoreApplication::applicationDirPath() + "/databases/journal.db";
    const QString CONNECTION_NAME = "JOURNAL";
    const QString MODEL_QUERY = "SELECT date FROM journal";




    void updateTimer();
    QString millisecondsToTime(long long int milliseconds);

    QShortcut *saveShortcut;

    Ui::Journal *ui;
    QSqlDatabase db;
    JournalSqlModel *model;

    QDateTime birthDate;
    QDateTime deathDate;
    QTimer *timer;
    bool formatAsSeconds = false;

};

#endif // JOURNAL_H

#ifndef NOTECARDS_H
#define NOTECARDS_H

#include <QWidget>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include "notecardssqlmodel.h"
#include "notecardsdelegate.h"
#include "filterorrows.h"
#include <QShortcut>
#include <QFile>
#include <QDate>
#include <QSet>
#include <QStringList>
#include "dailynotecardsdialog.h"
namespace Ui {
class Notecards;
}

class Notecards : public QWidget
{
    Q_OBJECT

public:
    explicit Notecards(QWidget *parent = nullptr);
    ~Notecards();

signals:
    void sendStatusbarMessage(QString message);

private slots:
    void buttonAddClicked();
    void buttonSaveClicked();
    void lineEditSearchTextChanged();
    void showContextMenuNotecards(const QPoint &pos);

    void on_pushButton_clicked();

private:
    const int CARDS_PER_DAY = 3;
    const int DELETE_CARDS_AFTER = 30;
    const QString DB_PATH = QCoreApplication::applicationDirPath() + "/databases/notecards.db";
    const QString CONNECTION_NAME = "NOTECARDS";
    const QString CONTEXT_MENU_CLEAR = "Clear Row Fields";
    const QString CONTEXT_MENU_DELETE = "Delete Row";

    QShortcut *saveShortcut;
    QShortcut *searchShortcut;
    QShortcut *tableShortcut;

    Ui::Notecards *ui;
    QSqlDatabase db ;
    NotecardsSqlModel *model;
    FilterOrRows *filterModel;
    NotecardsDelegate *delegate;


};

#endif // NOTECARDS_H

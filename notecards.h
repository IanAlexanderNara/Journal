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

private:
    const QString DB_PATH = QCoreApplication::applicationDirPath() + "/databases/notecards.db";
    const QString CONNECTION_NAME = "NOTECARDS";
    const QString CONTEXT_MENU_CLEAR = "Clear Row Fields";
    const QString CONTEXT_MENU_DELETE = "Delete Row";

    QShortcut *saveShortcut;

    Ui::Notecards *ui;
    QSqlDatabase db ;
    NotecardsSqlModel *model;
    FilterOrRows *filterModel;
    NotecardsDelegate *delegate;
};

#endif // NOTECARDS_H

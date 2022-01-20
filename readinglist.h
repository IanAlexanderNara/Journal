#ifndef READINGLIST_H
#define READINGLIST_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include "readinglistsqlmodel.h"
#include "readinglistdelegate.h"
#include "filterorrows.h"
#include <QShortcut>
namespace Ui {
class ReadingList;
}

class ReadingList : public QWidget
{
    Q_OBJECT

public:
    explicit ReadingList(QWidget *parent = nullptr);
    ~ReadingList();

signals:
    void sendStatusbarMessage(QString message);

private slots:
    void buttonAddClicked();
    void buttonSaveClicked();
    void lineEditSearchTextChanged();
    void showContextMenuNotecards(const QPoint &pos);

private:
    const QString DB_PATH = QCoreApplication::applicationDirPath() + "/databases/reading_list.db";
    const QString CONNECTION_NAME = "READING_LIST";
    const QString CONTEXT_MENU_CLEAR = "Clear Row Fields";
    const QString CONTEXT_MENU_DELETE = "Delete Row";

    Ui::ReadingList *ui;
    QSqlDatabase db;
    ReadingListSqlModel *model;
    ReadingListDelegate *delegate;
    FilterOrRows *filterModel;
    QShortcut *saveShortcut;
};

#endif // READINGLIST_H

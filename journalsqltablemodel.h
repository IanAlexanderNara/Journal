#ifndef JOURNALSQLTABLEMODEL_H
#define JOURNALSQLTABLEMODEL_H

#include <QSqlTableModel>
#include <QSqlDatabase>
class JournalSqlTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit JournalSqlTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase::database());

signals:

};

#endif // JOURNALSQLTABLEMODEL_H

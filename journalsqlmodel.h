#ifndef JOURNALSQLTABLEMODEL_H
#define JOURNALSQLTABLEMODEL_H

#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QDate>
class JournalSqlModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit JournalSqlModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase::database());
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // JOURNALSQLTABLEMODEL_H

#ifndef READINGLISTSQLMODEL_H
#define READINGLISTSQLMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include <QSqlDatabase>
class ReadingListSqlModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit ReadingListSqlModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase::database());
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // READINGLISTSQLMODEL_H

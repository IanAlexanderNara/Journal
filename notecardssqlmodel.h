#ifndef NOTECARDSSQLMODEL_H
#define NOTECARDSSQLMODEL_H

#include <QSqlTableModel>
#include <QSqlDatabase>
class NotecardsSqlModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit NotecardsSqlModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase::database());
    QVariant data(const QModelIndex &index, int role) const;

};

#endif // NOTECARDSSQLMODEL_H

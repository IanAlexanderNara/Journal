#include "journalsqlmodel.h"

JournalSqlModel::JournalSqlModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel{parent, db}
{

}

QVariant JournalSqlModel::data(const QModelIndex &index, int role) const
{
    if(index.column() == 0){
        QVariant value = QSqlTableModel::data(index, role);
        if(value.isValid() && role == Qt::DisplayRole){
            QDate date = QDate::fromJulianDay(value.toUInt());
            return date.toString(QString("MM/dd/yyyy"));
        }
        return value;
    } else {
        return QSqlTableModel::data(index, role);
    }
}

bool JournalSqlModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    // do not allow editing of data. Instead we perform direct sql insertion into database for simplicity
    return false;
}

Qt::ItemFlags JournalSqlModel::flags(const QModelIndex &index) const
{
    return QSqlTableModel::flags(index) & ~Qt::ItemIsEditable;
}

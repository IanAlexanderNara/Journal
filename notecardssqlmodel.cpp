#include "notecardssqlmodel.h"
#include <QSqlDatabase>
#include <QColor>



NotecardsSqlModel::NotecardsSqlModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel{parent, db}
{

}

QVariant NotecardsSqlModel::data(const QModelIndex &index, int role) const
{
    bool emptyVal = false;
    int row = index.row();
    if(QSqlTableModel::data(this->index(row, 1)).toString().isEmpty()) emptyVal = true;
    if(QSqlTableModel::data(this->index(row, 2)).toString().isEmpty()) emptyVal = true;
    if(QSqlTableModel::data(this->index(row, 3)).toString().isEmpty()) emptyVal = true;
    if(role == Qt::BackgroundColorRole){
        if(emptyVal) return QColor(Qt::darkRed);
    } else if (role == Qt::TextColorRole){
        if(emptyVal) return QColor(Qt::white);
    }
    return QSqlTableModel::data(index, role);
}


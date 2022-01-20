#include "readinglistsqlmodel.h"
#include <QColor>
#include "readinglistdelegate.h"
ReadingListSqlModel::ReadingListSqlModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel{parent, db}
{

}

QVariant ReadingListSqlModel::data(const QModelIndex &index, int role) const
{
    if(index.column() == ReadingListDelegate::statusColumn && role == Qt::TextColorRole){
        QString data = index.data().toString();
        if(data == ReadingListDelegate::READY){
            return QColor(Qt::darkYellow);
        } else if(data == ReadingListDelegate::READING){
            return QColor(Qt::darkRed);
        } else if (data == ReadingListDelegate::FINISHED){
            return QColor(Qt::darkGreen);
        } else {
            return QSqlTableModel::data(index, role);
        }
    }
    return QSqlTableModel::data(index, role);
}

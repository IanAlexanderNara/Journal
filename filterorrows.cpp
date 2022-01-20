#include "filterorrows.h"

FilterOrRows::FilterOrRows(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

bool FilterOrRows::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    // return true if any column contains the regexp
    bool found = false;
    for(int i = 0; i < sourceModel()->columnCount(source_parent); i++){
        QModelIndex index = sourceModel()->index(source_row, i, source_parent);
        if(sourceModel()->data(index).toString().contains(filterRegExp())) found = true;
    }
    return found;
}

#ifndef FILTERORROWS_H
#define FILTERORROWS_H

#include <QSortFilterProxyModel>

class FilterOrRows : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit FilterOrRows(QObject *parent = nullptr);

signals:


    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

#endif // FILTERORROWS_H

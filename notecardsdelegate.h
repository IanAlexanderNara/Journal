#ifndef NOTECARDSDELEGATE_H
#define NOTECARDSDELEGATE_H

#include <QStyledItemDelegate>
#include <QTimer>
#include "notecardinputdialog.h"

class NotecardsDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit NotecardsDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // NOTECARDSDELEGATE_H

#include "notecardsdelegate.h"

NotecardsDelegate::NotecardsDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

QWidget *NotecardsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 0){ // Disable editing of the notecard id
        return nullptr;
    } else {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void NotecardsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(editor){
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void NotecardsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(editor){
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void NotecardsDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(editor){
        QStyledItemDelegate::updateEditorGeometry(editor, option, index);
    }
}

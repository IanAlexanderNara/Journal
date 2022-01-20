#include "readinglistdelegate.h"
#include "readinglistsqlmodel.h"

const QString ReadingListDelegate::READY = "Ready to Start";
const QString ReadingListDelegate::READING = "Reading";
const QString ReadingListDelegate::FINISHED = "Fininshed";

ReadingListDelegate::ReadingListDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

QWidget *ReadingListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == statusColumn){
        QComboBox *editor = new QComboBox(parent);
        editor->addItems(statusList);
        editor->setItemData(0, QColor(Qt::darkYellow), Qt::TextColorRole);
        editor->setItemData(1, QColor(Qt::darkRed), Qt::TextColorRole);
        editor->setItemData(2, QColor(Qt::darkGreen), Qt::TextColorRole);
        return editor;
    } else {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void ReadingListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == statusColumn){
        QComboBox *combo = static_cast<QComboBox *>(editor);
        int idx = statusList.indexOf(index.data(Qt::DisplayRole).toString());
        combo->setCurrentIndex(idx);
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void ReadingListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == statusColumn){
        QComboBox *combo = static_cast<QComboBox *>(editor);
        model->setData(index, combo->currentText());
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void ReadingListDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

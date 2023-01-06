#include "notecardsdelegate.h"


NotecardsDelegate::NotecardsDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

QWidget *NotecardsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 0){ // Disable editing of the notecard id
        return nullptr;
    }
    else if(index.column() == 4){
        NotecardInputDialog *inputDialog = new NotecardInputDialog(parent);
        QString num = index.model()->data(index.model()->index(index.row(), 0)).toString();
        QString author = index.model()->data(index.model()->index(index.row(), 1)).toString();
        QString title = index.model()->data(index.model()->index(index.row(), 2)).toString();
        QString themes = index.model()->data(index.model()->index(index.row(), 3)).toString();
        if(!author.isEmpty() && !title.isEmpty() && !themes.isEmpty()){
            inputDialog->setWindowTitle(num + " - " + author + ", " + title + " - " + themes);
        }
        else{
            inputDialog->setWindowTitle(" ");
        }
        inputDialog->setModal(true);
        return inputDialog;
    }
    else {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void NotecardsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(editor && index.column() != 4){
        QStyledItemDelegate::setEditorData(editor, index);
    }
    else if(editor) {
        NotecardInputDialog* inputDialog = dynamic_cast<NotecardInputDialog*>(editor);
        inputDialog->resize(500, 375);
        QString data = index.model()->data(index).toString();
        inputDialog->setText(data);
        QTimer::singleShot(200, inputDialog, [=] () {
            inputDialog->setFocus();
        });
    }
}

void NotecardsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(editor && index.column() != 4){
        QStyledItemDelegate::setModelData(editor, model, index);
    }
    else if(editor){
        NotecardInputDialog* inputDialog = dynamic_cast<NotecardInputDialog*>(editor);
        QString data = inputDialog->getText();
        model->setData(index, QVariant(data));
    }
}

void NotecardsDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(editor){
        QStyledItemDelegate::updateEditorGeometry(editor, option, index);
    }
}


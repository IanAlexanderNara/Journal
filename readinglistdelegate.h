#ifndef READINGLISTDELEGATE_H
#define READINGLISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QComboBox>
class ReadingListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ReadingListDelegate(QObject *parent = nullptr);    

    static const QString READY;
    static const QString READING;
    static const QString FINISHED;
    static const int rowidColumn = 0;
    static const int titleColumn = 1;
    static const int authorColumn = 2;
    static const int statusColumn = 3;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;


private:
    QStringList statusList = {READY, READING, FINISHED};
};

#endif // READINGLISTDELEGATE_H

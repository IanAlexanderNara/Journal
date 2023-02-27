#ifndef DAILYNOTECARDSDIALOG_H
#define DAILYNOTECARDSDIALOG_H

#include <QDialog>
#include <QDebug>
namespace Ui {
class DailyNotecardsDialog;
}

class DailyNotecardsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DailyNotecardsDialog(QWidget *parent = nullptr, QList<int> daysIds = QList<int>(), QStringList daysAuthors = QStringList(), QStringList daysTitles = QStringList(), QStringList text = QStringList());
    ~DailyNotecardsDialog();

private slots:
    void on_nextButton_clicked();

    void on_previousButton_clicked();

private:
    Ui::DailyNotecardsDialog *ui;
    int index = 0;
    QList<int> daysIds;
    QStringList daysAuthors;
    QStringList daysTitles;
    QStringList daysCards;
    QString makeWindowTitle();
};

#endif // DAILYNOTECARDSDIALOG_H

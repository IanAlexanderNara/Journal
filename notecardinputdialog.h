#ifndef NOTECARDINPUTDIALOG_H
#define NOTECARDINPUTDIALOG_H

#include <QDialog>
#include <QFocusEvent>
#include <QDebug>

namespace Ui {
class NotecardInputDialog;
}

class NotecardInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NotecardInputDialog(QWidget *parent = nullptr);
    ~NotecardInputDialog();
    QString getText();
    void setText(QString text);
    void setFocus();

private slots:
    void on_pushButton_clicked();

private:
    Ui::NotecardInputDialog *ui;
};

#endif // NOTECARDINPUTDIALOG_H

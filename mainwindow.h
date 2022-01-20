#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "journal.h"
#include "notecards.h"
#include "readinglist.h"
#include <QApplication>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void recieveStatusbarMessage(QString message);
    void actionSettingsTriggered();

private:
    const QString SETTINGS_PATH = QCoreApplication::applicationDirPath() + "/settings/settings.INI";

    const int MESSAGE_DURATION = 5000;
    Ui::MainWindow *ui;
    Journal *journal;
    ReadingList *readingList;
    Notecards *notecards;
};
#endif // MAINWINDOW_H

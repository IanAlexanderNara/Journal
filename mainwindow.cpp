#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QDebug>
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    // create the databases folder if it doesn't exist
    QDir dirDb(QCoreApplication::applicationDirPath() + "/databases");
    if (!dirDb.exists())
        dirDb.mkpath(".");

    // create the settings folder if it doesn't exist
    QDir dirSettings(QCoreApplication::applicationDirPath() + "/settings");
    if (!dirSettings.exists())
        dirSettings.mkpath(".");


    ui->setupUi(this);

    this->setWindowTitle("Journal");

    // Setup the menu
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::actionSettingsTriggered);

    journal = new Journal(this);
    ui->tabWidget->addTab(journal, QString("Journal"));
    connect(journal, &Journal::sendStatusbarMessage, this, &MainWindow::recieveStatusbarMessage);

    readingList = new ReadingList(this);
    ui->tabWidget->addTab(readingList, QString("Reading List"));
    connect(readingList, &ReadingList::sendStatusbarMessage, this, &MainWindow::recieveStatusbarMessage);

    notecards = new Notecards(this);
    ui->tabWidget->addTab(notecards, QString("Notecards"));
    connect(notecards, &Notecards::sendStatusbarMessage, this, &MainWindow::recieveStatusbarMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recieveStatusbarMessage(QString message)
{
    ui->statusbar->showMessage(message, MESSAGE_DURATION);
}

void MainWindow::actionSettingsTriggered()
{
    SettingsDialog *settingsDialog = new SettingsDialog(this);
    settingsDialog->setWindowTitle("Settings");
    if(settingsDialog->exec() == QDialog::Accepted){
        journal->initBirthDeath();
    }
}



#include "notecards.h"
#include "ui_notecards.h"
#include <QSqlDatabase>
#include <QCoreApplication>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "notecardssqlmodel.h"
#include <QSortFilterProxyModel>
#include <QAction>
#include <QMenu>
#include "mouseclickfilter.h"
#include <QScrollBar>
Notecards::Notecards(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Notecards)
{
    ui->setupUi(this);

    // Setup up our signals and slots
    connect(ui->addButton, &QPushButton::clicked, this, &Notecards::buttonAddClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &Notecards::buttonSaveClicked);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &Notecards::lineEditSearchTextChanged);

    // Create our notecard database if it does not exist
    db = QSqlDatabase::addDatabase("QSQLITE", CONNECTION_NAME);
    db.setDatabaseName(DB_PATH);
    if(!db.open()){
        qDebug() << db.lastError();
        QMessageBox::warning(this, "Fatal Error", "Could not establish connection to notecard database.");
        exit(1);
    }
    QSqlQuery query(db);
    QString sqlString(QString("CREATE TABLE IF NOT EXISTS notecards ") +
                      QString("(number INTERGER PRIMARY KEY, author TEXT NOT NULL, title TEXT NOT NULL, themes TEXT NOT NULL, text TEXT);"));
    query.exec(sqlString);
    sqlString = QString(QString("CREATE TABLE IF NOT EXISTS daily ") +
                      QString("(number INTERGER PRIMARY KEY, date INTEGER NOT NULL);"));
    query.exec(sqlString);

    // clean daily records
    int threshold = QDate::currentDate().toJulianDay() - DELETE_CARDS_AFTER;
    QSqlQuery loadQuery(db);
    loadQuery.prepare("SELECT * FROM daily WHERE date <= ?");
    loadQuery.addBindValue(threshold);
    loadQuery.exec();
    QSqlQuery deleteQuery(db);
    while(loadQuery.next()){
        deleteQuery.prepare(QString("DELETE FROM daily WHERE number = ?"));
        deleteQuery.addBindValue(loadQuery.value(0));
        deleteQuery.exec();
    }

    // Set up our table view model
    model = new NotecardsSqlModel(this, db);
    delegate = new NotecardsDelegate(this);
    filterModel = new FilterOrRows(this);
    model->setTable("notecards");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("Number"));
    model->setHeaderData(1, Qt::Horizontal, tr("Author"));
    model->setHeaderData(2, Qt::Horizontal, tr("Title"));
    model->setHeaderData(3, Qt::Horizontal, tr("Themes"));
    model->setHeaderData(4, Qt::Horizontal, tr("Text"));
    filterModel->setSourceModel(model);
    filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->tableView->setModel(filterModel);
    ui->tableView->setItemDelegate(delegate);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->sortByColumn(0, Qt::AscendingOrder);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    // Setup context menu for table view
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &Notecards::showContextMenuNotecards);

    // Setup keyboard shortcut
    saveShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
    connect(saveShortcut, &QShortcut::activated, this, &Notecards::buttonSaveClicked);
    searchShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), this);
    connect(searchShortcut, &QShortcut::activated, this, [=] () {
        ui->lineEdit->setFocus();
    });
    tableShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this);
    connect(tableShortcut, &QShortcut::activated, this, [=] () {
        ui->tableView->setFocus();
        ui->tableView->setCurrentIndex(ui->tableView->model()->index(0, 1));
    });

}

Notecards::~Notecards()
{
    delete ui;
}

void Notecards::buttonAddClicked()
{
    // insert a new row at the bottom of the table and auto populate number and empty strings

    // determine number of records
    while(model->canFetchMore()) model->fetchMore();
    int numRecords = model->rowCount();

    model->insertRow(numRecords, QModelIndex());
    model->setData(model->index(numRecords, 0), numRecords + 1, Qt::EditRole);
    model->setData(model->index(numRecords, 1), "", Qt::EditRole);
    model->setData(model->index(numRecords, 2), "", Qt::EditRole);
    model->setData(model->index(numRecords, 3), "", Qt::EditRole);
}

void Notecards::buttonSaveClicked()
{
    if(model->submitAll()){
        while(model->canFetchMore()) model->fetchMore();
        emit sendStatusbarMessage(QString("Table save successful."));
    } else {
        emit sendStatusbarMessage(QString("Table save failed."));
    }
}

void Notecards::lineEditSearchTextChanged()
{
    filterModel->setFilterRegExp(ui->lineEdit->text());
}

void Notecards::showContextMenuNotecards(const QPoint &pos)
{
    QModelIndex index = ui->tableView->indexAt(pos);

    // Return if user didn't click directly on an tiem
    if(index.row() == -1)
        return;

    // Get global position for popping up menu
    QPoint globalPos = ui->tableView->mapToGlobal(pos);

    // Determine if last row or another row
    int number = index.sibling(index.row(), 0).data().toInt();
    int numRows = model->rowCount();

    //Construct menu and actions
    QMenu menu;
    menu.installEventFilter(new MouseClickFilter(this));
    menu.addAction(CONTEXT_MENU_CLEAR);
    // only allow deletion of the highest number notecard
    // (to preserve consistency/ordering with phsyical cards).
    if(number == numRows){
        menu.addAction(CONTEXT_MENU_DELETE);
    }
    QAction *action = menu.exec(globalPos);
    if(action && action->text() == CONTEXT_MENU_CLEAR){
        // use the number to find the REAL model index (in case sorted/filtered)
        for(int i = 1; i < model->columnCount(); i++){
            QModelIndex delIndex = model->index(number-1, i);
            model->setData(delIndex, "");
        }
    } else if(action && action->text() == CONTEXT_MENU_DELETE){
        QString message = "Delete row and save changes?";
        if(QMessageBox::Yes == QMessageBox(QMessageBox::Question, "Delete Row", message, QMessageBox::Yes|QMessageBox::No, this).exec()){
            model->removeRow(number-1);
            model->submitAll();
            while(model->canFetchMore()) model->fetchMore();
        }
    }
}


void Notecards::on_pushButton_clicked()
{

    QSet<int> currentIds = QSet<int>();
    QList<int> daysIds = QList<int>();
    QStringList daysCards = QStringList();
    QStringList daysTitles = QStringList();
    QStringList daysAuthors = QStringList();

    // load the ids of the recent notecards
    QSqlQuery query(db);
    query.exec(QString("SELECT number,date FROM daily;"));

    while (query.next()) {
        int id = query.value(0).toInt();
        int date = query.value(1).toInt();
        if(date == QDate::currentDate().toJulianDay()){
            daysIds.append(id);
        }
        currentIds.insert(id);
    }

    if(daysIds.empty()){
        // query for new cards
        query.exec(QString("SELECT * FROM notecards ORDER BY RANDOM();"));
        // check if recent


        while(daysCards.size() < CARDS_PER_DAY && query.next()){
            int potentialCardId = query.value(0).toInt();
            if(!currentIds.contains(potentialCardId)){
                daysCards.append(query.value(4).toString());
                daysAuthors.append(query.value(1).toString());
                daysTitles.append(query.value(2).toString());
                daysIds.append(potentialCardId);
            }
        }
        for(int i =0; i < daysIds.size(); i++){
            query.prepare("INSERT INTO daily (number, date) VALUES(:number, :date);");
            query.bindValue(":number", daysIds.at(i));
            query.bindValue(":date", QDate::currentDate().toJulianDay());
            query.exec();
        }
    } else {
        for(int i = 0; i < daysIds.size(); i++){
            query.prepare("SELECT * FROM notecards WHERE number = ?");
            query.addBindValue(daysIds.at(i));
            query.exec();
            query.next();
            daysCards.append(query.value(4).toString());
            daysAuthors.append(query.value(1).toString());
            daysTitles.append(query.value(2).toString());
        }
    }

    DailyNotecardsDialog *dailyNotecardsDialog= new DailyNotecardsDialog(this, daysIds, daysAuthors, daysTitles, daysCards);
    dailyNotecardsDialog->exec();
}


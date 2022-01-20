#include "readinglist.h"
#include "ui_readinglist.h"
#include "mouseclickfilter.h"
#include <QMenu>
ReadingList::ReadingList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReadingList)
{
    ui->setupUi(this);

    // Setup basic ui signals and slots
    connect(ui->addButton, &QPushButton::clicked, this, &ReadingList::buttonAddClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &ReadingList::buttonSaveClicked);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &ReadingList::lineEditSearchTextChanged);

    // Create our reading list database if it does not exist
    db = QSqlDatabase::addDatabase("QSQLITE", CONNECTION_NAME);
    db.setDatabaseName(DB_PATH);
    if(!db.open()){
        qDebug() << db.lastError();
        QMessageBox::warning(this, "Fatal Error", "Could not establish connection to reading list database.");
        exit(1);
    }
    QSqlQuery query(db);
    QString sqlString(QString("CREATE TABLE IF NOT EXISTS reading_list ") +
                      QString("(rowid INTEGER PRIMARY KEY NOT NULL, title TEXT NOT NULL, author TEXT NOT NULL, status TEXT NOT NULL);"));
    query.exec(sqlString);

    model = new ReadingListSqlModel(this, db);
    delegate = new ReadingListDelegate(this);
    filterModel = new FilterOrRows(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("reading_list");
    model->select();
    model->setHeaderData(ReadingListDelegate::titleColumn, Qt::Horizontal, tr("Title"));
    model->setHeaderData(ReadingListDelegate::authorColumn, Qt::Horizontal, tr("Author"));
    model->setHeaderData(ReadingListDelegate::statusColumn, Qt::Horizontal, tr("Status"));
    filterModel->setSourceModel(model);
    filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->tableView->setModel(filterModel);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setItemDelegateForColumn(ReadingListDelegate::statusColumn, delegate);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(ReadingListDelegate::statusColumn, Qt::DescendingOrder);
    ui->tableView->hideColumn(ReadingListDelegate::rowidColumn);

    // Setup context menu for table view
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &ReadingList::showContextMenuNotecards);

    // Setup shortcut
    saveShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
    connect(saveShortcut, &QShortcut::activated, this, &ReadingList::buttonSaveClicked);


}

ReadingList::~ReadingList()
{
    delete ui;
}

void ReadingList::buttonAddClicked()
{
    while(model->canFetchMore()) model->fetchMore();
    int numRecords = model->rowCount();
    model->insertRow(numRecords);
    for(int i = 0; i < model->columnCount(); i++){
        if(i != ReadingListDelegate::statusColumn && i != ReadingListDelegate::rowidColumn){
            model->setData(model->index(numRecords, i), "", Qt::EditRole);
        }
    }
    model->setData(model->index(numRecords, ReadingListDelegate::statusColumn), "Ready to Start", Qt::EditRole);
}

void ReadingList::buttonSaveClicked()
{
    if(model->submitAll()){
        emit sendStatusbarMessage(QString("Table save successful."));
    } else {
        emit sendStatusbarMessage(QString("Table save failed."));
    }
}

void ReadingList::lineEditSearchTextChanged()
{
    filterModel->setFilterRegExp(ui->lineEdit->text());
}

void ReadingList::showContextMenuNotecards(const QPoint &pos)
{
    QModelIndex temp= ui->tableView->indexAt(pos);
    QModelIndex index = filterModel->mapToSource(filterModel->index(temp.row(), temp.column()));

    // Return if user didn't click directly on an tiem
    if(index.row() == -1)
        return;

    // Get global position for popping up menu
    QPoint globalPos = ui->tableView->mapToGlobal(pos);

    //Construct menu and actions
    QMenu menu;
    menu.installEventFilter(new MouseClickFilter(this));
    menu.addAction(CONTEXT_MENU_CLEAR);
    menu.addAction(CONTEXT_MENU_DELETE);

    QAction *action = menu.exec(globalPos);
    if(action && action->text() == CONTEXT_MENU_CLEAR){
        for(int i = 0; i < model->columnCount(); i++){
            if(i != ReadingListDelegate::statusColumn && i != ReadingListDelegate::rowidColumn){
                QModelIndex delIndex = index.siblingAtColumn(i);
                model->setData(delIndex, "");
            }
        }
    } else if(action && action->text() == CONTEXT_MENU_DELETE){
        QString message = "Delete row and save changes?";
        if(QMessageBox::Yes == QMessageBox(QMessageBox::Question, "Delete Row", message, QMessageBox::Yes|QMessageBox::No, this).exec()){
            model->removeRow(index.row());
            model->submitAll();
        }
    }
}


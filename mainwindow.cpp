#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),io_service_(),server_(55555,io_service_),
    runThread_(boost::bind(&boost::asio::io_service::run, &io_service_))
{
    ui->setupUi(this);

    //events model
    this->eventsModel = new QStandardItemModel(0,4,this);
    QStringList lables;
    lables << "Location" << "Camera ID" << "Time" << "Priority";
    this->eventsModel->setHorizontalHeaderLabels(lables);

    //proxy
    this->eventProxyFilter = new QSortFilterProxyModel(this);
    this->eventProxyFilter->setSourceModel(this->eventsModel);
    this->eventProxyFilter->setFilterKeyColumn(3); //filter priority

    //table
    ui->tableView->setModel(this->eventProxyFilter);


    //id list
    this->clientIdModel = new QStringListModel(this);
    ui->listView->setModel(this->clientIdModel);


    //priority combobox
    QStringList comboBoxStrings;
    comboBoxStrings << "0" << "1" << "2" << "3";
    ui->comboBox->insertItems(0,comboBoxStrings);

    //connections
    //add types to meta type
    qRegisterMetaType<QVector<QString> >("QVector<QString>");
    qRegisterMetaType<u_int32_t>("u_int32_t");
    //new Client
    connect(&server_,SIGNAL(newConnection(u_int32_t)),this,SLOT(addClientConnection(u_int32_t)));
    //new event
    connect(&server_,SIGNAL(newEvent(QVector<QString>)),this , SLOT(newEventSlot(QVector<QString>)));

    //SQL
    setupDB();
    this->sqlModel = new QSqlQueryModel();
    ui->tableView_2->setModel(this->sqlModel);
}



MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}


void MainWindow::setupDB(){
    QString server = "";
    QString dbname = "";

    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("");
    db.setDatabaseName("");

    if(db.open())
    {
        qDebug() << "DB is open";
    }
    else{
        qDebug() << "Error:= " << db.lastError();
    }
}

void MainWindow::newEventSlot(QVector<QString> event){
    qDebug() << event;
    this->eventsModel->insertRow(0,QModelIndex());
    for(int i=0 ; i<event.size();i++)
    {
        QModelIndex modelindex = this->eventsModel->index(0,i,QModelIndex());
        this->eventsModel->setData(modelindex,event[i]);
    }
}

void MainWindow::addClientConnection(u_int32_t id){
    this->clientIdModel->insertRow(0,QModelIndex());
    QModelIndex index = this->clientIdModel->index(0,0,QModelIndex());
    this->clientIdModel->setData(index,id,0);
}


void MainWindow::removeCamera(u_int32_t id){
//    this->clientIdModel->removeRow(this->clientIdModel->match())
}

void MainWindow::openPictureView(std::string path){
    this->pPictureView = new PictureView(this);
    this->pPictureView->setPicture(path);
    this->show();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    QString priorityToFilter;
    switch (index) {
    case 0:
        priorityToFilter = "";
        break;
    case 1:
        priorityToFilter = "1";
        break;
    case 2:
        priorityToFilter = "2";
        break;
    case 3:
        priorityToFilter = "3";
        break;
    default:
        break;
    }
    this->eventProxyFilter->setFilterFixedString(priorityToFilter);

}


void MainWindow::on_pushButton_clicked()
{
    this->sqlModel->setQuery(ui->textEdit->toPlainText());
}

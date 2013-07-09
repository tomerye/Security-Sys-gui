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
    //delete client
    connect(&server_,SIGNAL(deleteConnectionSignal(u_int32_t)),this,SLOT(removeCamera(u_int32_t)));
    //new event
    connect(&server_,SIGNAL(newEvent(QVector<QString>)),this , SLOT(newEventSlot(QVector<QString>)));

    connect(&server_,SIGNAL(downloadProgressSignal(u_int32_t)),ui->progressBar,SLOT(setValue(int)));
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

//ID (DATE TIME) priority path
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
    QModelIndex index;
    QVariant data;
    for(int i=0;i<this->clientIdModel->rowCount();i++){
        index =  this->clientIdModel->index(i,0,QModelIndex());
        data = this->clientIdModel->data(index,Qt::DisplayRole);
        if(data==id)
        {
            qDebug() << "delete id " << id;
            this->clientIdModel->removeRow(i,QModelIndex());
        }
    }
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

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
//    QVariant clientID = this->clientIdModel->data(index,Qt::DisplayRole);
//    QSortFilterProxyModel clientIDFilter;
//    clientIDFilter.setSourceModel(this->eventsModel);
//    clientIDFilter.setFilterFixedString(clientID.toString());
//    clientIDFilter.setFilterKeyColumn(1);
//    QSortFilterProxyModel priorityFilter;
//    priorityFilter.setSourceModel(clientIDFilter);
//    priorityFilter.setFilterFixedString("1");
//    priorityFilter.setFilterKeyColumn(3);
//    int priority3 = priorityFilter.rowCount(QModelIndex());
//    priorityFilter.setFilterKeyColumn(2);
//    int priority2 = priorityFilter.rowCount(QModelIndex());
//    priorityFilter.setFilterKeyColumn(1);
//    int priority1 = priorityFilter.rowCount(QModelIndex());
//    ui->label->setText("level3: " + priority3 +" level2:" + priority2 + " level1:" + priority1);
}
void MainWindow::on_pushButton_2_clicked()
{
    this->pPictureView = new PictureView(this);
    this->pPictureView->setPicture("/home/tomer/tmp/tmp2/2");
    this->pPictureView->show();
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
//    QVariant data = index.data(QModelIndex);
    QStringList stringListData = (index.data()).toStringList();
//    this->server_.getFile(stringListData[1].toStdString(),stringListData[2].toStdString());
    this->server_.getFile(123,stringListData[2].toStdString());

}

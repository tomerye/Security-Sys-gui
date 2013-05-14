#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    ui->comboBox->
    connect(&server_,SIGNAL(newConnection(size_t)),this,SLOT(newClientConnection(size_t)));
    connect(&server_,SIGNAL(newEvent(QVector<QString>)),this , SLOT(newEventSlot(QVector<QString>)));
    qRegisterMetaType<QVector<QString> >();
    qRegisterMetaType<size_t>();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newEventSlot(QVector<QString> event){
    this->eventsModel->insertRow(0,QModelIndex());
    for(int i=0 ; i<event.size();i++)
    {
        QModelIndex modelindex = this->eventsModel->index(0,i,QModelIndex());
        this->eventsModel->setData(modelindex,event[i]);
    }
}

void MainWindow::newClientConnection(size_t id){
    this->clientIdModel->insertRow(0,QModelIndex());
    const QVariant *value = new QVariant(1);

    QModelIndex index = this->clientIdModel->index(0,0,QModelIndex());
    this->clientIdModel->setData(index,*value,0);
}


void MainWindow::removeCamera(size_t id){

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

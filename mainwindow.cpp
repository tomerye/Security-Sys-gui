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


    connect(&server_,SIGNAL(newEvent(QVector<QString>)),this , SLOT(newEventSlot(QVector<QString>)));
    qRegisterMetaType<QVector<QString> >();
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


void MainWindow::newCamera(int id){

}


void MainWindow::removeCamera(int){

}

void MainWindow::openPictureView(std::string path){
    this->pPictureView = new PictureView(this);
    this->pPictureView->setPicture(path);
    this->show();
}

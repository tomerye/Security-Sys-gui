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
    lables << "Camera_ID" << "Time" << "Priority" << "Picture_file";
    this->eventsModel->setHorizontalHeaderLabels(lables);

    //proxy
    this->eventProxyFilter = new QSortFilterProxyModel(this);
    this->eventProxyFilter->setSourceModel(this->eventsModel);
    this->eventProxyFilter->setFilterKeyColumn(2); //filter priority

    //table
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setModel(this->eventProxyFilter);



    //id list
    this->clientIdModel = new QStringListModel(this);
    ui->listView->setModel(this->clientIdModel);
    ui->listView_2->setModel(this->clientIdModel);


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
    connect(&server_,SIGNAL(newEvent(QVector<QString>)),this , SLOT(updateEventsTable(QVector<QString>)));
    connect(&server_,SIGNAL(newEvent(QVector<QString>)),this , SLOT(updateDB(QVector<QString>)));

    connect(&server_,SIGNAL(downloadProgressSignal(qint64,qint64)),this,SLOT(updateProgressBar(qint64,qint64)));
    connect(&server_,SIGNAL(newPicture(QString)),this,SLOT(openPictureView(QString)));

    setupDB();
    ui->progressBar->setValue(0);

}



MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}


void MainWindow::setupDB(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("events.db");

    if(db.open())
    {
        qDebug() << "DB is open";
    }
    else{
        qDebug() << "Error:= " << db.lastError();
    }
    QSqlQuery query;
    query.exec("DROP TABLE IF EXISTS events");
    query.exec("CREATE TABLE events(Camera_ID int, Time varchar(50), Priority int, Picture_file varchar(30))");
    this->sqlModel = new QSqlTableModel(this);
    this->sqlModel->setTable("events");
    this->ui->tableView_2->setModel(this->sqlModel);
}

void MainWindow::updateProgressBar(qint64 received,qint64 total){
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(received);
}

void MainWindow::updateEventsTable(QVector<QString> event){
    qDebug() << event;
    this->eventsModel->insertRow(0,QModelIndex());
    for(int i=0 ; i<event.size();i++)
    {
        QModelIndex modelindex = this->eventsModel->index(0,i,QModelIndex());
        this->eventsModel->setData(modelindex,event[i]);
    }
    ui->tableView->resizeColumnsToContents();
    this->updateLable();
}

void MainWindow::updateDB(QVector<QString> event){
    QSqlQuery query;
    QString q = "insert into events values("+event[0] +",'" +event[1]+"',"  +event[2]+",'" +event[3] +"')";
    qDebug() << "to DB" << q;
    bool res = query.exec(q);
    qDebug() << "insert res = " << res;
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

void MainWindow::openPictureView(QString path){
    this->pPictureView = new PictureView(this);
    this->pPictureView->setPicture(path.toStdString());
    this->pPictureView->show();
    this->ui->progressBar->setValue(0);
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
    this->sqlModel->select();
    ui->tableView_2->resizeColumnsToContents();
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    this->updateLable();

}


void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    int row = index.row();
    QString id =  this->eventProxyFilter->index(row,0).data().toString();
    QString picName =  this->eventProxyFilter->index(row,3).data().toString();
    std::stringstream ss;
    ss << id.toStdString();
    uint32_t intID;
    ss >> intID;
    this->server_.getFile(intID,picName.toStdString());

}



void MainWindow::on_pushButton_2_clicked()
{
    QString command  = this->ui->plainTextEdit->toPlainText();

    PacketForClient *commandPacket = new PacketForClient();
    commandPacket->opcode_ = 5;
    commandPacket->command_ = command.toStdString();
    QModelIndexList modIndexList = ui->listView_2->selectionModel()->selectedIndexes();
    QString clientID = modIndexList.at(0).data().toString();
    std::stringstream ss;
    ss << clientID.toStdString();
    uint32_t intID;
    ss >> intID;
    this->server_.send(intID,commandPacket);

}

void MainWindow::updateLable(){
    QModelIndexList modIndexList = ui->listView->selectionModel()->selectedIndexes();
    QString clientID = modIndexList.at(0).data().toString();
    QSortFilterProxyModel clientIDFilter;
    clientIDFilter.setSourceModel(this->eventsModel);
    clientIDFilter.setFilterKeyColumn(0);
    clientIDFilter.setFilterFixedString(clientID);
    QSortFilterProxyModel priorityFilter;
    priorityFilter.setSourceModel(&clientIDFilter);
    priorityFilter.setFilterKeyColumn(2);

    priorityFilter.setFilterFixedString("3");
    int priority3 = priorityFilter.rowCount();
    priorityFilter.setFilterFixedString("2");
    int priority2 = priorityFilter.rowCount();
    priorityFilter.setFilterFixedString("1");
    int priority1 = priorityFilter.rowCount();
    QString label;
    std::stringstream ss;
    ss << clientID.toStdString();
    uint32_t intID;
    ss >> intID;
    bool state = this->server_.getClientState(intID);
    QString strState = state?"Active":"Not Active";
    label ="State:" + strState +"\nlevel3: " + QString::number(priority3) +" level2:" + QString::number(priority2) + " level1:" + QString::number(priority1);
    ui->label->setText(label);
}


void MainWindow::on_pushButton_4_clicked()
{
    this->eventsModel->removeRows(0,this->eventsModel->rowCount());
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QString id = index.data().toString();
    std::stringstream ss;
    ss << id.toStdString();
    uint32_t intID;
    ss >> intID;
    PacketForClient *startImageProcessingPacket = new PacketForClient();
    bool clientStatus = server_.getClientState(intID);
    if(clientStatus){
        startImageProcessingPacket->opcode_=4;
    }
    else{
        startImageProcessingPacket->opcode_=3;

    }
    this->server_.send(intID,startImageProcessingPacket);
    server_.setClientState(intID,!clientStatus);

}

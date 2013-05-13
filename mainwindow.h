#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtserver.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "/home/tomer/workspace/Server/src/Server.h"
#include "pictureview.h"
#include <QtCore>
#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    boost::asio::io_service io_service_;
    Server server_;
    boost::thread runThread_;
    PictureView *pPictureView;
    QStandardItemModel *eventsModel;
    QSortFilterProxyModel *eventProxyFilter;

public slots:
    void newEventSlot(QVector<QString>);
    void newCamera(int);
    void removeCamera(int);
    void openPictureView(std::string);
};

#endif // MAINWINDOW_H

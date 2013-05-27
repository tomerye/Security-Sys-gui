#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "Server.h"
#include "pictureview.h"
#include <QtCore>
#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QStringListModel>
#include <QtSql>

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
    QSqlDatabase db;

    void setupDB();

    QStringListModel *clientIdModel;
    QStandardItemModel *eventsModel;
    QStandardItemModel *statisticModel;
    QSortFilterProxyModel *eventProxyFilter;
    QSqlQueryModel *sqlModel;

public slots:
    void newEventSlot(QVector<QString>);
    void removeCamera(u_int32_t);
    void openPictureView(std::string);
    void addClientConnection(u_int32_t);
private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_clicked();
    void on_listView_clicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H

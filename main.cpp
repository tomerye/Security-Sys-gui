#include "mainwindow.h"
#include <QApplication>
#include "qtserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    QtServer qtserver;
    return a.exec();
}

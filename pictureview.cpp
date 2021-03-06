#include "pictureview.h"
#include "ui_pictureview.h"

PictureView::PictureView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PictureView)
{
    ui->setupUi(this);

}

PictureView::~PictureView()
{
    delete ui;
}


void PictureView::setPicture(std::string path){
    QPixmap *image = new QPixmap(QString::fromStdString(path));
    this->ui->label = new QLabel(this);
    this->ui->label->setPixmap(*image);
}

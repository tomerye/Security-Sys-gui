#ifndef PICTUREVIEW_H
#define PICTUREVIEW_H

#include <QDialog>
#include <qgraphicsscene.h>
#include <QPixmap>

namespace Ui {
class PictureView;
}

class PictureView : public QDialog
{
    Q_OBJECT
    
public:
    explicit PictureView(QWidget *parent = 0);
    ~PictureView();
    void setPicture(std::string path);
    
private:
    Ui::PictureView *ui;

};

#endif // PICTUREVIEW_H

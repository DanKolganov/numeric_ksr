#include "helpform.h"
#include "ui_helpform.h"
#include <QPixmap>

HelpForm::HelpForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpForm)
{
    ui->setupUi(this);
    QString filename("/Users/d3zzle/numericLab1/src/gui/src/help screen");
    QImage* img=new QImage;
    img->load(filename);
    ui->image->setPixmap(QPixmap::fromImage(*img));

    QString filename2("/Users/d3zzle/numericLab1/src/gui/src/rk");
    QImage* img2=new QImage;
    img2->load(filename2);
    ui->image_rk_4->setPixmap(QPixmap::fromImage(*img2));
}

HelpForm::~HelpForm()
{
    delete ui;
}

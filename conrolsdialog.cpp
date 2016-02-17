#include "conrolsdialog.h"
#include "ui_conrolsdialog.h"
#include <QPainter>

ConrolsDialog::ConrolsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConrolsDialog)
{
    ui->setupUi(this);
    this->setFixedSize(451, 356);
    this->setModal(true);
    this->setStyleSheet("background-image: url(:/new/pics/pics/controls3.png);");
    this->setWindowTitle("Controls");
}


ConrolsDialog::~ConrolsDialog()
{
    delete ui;
}

void ConrolsDialog::paintEvent(QPaintEvent *pe)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

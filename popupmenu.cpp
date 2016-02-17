#include "popupmenu.h"
#include "ui_popupmenu.h"
#include <QPainter>

PopUpMenu::PopUpMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PopUpMenu)
{
    ui->setupUi(this);
}

void PopUpMenu::paintEvent(QPaintEvent *pe)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

PopUpMenu::~PopUpMenu()
{
    delete ui;
}

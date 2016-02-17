#include "testframe.h"
#include "ui_testframe.h"

TestFrame::TestFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TestFrame)
{
    ui->setupUi(this);
}

TestFrame::~TestFrame()
{
    delete ui;
}

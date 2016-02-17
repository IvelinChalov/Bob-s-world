#include "dialog.h"
#include "ui_dialog.h"
#include "savelevelsystem.h"
#include "component.h"
#include "errorsystem.h"
#include <QMessageBox>
#include <QPainter>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QRegExp rex("[1-9 a-z A-Z]*");
    QValidator* validator = new QRegExpValidator(rex, this);
    ui->fileNameEdit->setValidator(validator);
    ui->fileNameEdit->setMaxLength(15);
    ui->fileNameEdit->setToolTip("Cool charecters are 1 to 9 and a to z");
    this->setStyleSheet("Dialog{background-color:white;}");
}

void Dialog::paintEvent(QPaintEvent *pe)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_saveBtn_clicked() //TODO check for emty or just space and restrict the lenght of the file name
{
    QString error_message;
    QMessageBox messageBox;
    messageBox.setFixedSize(500,200);
    messageBox.setWindowTitle("Message");
    //messageBox.setWindowFlags(Qt::WindowContextHelpButtonHint);
    int error_code = SaveLevelSystem(ui->fileNameEdit->text(), false);
    if(error_code == NO_ERROR) {
        error_message = get_error_message(error_code);
        messageBox.setText(error_message);
        this->hide();
        messageBox.exec();
    }else{
         error_message = get_error_message(error_code);
         messageBox.setText(error_message);
         messageBox.exec();
    }


}

void Dialog::on_cancelBtn_clicked()
{
    this->hide();
}

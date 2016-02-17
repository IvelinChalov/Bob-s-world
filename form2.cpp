#include "form2.h"
#include "ui_form2.h"
#include "custombutton.h"
#include "loadlevelsystem.h"
#include "deletesystem.h"
#include "component.h"
#include "glwidget.h"
#include <QDebug>
#include <QPainter>
#include <QDir>

form2::form2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form2)
{
    ui->setupUi(this);
    ui->scrollArea->setFrameShape(QFrame::NoFrame);
    pos = 0;
}

form2::~form2()
{
    delete ui;
}

StackedWidget *form2::getStackedWid() const
{
    return stackedWid;
}

void form2::paintEvent(QPaintEvent *pe)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

void form2::setStackedWid(StackedWidget *value)
{
    stackedWid = value;
}

void form2::on_pushButton_clicked()
{
    if(from_editor) {
      getStackedWid()->setCurrentIndex(GAME_FORM);
      clear_all_entities();
      load_level_system("quickSave", true);
    }else{
     getStackedWid()->setCurrentIndex(START_FORM);
     from_editor = true;
    }
}

void form2::add() {
   customButton* button = new customButton;
   button->setMaximumWidth(50);
   button->setMinimumHeight(50);
   button->setMaximumWidth(50);
   button->setMaximumHeight(50);
   //button->setText(img[pos]);
   connect(button, SIGNAL(clicked()), button, SLOT(textChange()));
   ui->gridLayout->addWidget(button, 0, pos);

   pos++;
   button->show();
}

void form2::setLevelCount() {
    QString current = QCoreApplication::applicationDirPath();
    QString path = current + "/levels";
    //"C:/Users/The_warri0r/Documents/test/levels"
    QDir dir(path);
    QStringList filter;
    filter << "*.txt";
    QFileInfoList level_number = dir.entryInfoList(filter, QDir::Files);
    level_count = level_number.size();
    qDebug() << "Path = " << path << "level num = " << level_number.size();
}

void form2::fillLevelArray()
{
    QString current = QCoreApplication::applicationDirPath();
    QString path = current + "/levels";
    QDir dir(path);
    QStringList filter;
    filter << "*.txt";
    QFileInfoList level_number = dir.entryInfoList(filter, QDir::Files);
    level_arr.clear();//when level count change you must reset it
    for (int i = 0; i < level_number.size(); ++i) {
        level_arr.push_back(level_number.at(i).baseName());
    }

}

void form2::addButtons() {
    int current_row = 0;
    int current_column = -1;
    int max_row_num = 4;
 for(int i = 0; i < level_arr.size(); ++i) {
     if(level_arr.value(i) == "quickSave")continue;
     customButton* button = new customButton;
     int button_size = 90;
     button->setMaximumWidth(button_size);
     button->setMinimumHeight(button_size);
     button->setMaximumWidth(button_size);
     button->setMaximumHeight(button_size);
     button->setText(level_arr.value(i));
     button->setFile_name(level_arr.value(i));
     button->setStackedWid(getStackedWid());
     button->setGame_form(game_form);
     QString style = "customButton{background-image: url(:/new/pics/pics/load_button.png); border: none; "
                     "} customButton:hover{border-width:0.5px; border-style:solid; border-color: rgb(255, 255, 255); }";
     button->setStyleSheet(style);
     QFont serifFont("Times", 10, QFont::Bold);
     button->setFont(serifFont);
     connect(button, SIGNAL(clicked()), button, SLOT(load_level()));
     if(current_column == (max_row_num)) {
         current_column = 0;
         ++current_row;
     } else {
         ++current_column;
     }
     ui->gridLayout->addWidget(button, current_row, current_column);
     button->show();
 }
}

void form2::generate_levels() {
    setLevelCount();
    qDebug() << "level count = " << level_count;
    fillLevelArray();
    addButtons();
}


int form2::getLevel_count() const
{
    return level_count;
}

void form2::setGame_form(GameForm *value)
{
    game_form = value;
}

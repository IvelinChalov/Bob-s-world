#include "gameform.h"
#include "ui_gameform.h"
#include "deletesystem.h"
#include "component.h"
#include "Entity.h"
#include "savelevelsystem.h"
#include "form2.h"
#include "loadlevelsystem.h"
#include "popupmenu.h"
#include <QDebug>
#include <iostream>
#include <QStackedWidget>
#include <QPainter>

GameForm::GameForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameForm)
{
    cout<<"Constructor is initialized"<<endl;
    ui->setupUi(this);
    PopUpMenu* pop_up_menu = new PopUpMenu;
    //gameForm->setStackedWid(ui->stack_widget);
    ui->popUpMenu->addWidget(pop_up_menu);
    ui->popUpMenu->setCurrentIndex(0);
    ui->popUpMenu->setVisible(false);
    ui->widget_2->setWindowModality(Qt::WindowModal);
    ui->widget_2->setWindowTitle("Save as..");
    ui->widget_2->setFixedSize(ui->widget_2->size());

    QPixmap pixmap("C:/Users/The_warri0r/Documents/test/picssave_icon2.png");
    QIcon ButtonIcon(pixmap);
    ui->save_btn->setIcon(ButtonIcon);
    ui->save_btn->setIconSize(pixmap.rect().size());
    ui->save_btn->setVisible(false);
    ui->loadButn->setVisible(false);

    ui->restart_btn->setEnabled(false);


//    ui->widget_2->setGeometry(0, 0, 200, 200);1
//    qDebug() << "height" << this->x();
      //QPoint p = QWidget::mapFromParent(this->pos());
//    qDebug() << "pos = " << this->pos().x() << "|" << this->pos().y();
    //ui->widget_2->move(this->pos().x(), this->pos().y());
}

GameForm::~GameForm()
{
    delete ui;
}

StackedWidget *GameForm::getStackedWid() const
{
    return stackedWid;
}

void GameForm::setStackedWid(StackedWidget *value)
{
    stackedWid = value;
}

void GameForm::on_pushButton_clicked()
{
    if(from_editor) {
      getStackedWid()->setCurrentIndex(START_FORM);
      ui->loadButn->setVisible(false);
      ui->save_btn->setVisible(false);
    }else{
     getStackedWid()->setCurrentIndex(LEVEL_SELECT_FORM);
    }
    clear_all_entities();
    reset_level_flags();

//    qDebug() << "screen height = " << this->height();
//    qDebug() << "screen width = " << this->width();
}

void GameForm::on_loadButn_clicked()
{
    load_from_editor = true;
    getLoad_form()->generate_levels();
    getStackedWid()->setCurrentIndex(LEVEL_SELECT_FORM);
    reset_level_flags();
    qDebug() << "load_from_editor = "<< load_from_editor;
//    getStackedWid()->setCurrentIndex(1);
//    clear_all_entities();
}

void GameForm::on_save_btn_clicked()
{
    ui->widget_2->show();
}

form2 *GameForm::getLoad_form() const
{
    return load_form;
}

void GameForm::setLoad_form(form2 *value)
{
    load_form = value;
}

void GameForm::paintEvent(QPaintEvent *pe)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

void GameForm::on_play_btn_clicked()
{
    bool p = ui->widget->getPause();
    p = !p;
    ui->widget->setPause(p);
    if(p){
        //ui->play_btn->setText("pause");
        QString style = "#play_btn{background-image: url(:/new/pics/pics/play.png); border: none; } #play_btn:hover{border-width:0.5px; border-style:solid; border-color: rgb(255, 255, 255); }";
        ui->play_btn->setStyleSheet(style);
        ui->play_btn->setEnabled(false);
        ui->restart_btn->setEnabled(true);
    }else{
        QString style = "#play_btn{background-image: url(:/new/pics/pics/pause.png); border: none; } #play_btn:hover{border-width:0.5px; border-style:solid; border-color: rgb(255, 255, 255); }";
        ui->play_btn->setStyleSheet(style);
        remove_file("quickSave");
        SaveLevelSystem("quickSave", true);
       // ui->play_btn->setText("play");
    }
}

void GameForm::on_restart_btn_clicked() {
        cout<<"clicking on restart BTN " << endl;
        clear_all_entities();
//    if(from_editor && !load_from_editor){
        load_level_system("quickSave", true);
        if(!from_editor) add_move_mask();//TODO it removes moving. The name is wronge
        ui->play_btn->setEnabled(true);
        ui->restart_btn->setEnabled(false);
//    }else{
//        load_level_system(current_level);
//    }
}

QString GameForm::getCurrent_level() const
{
    return current_level;
}

void GameForm::setCurrent_level(QString &value)
{
    current_level = value;
}

void GameForm::reset_level_flags() {
    QString style = "#play_btn{background-image: url(:/new/pics/pics/play.png); border: none; } #play_btn:hover{border-width:0.5px; border-style:solid; border-color: rgb(255, 255, 255); }";
    ui->play_btn->setStyleSheet(style);
    ui->restart_btn->setEnabled(false);
    ui->play_btn->setEnabled(true);
    ui->widget->setPause(true);
}

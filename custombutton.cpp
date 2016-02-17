#include "custombutton.h"
#include "loadlevelsystem.h"
#include "savelevelsystem.h"
#include "glwidget.h"
#include "deletesystem.h"
#include <QDebug>
#include <QString>
#include <iostream>
#include <QDesktopWidget>

using namespace std;

customButton::customButton(QPushButton *parent) : QPushButton(parent)
{
    buttonT = "";
}

void customButton::setButtonText(QString text)
{
    this->setText(text);
}

QString customButton::getButtonT() const
{
    return buttonT;
}

void customButton::setButtonT(const QString &value)
{
    buttonT = value;
}

QString customButton::getFile_name() const
{
    return file_name;
}

void customButton::setFile_name(const QString &value)
{
    file_name = value;
}

StackedWidget *customButton::getStackedWid() const
{
    return stackedWid;
}

void customButton::setStackedWid(StackedWidget *value)
{
    stackedWid = value;
}

void customButton::setGame_form(GameForm *value)
{
    game_form = value;
}

void customButton::textChange()
{
    setButtonText("hi there");

}

void customButton::load_level()
{
//    cout<<"LOADING THE LEVEL !!!"<<endl;
//    clear_all_entities();
//    cout<<"after clear"<<endl;
    game_form->reset_level_flags();
//    cout<<"after reset"<<endl;
//   // load_level_system(getFile_name(), false);original place
    QString file_name = getFile_name();
//    cout<<"after file name"<<endl;
    game_form->setCurrent_level(file_name);
//    cout<<"after setCurrent_level"<<endl;
//    load_level_system(getFile_name(), false);
//    if(!from_editor)add_move_mask();
//    cout<<"after add_move_mask"<<endl;
    getStackedWid()->setCurrentIndex(GAME_FORM);
    if(!from_editor) {
        clear_all_entities();
        load_level_system(file_name, false);
        add_move_mask();
    }else{
       clear_all_entities();
       load_level_system(file_name, false);
       QDesktopWidget dw;
       QRect mainScreenSize = dw.availableGeometry(dw.primaryScreen());
       int w = mainScreenSize.width() * 0.6;
      // create_templates(w - 39);//Correction because of a smaller window
       //init_quantity(); why do i need this?
    }
//    cout<<"after setCurrentIndex"<<endl;
//    if(from_editor){
//        create_templates(1048);
//        cout<<"after create_templates"<<endl;
//        init_quantity();
//        cout<<"after init_quantity"<<endl;
//    }

}

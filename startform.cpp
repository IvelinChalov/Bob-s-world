#include "startform.h"
#include "glwidget.h"
#include "entity.h"
#include "loadlevelsystem.h"
#include "ui_startform.h"
#include <QDebug>
#include <QDesktopWidget>

StartForm::StartForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartForm)
{
    ui->setupUi(this);
    QDesktopWidget dw;
    QRect mainScreenSize = dw.availableGeometry(dw.primaryScreen());
    int w = 0.12 * mainScreenSize.width();
    int h = 0.11 * mainScreenSize.height();
    ui->startButton->setMinimumSize(w, h);
    ui->startButton->setMaximumSize(w, h);
}

StartForm::~StartForm()
{
    delete ui;
}

StackedWidget *StartForm::getStackedWid() const
{
    return stackedWid;
}

void StartForm::setStackedWid(StackedWidget *value)
{
    stackedWid = value;
}

void StartForm::on_pushButton_4_clicked()
{

}

void StartForm::on_startButton_clicked()
{
    getStackedWid()->setCurrentIndex(LEVEL_SELECT_FORM);
    getLoad_form()->generate_levels();
    from_editor = false;
    load_from_editor = false;
}

void StartForm::on_levelCreatorBtn_clicked()
{
    getStackedWid()->setCurrentIndex(GAME_FORM);
    GLWidget* wid =  getStackedWid()->findChild<GLWidget*>("widget");
    create_templates(wid->getScreen_width());
    qDebug() << "width === " << wid->getScreen_width();
    from_editor = true;
    QPushButton* button = getGame_form()->findChild<QPushButton*>("save_btn");
    button->setVisible(true);
    button = getGame_form()->findChild<QPushButton*>("loadButn");
    button->setVisible(true);
    qDebug() << "BUTTON flag" << from_editor;
}

GameForm *StartForm::getGame_form() const
{
    return game_form;
}

void StartForm::setGame_form(GameForm *value)
{
    game_form = value;
}

form2 *StartForm::getLoad_form() const
{
    return load_form;
}

void StartForm::setLoad_form(form2 *value)
{
    load_form = value;
}

void StartForm::paintEvent(QPaintEvent *pe)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

QDialog *StartForm::getDialog() const
{
    return dialog;
}

void StartForm::setDialog(QDialog *value)
{
    dialog = value;
}

void StartForm::on_controlsButton_clicked()
{
    ui->widget->setVisible(true);
}

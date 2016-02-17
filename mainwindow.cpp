#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "testframe.h"
#include "form2.h"
#include "startform.h"
#include "gameform.h"
#include "Entity.h"
#include <qopengl.h>
#include <QPushButton>
#include <QDesktopWidget>
#include <QDebug>
#include "glwidget.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    QDesktopWidget dw;
    QRect mainScreenSize = dw.availableGeometry(dw.primaryScreen());
    int w = mainScreenSize.width() * 0.6;
    int h = mainScreenSize.height() * 0.8;
    screen_width = w;
    screen_height = h;
    //int screen_width = w;
    //int screen_height = h;
    qDebug() << "w = " << w << "h = " << h;
    setFixedSize(w, h);

    QIcon icon("pics/icon.png");
    this->setWindowIcon(icon);
    this->setWindowTitle("Bob world");

    form2* f2 = new form2;
    f2->setStackedWid(ui->stack_widget);


    StartForm* startForm = new StartForm;
    startForm->setStackedWid(ui->stack_widget);
    startForm->setLoad_form(f2);

    GameForm* gameForm = new GameForm;
    gameForm->setStackedWid(ui->stack_widget);
    gameForm->setLoad_form(f2);
    GLWidget* wi =  gameForm->findChild<GLWidget*>("widget");
    wi->setGf(gameForm);
    f2->setGame_form(gameForm);

    startForm->setGame_form(gameForm);

    ui->stack_widget->addWidget(startForm);
    ui->stack_widget->addWidget(f2);
    ui->stack_widget->addWidget(gameForm);

    change_page(0);
//    setStack(ui->stack_widget);

    QPushButton* button =  ui->stack_widget->findChild<QPushButton*>("exitButton");
    connect(button, SIGNAL(clicked()), this, SLOT(close()));
    //qDebug() << "pos = " << this->pos().x() << "|" << this->pos().y();
}

void MainWindow::change_page(int page)
{
    ui->stack_widget->setCurrentIndex(page);
}

int MainWindow::get_screen_width()
{
    return screen_width;
}

int MainWindow::get_screen_height()
{
    return screen_height;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chage(int id)
{
    ui->stack_widget->setCurrentIndex(id);
}

boolean clicked = false;
void MainWindow::on_pushButton_clicked()
{
//    if(!clicked) {
//        ui->label->setVisible(false);
//        clicked = true;
//    } else {
//        ui->label->setVisible(true);
//        clicked = false;
//    }
}

void MainWindow::on_pushButton_2_clicked()
{
    //change_page(2);
}

QStackedWidget *MainWindow::getStack() const
{
    return stack;
}

void MainWindow::setStack(QStackedWidget *value)
{
    stack = value;
}

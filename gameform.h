#ifndef GAMEFORM_H
#define GAMEFORM_H

#include <QWidget>
#include "stackedwidget.h"
#include "form2.h"

namespace Ui {
class GameForm;
}

class form2;//forward declaratin because two classes depend on eachother

class GameForm : public QWidget
{
    Q_OBJECT

public:
    explicit GameForm(QWidget *parent = 0);
    ~GameForm();

    StackedWidget *getStackedWid() const;
    void setStackedWid(StackedWidget *value);

    form2 *getLoad_form() const;
    void setLoad_form(form2 *value);
    void paintEvent(QPaintEvent *pe);

    void setCurrent_level(QString &value);
    void reset_level_flags();

    QString getCurrent_level() const;

private slots:
    void on_pushButton_clicked();

    void on_loadButn_clicked();

    void on_save_btn_clicked();

    void on_play_btn_clicked();

    void on_restart_btn_clicked();

private:
    Ui::GameForm *ui;
    StackedWidget* stackedWid;
    form2* load_form;
    form2* ff;
    QString current_level;

};

#endif // GAMEFORM_H

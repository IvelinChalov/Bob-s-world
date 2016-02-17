#ifndef STARTFORM_H
#define STARTFORM_H

#include <QWidget>
#include <QDialog>
#include "form2.h"
#include "gameform.h"
#include "stackedwidget.h"

namespace Ui {
class StartForm;
}

class StartForm : public QWidget
{
    Q_OBJECT

public:
    explicit StartForm(QWidget *parent = 0);
    ~StartForm();

    StackedWidget *getStackedWid() const;
    void setStackedWid(StackedWidget *value);

    QDialog *getDialog() const;
    void setDialog(QDialog *value);

    form2 *getLoad_form() const;
    void setLoad_form(form2 *value);
    void paintEvent(QPaintEvent *pe);


    GameForm *getGame_form() const;
    void setGame_form(GameForm *value);

private slots:
    void on_pushButton_4_clicked();

    void on_startButton_clicked();

    void on_levelCreatorBtn_clicked();

    void on_controlsButton_clicked();

private:
    Ui::StartForm *ui;
    QDialog* dialog;
    StackedWidget* stackedWid;
    form2* load_form;
    GameForm* game_form;
};

#endif // STARTFORM_H

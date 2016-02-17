#ifndef FORM2_H
#define FORM2_H

#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QVector>
#include "stackedwidget.h"
#include "gameform.h"

namespace Ui {
class form2;
}

class GameForm;

class form2 : public QWidget
{
    Q_OBJECT

public:
    explicit form2(QWidget *parent = 0);
    ~form2();

    StackedWidget *getStackedWid() const;
    void paintEvent(QPaintEvent *pe);
    void setStackedWid(StackedWidget *value);
    void add();
    void setLevelCount();
    void fillLevelArray();
    void addButtons();
    void generate_levels();

    int getLevel_count() const;

    void setGame_form(GameForm *value);

private slots:
    void on_pushButton_clicked();

private:
    Ui::form2 *ui;
    StackedWidget* stackedWid;
    int pos;
    QVector<QString> level_arr;
    int level_count;
    GameForm* game_form;
};

#endif // FORM2_H

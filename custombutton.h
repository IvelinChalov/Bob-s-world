#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QString>
#include "stackedwidget.h"
#include "gameform.h"


class customButton : public QPushButton
{
    Q_OBJECT
public:
    explicit customButton(QPushButton *parent = 0);

    void setButtonText(QString text);

    QString getButtonT() const;
    void setButtonT(const QString &value);

    QString getFile_name() const;
    void setFile_name(const QString &value);

    StackedWidget *getStackedWid() const;
    void setStackedWid(StackedWidget *value);

    void setGame_form(GameForm *value);

private:
    QString buttonT;
    QString file_name;
    StackedWidget* stackedWid;
    GameForm* game_form;


signals:

public slots:
    void textChange();
    void load_level();

};

#endif // CUSTOMBUTTON_H

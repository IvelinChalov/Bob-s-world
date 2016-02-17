#ifndef POPUPMENU_H
#define POPUPMENU_H

#include <QWidget>

namespace Ui {
class PopUpMenu;
}

class PopUpMenu : public QWidget
{
    Q_OBJECT

public:
    explicit PopUpMenu(QWidget *parent = 0);
    void paintEvent(QPaintEvent *pe);
    ~PopUpMenu();

private:
    Ui::PopUpMenu *ui;
};

#endif // POPUPMENU_H

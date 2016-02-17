#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

#include <QStackedWidget>
#include <mainwindow.h>

namespace Ui {
class StackedWidget;
}

class StackedWidget : public QStackedWidget
{
    Q_OBJECT

public:
    explicit StackedWidget(QWidget *parent = 0);
    ~StackedWidget();
    void setMainWindow(MainWindow window);
    MainWindow getMainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::StackedWidget *ui;
};

#endif // STACKEDWIDGET_H

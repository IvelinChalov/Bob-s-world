#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void change_page(int page);
    int get_screen_width();
    int get_screen_height();
    ~MainWindow();

    QStackedWidget *getStack() const;
    void setStack(QStackedWidget *value);

private slots:

    void chage(int id);
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stack;
    int screen_width;
    int screen_height;
};

#endif // MAINWINDOW_H

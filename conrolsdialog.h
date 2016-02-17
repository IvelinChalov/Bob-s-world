#ifndef CONROLSDIALOG_H
#define CONROLSDIALOG_H

#include <QDialog>

namespace Ui {
class ConrolsDialog;
}

class ConrolsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConrolsDialog(QWidget *parent = 0);
    void paintEvent(QPaintEvent *pe);
    ~ConrolsDialog();

private:
    Ui::ConrolsDialog *ui;
};

#endif // CONROLSDIALOG_H

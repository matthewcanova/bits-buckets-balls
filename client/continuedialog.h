#ifndef CONTINUEDIALOG_H
#define CONTINUEDIALOG_H

#include <QDialog>

namespace Ui {
class continueDialog;
}

class continueDialog : public QDialog
{
    Q_OBJECT

public:
    explicit continueDialog(QWidget *parent = 0);
    ~continueDialog();

signals:
    void accepted();
    void rejected();

private slots:
    void yes_pushed();
    void no_pushed();

private:
    Ui::continueDialog *ui;

};

#endif // CONTINUEDIALOG_H

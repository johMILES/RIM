#ifndef BASEINFOEDIT_H
#define BASEINFOEDIT_H

#include <QDialog>

namespace Ui {
class BaseInfoEdit;
}

class BaseInfoEdit : public QDialog
{
    Q_OBJECT

public:
    explicit BaseInfoEdit(QWidget *parent = 0);
    ~BaseInfoEdit();

private slots:
    void cancel();
    void confirm();

private:
    Ui::BaseInfoEdit *ui;
};

#endif // BASEINFOEDIT_H

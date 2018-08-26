#ifndef DATABASEEDIT_H
#define DATABASEEDIT_H

#include <QDialog>

namespace Ui {
class DatabaseEdit;
}

class DatabaseEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseEdit(QWidget *parent = 0);
    ~DatabaseEdit();

private slots:
    void cancel();
    void confirm();

private:
    Ui::DatabaseEdit *ui;
};

#endif // DATABASEEDIT_H

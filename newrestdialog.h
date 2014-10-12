#ifndef NEWRESTDIALOG_H
#define NEWRESTDIALOG_H

#include <QDialog>
#include "rest.h"

namespace Ui {
class NewRestDialog;
}

class NewRestDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewRestDialog(QWidget *parent = 0);
    ~NewRestDialog();

    Rest* createRest() const;

private slots:
    void confirmed();
    void typeChanged();
    
private:
    bool isInterval() const;
    void showError(const QString &);

    Ui::NewRestDialog *ui;
};

#endif // NEWRESTDIALOG_H

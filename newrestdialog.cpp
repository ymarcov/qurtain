#include "newrestdialog.h"
#include "ui_newrestdialog.h"
#include "intervalrest.h"
#include "singleshotrest.h"

#include <QDateTime>
#include <QMessageBox>

NewRestDialog::NewRestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewRestDialog)
{
    ui->setupUi(this);

    // connect signals & slots
    connect(ui->btnAdd, SIGNAL(clicked()), SLOT(confirmed()));
    connect(ui->radioInterval, SIGNAL(clicked()), SLOT(typeChanged()));
    connect(ui->radioSingleShot, SIGNAL(clicked()), SLOT(typeChanged()));

    // get current time with 0 msec
    QDateTime now = QDateTime::currentDateTime();
    QTime nowT = now.time();
    now.setTime(QTime(nowT.hour(), nowT.minute(), nowT.second(), 0));

    // set default values
    ui->editStart->setDateTime(now);
    ui->editEnd->setDateTime(now.addSecs(3600));
    ui->editInterval->setTime(QTime(0, 1, 0));
    ui->editDuration->setTime(QTime(0, 1, 0));
    ui->radioSingleShot->click();
}

NewRestDialog::~NewRestDialog()
{
    delete ui;
}

Rest* NewRestDialog::createRest() const
{
    if (isInterval())
        return new IntervalRest(ui->editStart->dateTime(),
                                ui->editEnd->dateTime(),
                                ui->editInterval->time(),
                                ui->editDuration->time());
    else
        return new SingleShotRest(ui->editStart->dateTime(),
                                  ui->editDuration->time());
}

void NewRestDialog::confirmed()
{
    if (isInterval())
    {
        if (ui->editEnd->dateTime() < ui->editStart->dateTime())
            return showError("End time is earlier than start time.");
        if (QTime(0, 0, 0).secsTo(ui->editInterval->time()) == 0)
            return showError("Interval must be greater than zero.");
    }
    else
    {
        if (ui->editStart->dateTime() < QDateTime::currentDateTime())
            return showError("Trigger time is in the past.");
    }

    if (QTime(0, 0, 0).secsTo(ui->editDuration->time()) == 0)
        return showError("Duration must be greater than zero.");

    emit accept();
}

void NewRestDialog::typeChanged()
{
    ui->editEnd->setEnabled(isInterval());
    ui->editInterval->setEnabled(isInterval());
}

bool NewRestDialog::isInterval() const
{
    return ui->radioInterval->isChecked();
}

void NewRestDialog::showError(const QString &err)
{
    QMessageBox::warning(this, "Validation Error", err, QMessageBox::Ok);
}

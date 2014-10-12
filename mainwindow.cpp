#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newrestdialog.h"
#include "intervalrest.h"
#include "singleshotrest.h"

#include <QDesktopWidget>
#include <QPalette>
#include <QStandardItemModel>
#include <QStandardItem>

// TODO
// 1. ESC to hide curtain
// 2. Add options to show countdown in curtain

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer),
    dirty(false)
{
    ui->setupUi(this);
    setupCurtain();

    // connect signals & slots
    connect(ui->actionAdd, SIGNAL(triggered()), SLOT(newRestDialog()));
    connect(ui->btnRemoveAll, SIGNAL(clicked()), SLOT(removeAll()));
    connect(ui->btnRemoveSelected, SIGNAL(clicked()), SLOT(removeSelected()));
    connect(timer, SIGNAL(timeout()), SLOT(updateClock()));
    connect(timer, SIGNAL(timeout()), SLOT(updateCurtainVisibility()));
    connect(timer, SIGNAL(timeout()), SLOT(cleanupExpired()));
    connect(timer, SIGNAL(timeout()), SLOT(refreshList()));

    // start clock
    timer->setInterval(50);
    timer->start();

    // update UI immediately
    updateClock();
    refreshList();
}

MainWindow::~MainWindow()
{
    delete timer;
    qDeleteAll(curtain);
    removeAll();
    delete ui;
}

void MainWindow::newRestDialog()
{
    NewRestDialog dialog;
    if (dialog.exec() == NewRestDialog::Accepted)
    {
        rests.append(dialog.createRest());
        dirty = true;
    }
}

void MainWindow::updateClock()
{
    QTime now = QTime::currentTime();
    QString msg = QString("Clock: %0").arg(now.toString());
    ui->statusBar->showMessage(msg);
}


void MainWindow::updateCurtainVisibility()
{
    QDateTime now = QDateTime::currentDateTime();

    if (curtainExpiration.isValid())
    {
        if (now.msecsTo(curtainExpiration) <= 0)
        {
            curtainExpiration = QDateTime();
            showCurtain(false);
        }
        return;
    }

    foreach (const Rest *r, rests)
        if (r->triggeredAt(now))
        {
            int durationSeconds = QTime(0, 0, 0).secsTo(r->duration);
            curtainExpiration = QDateTime::currentDateTime().addSecs(durationSeconds);
            showCurtain(true);
            return;
        }
}

void MainWindow::cleanupExpired()
{
    QDateTime now = QDateTime::currentDateTime();

    QMutableListIterator<Rest*> iterator(rests);
    while (iterator.hasNext())
    {
        Rest *r = iterator.next();
        if (r->expiresAt(now))
        {
            iterator.remove();
            delete r;
            dirty = true;
        }
    }
}

void MainWindow::refreshList()
{
    if (!dirty)
        return;

    QStandardItemModel *model = new QStandardItemModel(0, 5);

    // create header

    {
        QStringList labels;
        labels.append("Type");
        labels.append("Start");
        labels.append("End");
        labels.append("Interval");
        labels.append("Duration");
        model->setHorizontalHeaderLabels(labels);
    }

    // create items

    foreach (const Rest *r, rests)
    {
        QList<QStandardItem*> cols;
        QString dateFormat = "yyyy/MM/dd hh:mm:ss";

        if (const SingleShotRest *ssr = dynamic_cast<const SingleShotRest*>(r))
        {
            cols.append(new QStandardItem("Single Shot"));
            cols.append(new QStandardItem(ssr->triggerTime.toString(dateFormat)));
            cols.append(new QStandardItem("N/A"));
            cols.append(new QStandardItem("N/A"));
            cols.append(new QStandardItem(ssr->duration.toString()));
        }
        else if (const IntervalRest *ir = dynamic_cast<const IntervalRest*>(r))
        {
            cols.append(new QStandardItem("Interval"));
            cols.append(new QStandardItem(ir->start.toString(dateFormat)));
            cols.append(new QStandardItem(ir->end.toString(dateFormat)));
            cols.append(new QStandardItem(ir->interval.toString()));
            cols.append(new QStandardItem(ir->duration.toString()));
        }

        model->appendRow(cols);
    }

    delete ui->tableView->model();
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    dirty = false;
}

void MainWindow::removeAll()
{
    qDeleteAll(rests);
    rests.clear();
    dirty = true;
}

void MainWindow::removeSelected()
{
    QModelIndex idx = ui->tableView->currentIndex();
    if (idx.isValid())
    {
        QList<Rest*>::iterator rest = rests.begin() + idx.row();
        delete *rest;
        rests.erase(rest);
        dirty = true;
    }
}

void MainWindow::setupCurtain()
{
    QList<QWidget*> result;
    QDesktopWidget *desktop = QApplication::desktop();

    QPalette palette;
    palette.setColor(QPalette::Background, Qt::black);

    for (int i = 0; i < desktop->screenCount(); i++)
    {
        QWidget *screenCurtain = new QWidget;
        screenCurtain->setAutoFillBackground(true);
        screenCurtain->setPalette(palette);
        screenCurtain->setCursor(QCursor(Qt::BlankCursor));
        screenCurtain->setWindowFlags(Qt::WindowStaysOnTopHint);
        screenCurtain->setGeometry(desktop->screenGeometry(i));
        result.append(screenCurtain);
    }

    curtain.swap(result);
}

void MainWindow::showCurtain(bool isVisible)
{
    foreach (QWidget *screenCurtain, curtain)
        if (isVisible)
            screenCurtain->showFullScreen();
        else
            screenCurtain->hide();
}


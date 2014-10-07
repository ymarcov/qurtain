#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "rest.h"

#include <QDateTime>
#include <QList>
#include <QPointer>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void cleanupExpired();
    void newRestDialog();
    void refreshList();
    void removeAll();
    void removeSelected();
    void updateClock();
    void updateCurtainVisibility();

private:
    void setupCurtain();
    void showCurtain(bool);

    Ui::MainWindow *ui;
    QTimer *timer;
    QList<QWidget*> curtain;
    QList<Rest*> rests;
    QDateTime curtainExpiration;
    bool dirty;
};

#endif // MAINWINDOW_H

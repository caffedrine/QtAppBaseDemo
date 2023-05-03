#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QObject>
#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QTime>

#include "src/MainApp.h"


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

private:
    Ui::MainWindow *ui;
    MainApp mainApp;
};

#endif // _MAINWINDOW_H_

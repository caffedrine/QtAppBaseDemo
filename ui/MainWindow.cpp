#include <QNetworkInterface>
#include <iostream>
#include <QDateTime>
#include "MainWindow.h"
#include "ui_mainwindow.h"

#include "base/utils/utils.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

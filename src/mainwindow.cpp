#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QAction>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    connect(this->findChild<QAction *>("actionTest"), &QAction::triggered, this, &MainWindow::test);
    connect(this->findChild<QAction *>("actionQuit"), &QAction::triggered, this, &QApplication::quit);
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    this->findChild<GuitarBoard *>("guitarboard")->resize();
}

void MainWindow::test()
{
    GuitarBoard *guitarboard = this->findChild<GuitarBoard *>("guitarboard");
    unsigned char played[4] = {72, 45, 54, 60};

    guitarboard->play(1, 0);
}

GuitarBoard *MainWindow::guitarboard()
{
    return this->findChild<GuitarBoard *>("guitarboard");
}

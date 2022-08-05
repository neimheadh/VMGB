#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <iostream>
#include "settingwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _settings = new QSettings("Neimheadh", "VMGB", this);

    QWidget *content = this->findChild<QWidget *>("content");
    _contentLayout = new QVBoxLayout(this);
    _contentLayout->setMargin(0);
    content->setLayout(_contentLayout);

    _guitarboard = new GuitarBoard(content, _settings);
    _contentLayout->addWidget(_guitarboard);
    _guitarboard->setObjectName("guitarboard");

    connect(this->findChild<QAction *>("actionQuit"), &QAction::triggered, this, &QApplication::quit);
    connect(this->findChild<QAction *>("actionSettings"), &QAction::triggered, this, &MainWindow::openSettingWindow);
}

MainWindow::~MainWindow()
{
    delete _guitarboard;
    delete _contentLayout;
    delete _ui;
}

void MainWindow::openSettingWindow()
{
    SettingWindow *settingWindow = new SettingWindow(_settings, this);

    settingWindow->exec();
    settingWindow->close();

    delete settingWindow;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    _guitarboard->resize();
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    _guitarboard->resize();
}

GuitarBoard *MainWindow::guitarboard()
{
    return this->_guitarboard;
}

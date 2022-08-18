#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <iostream>
#include "settingswindow.h"


MainWindow::MainWindow(EventManager *eventManager, QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
{
    _eventManager = eventManager;

    _ui->setupUi(this);

    _settings = new QSettings("Neimheadh", "VMGB", this);

    QWidget *content = this->findChild<QWidget *>("content");
    _contentLayout = new QVBoxLayout(this);
    _contentLayout->setMargin(0);
    content->setLayout(_contentLayout);

    _guitarboard = new Guitar::Board(_settings, _eventManager);
    _contentLayout->addWidget(_guitarboard);
    _guitarboard->setObjectName("guitarboard");

    connect(this->findChild<QAction *>("actionQuit"), &QAction::triggered, this, &QApplication::quit);
    connect(this->findChild<QAction *>("actionSettings"), &QAction::triggered, this, &MainWindow::openSettingsWindow);
}

MainWindow::~MainWindow()
{
    delete _guitarboard;
    delete _contentLayout;
    delete _ui;
}

void MainWindow::openSettingsWindow()
{
    SettingsWindow *settingsWindow = new SettingsWindow(_settings, _guitarboard, _eventManager);

    settingsWindow->exec();
    settingsWindow->close();

    delete settingsWindow;
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

Guitar::Board *MainWindow::guitarboard()
{
    return this->_guitarboard;
}

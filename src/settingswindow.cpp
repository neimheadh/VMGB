#include "settingswindow.h"
#include "ui_settingswindow.h"

#include "setting/driver.h"
#include "setting/guitar.h"

SettingsWindow::SettingsWindow(QSettings *settings, Guitar::Board *guitar, EventManager *eventManager, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::SettingsWindow)
{
    _eventManager = eventManager;
    _guitar = guitar;
    _settings = settings;

    _ui->setupUi(this);

    _guitarTab = nullptr;
    _buildTabs();
}

SettingsWindow::~SettingsWindow()
{
    _cleanTabs();
    delete _ui;
}

QTabWidget *SettingsWindow::tabs()
{
    return findChild<QTabWidget *>("tabs");
}

void SettingsWindow::_buildTabs()
{
    QTabWidget *tabs = this->tabs();

    _cleanTabs();

    _driverTab = new Setting::Driver(_settings, _eventManager);
    _guitarTab = new Setting::Guitar(_settings, _eventManager);

    tabs->addTab(_guitarTab, "Guitar");
    tabs->addTab(_driverTab, "Driver");
}

void SettingsWindow::_cleanTabs()
{
    QTabWidget *tabs = this->tabs();

    while (tabs->count() > 0) {
        tabs->removeTab(0);
    }

    if (_driverTab != nullptr) {
        delete _driverTab;
        _driverTab = nullptr;
    }

    if (_guitarTab != nullptr) {
        delete _guitarTab;
        _guitarTab = nullptr;
    }
}

void SettingsWindow::on_SettingsWindow_accepted()
{
    _guitarTab->save();
    _driverTab->save();
}


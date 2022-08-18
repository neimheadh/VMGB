#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QSettings>
#include <QTabWidget>
#include "eventmanager.h"

namespace Ui {
class SettingsWindow;
}

namespace Setting {
class Driver;
class Guitar;
}

namespace Guitar {
class Board;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QSettings *settings, Guitar::Board *guitar = nullptr, EventManager *eventManager = nullptr, QWidget *parent = nullptr);
    ~SettingsWindow();

    QTabWidget *tabs();

private slots:
    void on_SettingsWindow_accepted();

private:
    EventManager *_eventManager;
    Guitar::Board *_guitar;
    Ui::SettingsWindow *_ui;
    QSettings *_settings;

    Setting::Driver *_driverTab;
    Setting::Guitar *_guitarTab;

    void _buildTabs();
    void _cleanTabs();
};

#endif // SETTINGSWINDOW_H

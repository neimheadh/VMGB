#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QDialog>
#include <QSettings>
#include <QVBoxLayout>
#include "setting/guitarsetting.h"

namespace Ui {
class SettingWindow;
}

class SettingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingWindow(QSettings *setting, QWidget *parent = nullptr);
    ~SettingWindow();

    GuitarSetting *guitar();

private slots:
    void on_SettingWindow_accepted();

private:
    GuitarSetting *_guitar_ui;
    QSettings *_settings;
    Ui::SettingWindow *_ui;
    QVBoxLayout *_tab_guitar_layout;
    QVBoxLayout *_tab_midi_layout;

    QVBoxLayout *_initLayout(const char *childName);
};

#endif // SETTINGWINDOW_H

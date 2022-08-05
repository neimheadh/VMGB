#include "settingwindow.h"
#include "ui_settingwindow.h"

SettingWindow::SettingWindow(QSettings *settings, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::SettingWindow)
{
    _ui->setupUi(this);

    _settings = settings;
    _tab_guitar_layout = _initLayout("tab_guitar");
    _tab_midi_layout = _initLayout("tab_midi");

    _guitar_ui = new GuitarSetting(_settings, findChild<QWidget *>("tab_guitar"));
}

SettingWindow::~SettingWindow()
{
    delete _tab_guitar_layout;
    delete _tab_midi_layout;
    delete _ui;
}

GuitarSetting *SettingWindow::guitar()
{
    return this->findChild<GuitarSetting *>("tab_guitar");
}

QVBoxLayout *SettingWindow::_initLayout(const char *childName)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);

    findChild<QWidget *>(childName)->setLayout(layout);

    return layout;
}

void SettingWindow::on_SettingWindow_accepted()
{
    _guitar_ui->save();
}


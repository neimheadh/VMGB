#include "setting/driver.h"
#include "ui_setting_driver.h"

#include <QMessageBox>
#include "midi/driver/alsa.h"
#include "setting/setting.h"

using namespace Setting;

const char *Driver::DEFAULT_DRIVER = MIDI::Driver::Alsa::NAME;
const char *Driver::DEFAULT_NAME = "VMGB";

Driver::Driver(QSettings *settings, EventManager *em, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::SettingDriver)
{
    _em = em;
    _settings = settings;

    _ui->setupUi(this);
}

Driver::~Driver()
{
    delete _ui;
}

void Driver::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    QComboBox *drivers = _drivers();

    drivers->clear();
    drivers->addItem(MIDI::Driver::Alsa::NAME);

    _applicationName()->setText(name(_settings));
    drivers->setCurrentText(driver(_settings));
    _channel()->setValue(channel(_settings));
}

unsigned char Driver::channel(QSettings *settings)
{
    unsigned char channel;

    settings->beginGroup(SETTING_GROUP_DRIVER);
    channel = settings->value(SETTING_DRIVER_CHANNEL, QString::number(DEFAULT_CHANNEL)).toUInt();
    settings->endGroup();

    return channel;
}

QString Driver::driver(QSettings *settings)
{
    QString driver;

    settings->beginGroup(SETTING_GROUP_DRIVER);
    driver = settings->value(SETTING_DRIVER_DRIVER, DEFAULT_DRIVER).toString();
    settings->endGroup();

    return driver;
}

QString Driver::name(QSettings *settings)
{
    QString name;

    settings->beginGroup(SETTING_GROUP_DRIVER);
    name = settings->value(SETTING_DRIVER_NAME, DEFAULT_NAME).toString();
    settings->endGroup();

    return name;
}

void Driver::save()
{
    if (_changed) {
        _settings->beginGroup(SETTING_GROUP_DRIVER);
        _settings->setValue(SETTING_DRIVER_CHANNEL, _channel()->value());
        _settings->setValue(SETTING_DRIVER_DRIVER, _drivers()->currentText());
        _settings->setValue(SETTING_DRIVER_NAME, _applicationName()->text());
        _settings->endGroup();

        _changed = false;

        QMessageBox msgBox;
        msgBox.setText("Your driver setting changes will take effect after restart.");
        msgBox.exec();
    }
}

QLineEdit *Driver::_applicationName()
{
    return findChild<QLineEdit *>("application_name");
}

QSpinBox *Driver::_channel()
{
    return findChild<QSpinBox *>("channel");
}

QComboBox *Driver::_drivers()
{
    return findChild<QComboBox *>("driver_select");
}

void Driver::on_driver_select_currentTextChanged(const QString &arg1)
{
    _changed = true;
}


void Driver::on_application_name_textChanged(const QString &arg1)
{
    _changed = true;
}


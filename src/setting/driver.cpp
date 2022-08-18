#include "setting/driver.h"
#include "ui_setting_driver.h"

using namespace Setting;

Driver::Driver(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::SettingDriver)
{
    _ui->setupUi(this);
}

Driver::~Driver()
{
    delete _ui;
}

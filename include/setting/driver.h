#ifndef DRIVER_H
#define DRIVER_H

#include <QWidget>

namespace Ui {
class SettingDriver;
}

namespace Setting {
class Driver : public QWidget
{
    Q_OBJECT

public:
    explicit Driver(QWidget *parent = nullptr);
    ~Driver();

private:
    Ui::SettingDriver *_ui;
};
}

#endif // DRIVER_H

#ifndef DRIVER_H
#define DRIVER_H

#include <QComboBox>
#include <QLineEdit>
#include <QSettings>
#include <QWidget>
#include "eventmanager.h"

namespace Ui {
class SettingDriver;
}

namespace Setting {
class Driver : public QWidget
{
    Q_OBJECT

public:
    static const char *DEFAULT_DRIVER;
    static const char *DEFAULT_NAME;

    static QString driver(QSettings *settings);
    static QString name(QSettings *settings);

    explicit Driver(QSettings *settings, EventManager *em = nullptr, QWidget *parent = nullptr);
    ~Driver();

    void save();

    void showEvent(QShowEvent *event);
private slots:
    void on_driver_select_currentTextChanged(const QString &arg1);

    void on_application_name_textChanged(const QString &arg1);

private:
    EventManager *_em;
    QSettings *_settings;
    Ui::SettingDriver *_ui;

    bool _changed = false;

    QLineEdit *_applicationName();
    QComboBox *_drivers();
};
}

#endif // DRIVER_H

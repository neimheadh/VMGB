#ifndef GUITARSETTING_H
#define GUITARSETTING_H

#include <QSettings>
#include <QSpinBox>
#include <QTextEdit>
#include <QWidget>

namespace Ui {
class GuitarSetting;
}

class GuitarSetting : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Guitar application settings group.
     */
    static const char SETTINGS_GROUP[];

    static QString tuningToSetting(unsigned char strings, const unsigned char *tuning);
    static unsigned char *settingToTuning(QString setting);

    explicit GuitarSetting(QSettings *settings, QWidget *parent = nullptr);
    ~GuitarSetting();

    void save();

    void showEvent(QShowEvent *event);

    QSpinBox *frets();
    QSpinBox *strings();
    QTextEdit *tuning();

private:
    QSettings *_settings;
    Ui::GuitarSetting *_ui;
};

#endif // GUITARSETTING_H

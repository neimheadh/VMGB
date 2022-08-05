#include "setting/guitarsetting.h"
#include "ui_setting_guitar.h"

#include <string.h>
#include "keynote.h"
#include "guitarboard.h"

const char GuitarSetting::SETTINGS_GROUP[] = "guitar";

GuitarSetting::GuitarSetting(QSettings *settings, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GuitarSetting)
{
    _ui->setupUi(this);

    _settings = settings;
}

GuitarSetting::~GuitarSetting()
{
    delete _ui;
}

unsigned char *GuitarSetting::settingToTuning(QString setting)
{
    QStringList strings = setting.split(",");
    unsigned char *tuning = new unsigned char[strings.length()];
    unsigned char note;

    for (unsigned char i = 0; i < strings.length(); ++i) {
        for (note = 0; note <= KeyNote::MIDI_MAX; note++) {
            QString str = strings[i];
            int cmp = strcmp(KeyNote::MIDI_MAP[note], str.toStdString().data());
            if (cmp == 0) {
                break;
            }
        }
        tuning[i] = note;
    }

    return tuning;
}

QString GuitarSetting::tuningToSetting(unsigned char strings, const unsigned char *tuning)
{
    QString setting;

    for (unsigned char i = 0; i < strings; i++) {
        if (i > 0) {
            setting += ",";
        }
        setting += KeyNote::MIDI_MAP[tuning[i]];
    }

    return setting;
}

void GuitarSetting::save()
{
    unsigned char _frets = _settings->value("frets", QString::number(GuitarBoard::DEFAULT_FRETS)).toString().toUInt();
    unsigned char _strings = _settings->value("strings", QString::number(GuitarBoard::DEFAULT_FRETS)).toString().toUInt();
    QString _tuning = _settings->value("tuning", GuitarSetting::tuningToSetting(GuitarBoard::DEFAULT_STRINGS, GuitarBoard::DEFAULT_TUNING)).toString();
    unsigned char frets = this->frets()->value();
    unsigned char strings = this->strings()->value();
    QString tuning = this->tuning()->toPlainText();

    if (_frets != frets
            || _strings != strings
            || _tuning != tuning
    ) {
        _settings->beginGroup(GuitarSetting::SETTINGS_GROUP);
        _settings->setValue("frets", QString::number(frets));
        _settings->setValue("strings", QString::number(strings));
        _settings->setValue("tuning", tuning);
        _settings->endGroup();
    }
}

void GuitarSetting::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    _settings->beginGroup(GuitarSetting::SETTINGS_GROUP);
    frets()->setValue(_settings->value("frets", QString::number(GuitarBoard::DEFAULT_FRETS)).toString().toUInt());
    strings()->setValue(_settings->value("strings", QString::number(GuitarBoard::DEFAULT_STRINGS)).toString().toUInt());
    tuning()->setText(_settings->value("tuning", GuitarSetting::tuningToSetting(GuitarBoard::DEFAULT_STRINGS, GuitarBoard::DEFAULT_TUNING)).toString());
    _settings->endGroup();
}

QSpinBox *GuitarSetting::frets()
{
    return findChild<QSpinBox *>("frets_input");
}

QSpinBox *GuitarSetting::strings()
{
    return findChild<QSpinBox *>("strings_input");
}

QTextEdit *GuitarSetting::tuning()
{
    return findChild<QTextEdit *>("tuning_input");
}

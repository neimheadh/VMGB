#include "setting/guitar.h"
#include "ui_setting_guitar.h"

#include <QAction>
#include <QFormLayout>
#include <QLineEdit>
#include <string.h>
#include "midi/midi.h"
#include "setting/setting.h"

using namespace MIDI;
using namespace Setting;

const unsigned char Guitar::DEFAULT_TUNING[] = {40, 45, 50, 55, 59, 64};

Guitar::Guitar(QSettings *settings, EventManager *eventManager, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::SettingGuitar)
{
    _settings = settings;
    _eventManager = eventManager;

    _ui->setupUi(this);
}

Guitar::~Guitar()
{
    delete _ui;

    _clearTuning();
    if (_tuning != nullptr) {
        delete[] _tuning;
    }
}

unsigned char *Guitar::settingToTuning(QString setting)
{
    QStringList strings = setting.split(",");
    unsigned char *tuning = new unsigned char[strings.length()];
    unsigned char note;

    for (unsigned char i = 0; i < strings.length(); ++i) {
        for (note = 0; note <= MIDI_NOTE_MAX; note++) {
            QString str = strings[i];
            int cmp = strcmp(MIDI_NOTE_MAP[note], str.toStdString().data());
            if (cmp == 0) {
                break;
            }
        }
        tuning[i] = note;
    }

    return tuning;
}

unsigned char *Guitar::settingToTuning(QVariant setting)
{
    return settingToTuning(setting.toString());
}

QString Guitar::tuningToSetting(unsigned char strings, const unsigned char *tuning)
{
    QString setting;

    for (unsigned char i = 0; i < strings; i++) {
        if (i > 0) {
            setting += ",";
        }
        setting += MIDI_NOTE_MAP[tuning[i]];
    }

    return setting;
}

void Guitar::save()
{
    if (_changed) {
        _settings->beginGroup(SETTING_GROUP_GUITAR);
        _settings->setValue(SETTING_GUITAR_FRETS, QString::number(_frets));
        _settings->setValue(SETTING_GUITAR_STRINGS, QString::number(_strings));
        _settings->setValue(SETTING_GUITAR_TUNING, tuningToSetting(_strings, _tuning));
        _settings->endGroup();

        if (_eventManager != nullptr) {
            _eventManager->dispatch(EVENT_SAVE_GUITAR, (void *) _settings);
        }
    }
}

unsigned char Guitar::frets()
{
    return _frets;
}

unsigned char Guitar::frets(QSettings *settings)
{
    unsigned char frets;

    settings->beginGroup(SETTING_GROUP_GUITAR);
    frets = settings->value(SETTING_GUITAR_FRETS, QString::number(DEFAULT_FRETS)).toString().toUInt();
    settings->endGroup();

    return frets;
}

unsigned char Guitar::strings()
{
    return _strings;
}

unsigned char Guitar::strings(QSettings *settings)
{
    unsigned char strings;

    settings->beginGroup(SETTING_GROUP_GUITAR);
    strings = settings->value(SETTING_GUITAR_STRINGS, QString::number(DEFAULT_STRINGS)).toString().toUInt();
    settings->endGroup();

    return strings;
}

const unsigned char *Guitar::tuning()
{
    return _tuning;
}

unsigned char *Guitar::tuning(QSettings *settings)
{
    unsigned char *tuning;

    settings->beginGroup(SETTING_GROUP_GUITAR);
    tuning = settingToTuning(settings->value(SETTING_GUITAR_TUNING, tuningToSetting(DEFAULT_STRINGS, DEFAULT_TUNING)));
    settings->endGroup();

    return tuning;
}

void Guitar::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    _loadSetting();
    _fretsInput()->setValue(_frets);
    _stringsInput()->setValue(_strings);
    _changed = false;
}

void Guitar::on_frets_input_valueChanged(int frets)
{
    _frets = frets;
    _changed = true;
}

void Guitar::on_strings_input_valueChanged(int strings)
{
    _strings = strings;
    _buildTuning();
    _changed = true;
}

QSpinBox *Guitar::_fretsInput()
{
    return findChild<QSpinBox *>("frets_input");
}

QSpinBox *Guitar::_stringsInput()
{
    return findChild<QSpinBox *>("strings_input");
}

QWidget *Guitar::_tuningArea()
{
    return findChild<QWidget *>("tuning_area");
}

void Guitar::_buildTuning()
{
    QFormLayout *layout = (QFormLayout *) _tuningArea()->layout();

    _clearTuning();
    _tuningInputs = new MidiNoteInput *[_strings];
    _tuningLabels = new QLabel *[_strings];

    for (unsigned char i = 0; i < _strings; i++) {
        _tuningLabels[i] = new QLabel("String " + QString::number(i + 1));
        _tuningInputs[i] = new MidiNoteInput(_tuning[i]);
        _tuningInputs[i]->setObjectName(QString::number(i));

        connect(_tuningInputs[i], &MidiNoteInput::valueChanged, this, &Guitar::_tuningChange);
        layout->addRow(_tuningLabels[i], _tuningInputs[i]);
    }
}

void Guitar::_clearTuning()
{
    QFormLayout *layout = (QFormLayout *) _tuningArea()->layout();

    while(layout->rowCount() > 0) {
        layout->removeRow(0);
    }

    delete[] _tuningInputs;
    _tuningInputs = nullptr;

    delete[] _tuningLabels;
    _tuningLabels = nullptr;
}

void Guitar::_loadSetting()
{
    if (_tuning != nullptr) {
        delete[] _tuning;
        _tuning = nullptr;
    }

    _frets = frets(_settings);
    _strings = strings(_settings);
    _tuning = tuning(_settings);
}

void Guitar::_tuningChange(Setting::MidiNoteInput *input, unsigned char value)
{
    unsigned char string = input->objectName().toUInt();

    if (string <= _strings) {
        _tuning[string] = value;
        _changed = true;
    }
}

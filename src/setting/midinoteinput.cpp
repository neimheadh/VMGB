#include "setting/midinoteinput.h"
#include "ui_setting_midinoteinput.h"

#include <QKeyEvent>
#include "midi/midi.h"

using namespace MIDI;
using namespace Setting;

MidiNoteInput::MidiNoteInput(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::MidiNoteInput)
{
    _ui->setupUi(this);
}

MidiNoteInput::MidiNoteInput(unsigned char value, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::MidiNoteInput)
{
    _ui->setupUi(this);
    _value = value;
}

MidiNoteInput::~MidiNoteInput()
{
    delete _ui;
}

void MidiNoteInput::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);

    switch (event->key()) {
    case 16777235: // Up arrow
    case Qt::Key_Plus:
        if (_value < MIDI_NOTE_MAX) {
            setValue(_value + 1);
        }
        break;
    case 16777237: // Down arrow
    case Qt::Key_Minus:
        if (_value > 0) {
            setValue(_value - 1);
        }
        break;
    }
}

void MidiNoteInput::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    setValue(_value);
}

QLineEdit *MidiNoteInput::input()
{
    return findChild<QLineEdit *>("input");
}

void MidiNoteInput::setValue(unsigned char value)
{
    if (value > MIDI_NOTE_MAX) {
        value = MIDI_NOTE_MAX;
    }
    _value = value;
    input()->setText(MIDI_NOTE_MAP[_value]);
    emit valueChanged(this, value);
}

unsigned char MidiNoteInput::value()
{
    return _value;
}

void MidiNoteInput::on_increase_clicked()
{
    if (_value < MIDI_NOTE_MAX) {
        setValue(_value + 1);
    }
}

void MidiNoteInput::on_decrease_clicked()
{
    if (_value > 0) {
        setValue(_value - 1);
    }
}

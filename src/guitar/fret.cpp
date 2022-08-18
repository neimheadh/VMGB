#include "guitar/fret.h"
#include "ui_guitar_fret.h"

#include <iostream>
#include <QTime>
#include "midi/midi.h"

using namespace MIDI;
using namespace Guitar;


const char *Fret::EVENT_FRET_PRESS = "Guitar::Board::EVENT_FRET_PRESS";
const char *Fret::EVENT_FRET_RELEASE = "Guitar::Board::EVENT_FRET_RELEASE";

Fret::Fret(unsigned char note, EventManager *eventManager, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GuitarFret)
{
    _ui->setupUi(this);

    _note = note > MIDI_NOTE_MAX ? MIDI_NOTE_MAX : note;
    _em = eventManager;

    button()->setText(MIDI_NOTE_MAP[note]);
}

Fret::~Fret()
{
    delete _ui;
}

void Fret::play()
{
    button()->setDown(true);
}

void Fret::release()
{
    button()->setDown(false);
}

void Fret::setFirst()
{
    QPushButton *button = this->button();

    button->setText("");
    button->setStyleSheet("QPushButton {background: #ccc;} QPushButton:pressed {background: #f00;}");
}

QPushButton *Fret::button()
{
    return this->findChild<QPushButton *>("button");
}

unsigned char Fret::note()
{
    return _note;
}

void Fret::on_button_pressed()
{
    struct MIDI::MidiEvent event;

    if (_em) {
        _em->dispatch(EVENT_FRET_PRESS, &_note);
        event = {
            .type = MIDI::MidiEventType::NOTE_ON,
            .value = _note
        };
        _em->dispatch(MIDI::EVENT_MIDI_SEND, &event);
    }

    play();
}

void Fret::on_button_released()
{
    struct MIDI::MidiEvent event;

    if (_em) {
        _em->dispatch(EVENT_FRET_RELEASE, &_note);
        event = {
            .type = MIDI::MidiEventType::NOTE_OFF,
            .value = _note
        };
        _em->dispatch(MIDI::EVENT_MIDI_SEND, &event);
    }

    release();
}


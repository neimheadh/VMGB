#include "guitar/string.h"
#include "ui_guitar_string.h"

#include <QPushButton>
#include <iostream>
#include "midi/midi.h"

using namespace MIDI;
using namespace Guitar;

String::String(QSettings *settings, unsigned char frets, unsigned char note, EventManager *em, QWidget *parent)
    : QWidget{parent},
      _ui(new Ui::GuitarString)
{
    _ui->setupUi(this);

    _frets = frets;
    _em = em;
    _note = note > MIDI_NOTE_MAX ? MIDI_NOTE_MAX : note;
    _settings = settings;

    _build();
}

String::~String()
{
    delete _ui;
}

unsigned char String::frets()
{
    return _frets;
}

unsigned char String::note()
{
    return _note;
}

int String::playing()
{
    return _playing;
}

bool String::canPlay(unsigned char note)
{
    return note >= _note && note < _note + _frets;
}

void String::play(unsigned char fret)
{
    release();
    if (fret < _frets) {
        _playing = fret;
        this->fret(fret)->play();
    }
}

void String::playMidi(unsigned char note)
{
    if (canPlay(note)) {
        play(note - _note);
    }
}

void String::release()
{
    if (_playing >= 0) {
        _release(_playing);
        _playing = -1;
    }
}

Fret *String::fret(unsigned char fret)
{
    return this->findChild<Fret *>(QString::number(fret));
}

Fret *String::fret(QString fret)
{
    return this->findChild<Fret *>(fret);
}

QLabel *String::label()
{
    return this->findChild<QLabel *>("label");
}

void String::_build()
{
    Fret *Fret = nullptr;

    for (unsigned int i = 0; i < _frets; i++) {
        Fret = new Guitar::Fret(_settings, _note + i, _em, this);
        Fret->setObjectName(QString::number(i));

        if (i == 0) {
            Fret->setFirst();
        }

        this->layout()->addWidget(Fret);
    }

    label()->setText(MIDI_NOTE_MAP[_note]);
}

void String::_release(unsigned char fret)
{
    if (fret < _frets) {
        this->fret(fret)->release();
    }
}

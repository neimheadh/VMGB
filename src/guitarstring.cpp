#include "guitarstring.h"
#include "ui_guitarstring.h"

#include <QPushButton>
#include <iostream>


GuitarString::GuitarString(QWidget *parent, unsigned char frets, unsigned char note)
    : QWidget{parent},
      _ui(new Ui::GuitarString)
{
    _ui->setupUi(this);

    _frets = frets;
    _note = note > KeyNote::MIDI_MAX ? KeyNote::MIDI_MAX : note;

    _build();
}

GuitarString::~GuitarString()
{
    delete _ui;
}

unsigned char GuitarString::frets()
{
    return _frets;
}

unsigned char GuitarString::note()
{
    return _note;
}

int GuitarString::playing()
{
    return _playing;
}

bool GuitarString::canPlay(unsigned char note)
{
    return note >= _note && note < _note + _frets;
}

GuitarString *GuitarString::play(unsigned char fret)
{
    release();
    if (fret < _frets) {
        _playing = fret;
        this->key(fret)->play();
    }

    return this;
}

GuitarString *GuitarString::playMidi(unsigned char note)
{
    if (canPlay(note)) {
        play(note - _note);
    }

    return this;
}

GuitarString *GuitarString::release()
{
    if (_playing >= 0) {
        _release(_playing);
        _playing = -1;
    }

    return this;
}

QLabel *GuitarString::label()
{
    return this->findChild<QLabel *>("label");
}

KeyNote *GuitarString::key(unsigned char key)
{
    return this->findChild<KeyNote *>(QString::number(key));
}

KeyNote *GuitarString::key(QString key)
{
    return this->findChild<KeyNote *>(key);
}

GuitarString *GuitarString::_build()
{
    KeyNote *keynote = nullptr;

    for (unsigned int i = 0; i < _frets; i++) {
        keynote = new KeyNote(this, _note + i);
        keynote->setObjectName(QString::number(i));

        if (i == 0) {
            keynote->setFirst();
        }

        this->layout()->addWidget(keynote);
    }

    label()->setText(KeyNote::MIDI_MAP[_note]);

    return this;
}

void GuitarString::_release(unsigned char fret)
{
    if (fret < _frets) {
        key(fret)->release();
    }
}

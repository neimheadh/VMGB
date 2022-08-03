#include "guitarboard.h"
#include "ui_guitarboard.h"

// std::sort
#include <algorithm>
#include <iostream>

const unsigned char GuitarBoard::DEFAULT_TUNING[] = {40, 45, 50, 55, 59, 64};

GuitarBoard::GuitarBoard(QWidget *parent, unsigned char frets, unsigned char strings, const unsigned char tuning[]) :
    QWidget(parent),
    _ui(new Ui::GuitarBoard)
{
    _ui->setupUi(this);

    _frets = frets;

    _setTuning(strings, tuning == nullptr ? GuitarBoard::DEFAULT_TUNING : tuning);
    _build();
}

GuitarBoard::~GuitarBoard()
{
    delete _ui;

    if (_tuning != nullptr) {
        delete[] _tuning;
        _tuning = nullptr;
    }

    if (_midi_strings != nullptr) {
        delete[] _midi_strings;
        _tuning = nullptr;
    }
}

unsigned char GuitarBoard::frets()
{
    return _frets;
}

unsigned char GuitarBoard::strings()
{
    return _strings;
}

const unsigned char *GuitarBoard::tuning()
{
    return _tuning;
}

bool GuitarBoard::canPlay(unsigned char note)
{
    for (unsigned char i = 0; i < _strings; i++) {
        if (string(i)->canPlay(note)) {
            return true;
        }
    }

    return false;
}

GuitarBoard *GuitarBoard::play(unsigned char string, unsigned char fret)
{
    if (string < _strings) {
        this->string(string)->play(fret);
    }

    return this;
}

GuitarBoard *GuitarBoard::play(int chord[], unsigned char notes)
{
    release();

    for (unsigned char i = 0; i < notes; i++) {
        if (i < _strings && chord[i] >= 0) {
            play(i, chord[i]);
        }
    }

    return this;
}

GuitarBoard *GuitarBoard::playMidi(unsigned char note)
{
    GuitarString *string;

    if (canPlay(note)) {
        for (unsigned char i = 0; i < _strings; i++) {
            string = this->string(i);

            if (string->playing() == -1 && string->canPlay(note)) {
                string->playMidi(note);
                _midi_strings[i] = note;
                break;
            }
        }
    }

    return this;
}

GuitarBoard *GuitarBoard::playMidi(const unsigned char notes[], unsigned char length)
{
    unsigned char sorted[length];

    release();

    for (unsigned char i = 0; i < length; i++) sorted[i] = notes[i];

    std::sort(sorted, sorted + length);

    for (unsigned char i = 0; i < length; i++) {
        playMidi(sorted[i]);
    }

    return this;
}

GuitarBoard *GuitarBoard::release()
{
    for (unsigned char string = 0; string < _strings; string++) {
        this->release(string);
    }

    return this;
}

GuitarBoard *GuitarBoard::release(unsigned char string)
{
    if (string < _strings) {
        this->string(string)->release();
    }

    return this;
}

GuitarBoard *GuitarBoard::releaseMidi(unsigned char note)
{
    for (unsigned char i = 0; i < _strings; i++) {
        if (_midi_strings[i] == note) {
            string(i)->release();
            _midi_strings[i] = -1;
        }
    }

    return this;
}

GuitarBoard *GuitarBoard::resize()
{

    stringFrame()->resize(width(), height());
    dotFrame()->resize(stringFrame()->width() - 30, stringFrame()->height());

    stringFrame()->move(0, 0);
    dotFrame()->move(30, 0);

    dotFrame()->setAttribute(Qt::WA_TransparentForMouseEvents);

    return this;
}


QFrame *GuitarBoard::dotFrame()
{
    return this->findChild<QFrame *>("dot");
}

GuitarString *GuitarBoard::string(QString string)
{
    return stringFrame()->findChild<GuitarString *>(string);
}

GuitarString *GuitarBoard::string(unsigned char string)
{
    return stringFrame()->findChild<GuitarString *>(QString::number(string));
}

QFrame *GuitarBoard::stringFrame()
{
    return this->findChild<QFrame *>("string");
}

GuitarBoard *GuitarBoard::_build()
{
    GuitarString *guitarstring = nullptr;
    QGridLayout *dotBoard = (QGridLayout *) dotFrame()->layout();
    QLabel *dot;

    if (_midi_strings != nullptr) {
        delete[] _midi_strings;
    }

    _midi_strings = new int[_strings];

    for (int i = _strings - 1; i >= 0; i--) {
        guitarstring = new GuitarString(this, _frets, _tuning[i]);
        guitarstring->setObjectName(QString::number(i));
        stringFrame()->layout()->addWidget(guitarstring);
        _midi_strings[i] = -1;
    }

    /* @fixme The dots doesn't fit exactly on very big board */
    for (unsigned char i = 0; i < _frets; i++) {
        for (unsigned char j = 0; j < 3; j++) {
            dot = new QLabel();
            dot->setText("•");
            dot->setAlignment(Qt::AlignmentFlag::AlignCenter | Qt::AlignmentFlag::AlignHCenter);
            if (((i+1) % 2 == 0 && i > 2 && (i+1) % 12 && (i-1) % 12 && j == 1)
              || (i && i % 12 == 0 && j != 1)
            ) {
                dot->setStyleSheet("font-size: 50px;padding-top:10px;color: rgba(255, 255, 255, .8)");
            } else {
                dot->setStyleSheet("font-size: 50px;color: transparent");
            }
            dot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            dotBoard->addWidget(dot, j, i);
        }
    }

    resize();

    return this;
}

GuitarBoard *GuitarBoard::_setTuning(unsigned char strings, const unsigned char tuning[])
{
    _strings = strings;

    if (_tuning != nullptr) {
        delete[] _tuning;
        _tuning = nullptr;
    }

    _tuning = new unsigned char[strings];

    for (int i = 0; i < strings; i++) {
        _tuning[i] = tuning[i];
    }

    return this;
}

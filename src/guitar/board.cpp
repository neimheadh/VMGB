#include "guitar/board.h"
#include "ui_guitar_board.h"

// std::sort
#include <algorithm>
#include <iostream>

#include "midi/midi.h"
#include "setting/guitar.h"

using namespace Guitar;

void _on_midi_input(Board *board, const MIDI::MidiEvent *event) {
    switch (event->type) {
    case MIDI::MidiEventType::NOTE_OFF:
        board->releaseMidi(event->value);
        break;
    case MIDI::MidiEventType::NOTE_ON:
        board->playMidi(event->value);
        break;
    }
}

void _on_setting_save(Board *board, QSettings *settings) {
    board->build();
}

Board::Board(QSettings *settings, EventManager *eventManager, QWidget *parent):
    QWidget(parent),
    _ui(new Ui::GuitarBoard)
{
    _frets = 0;
    _strings = 0;
    _tuning = nullptr;

    _eventManager = eventManager;
    _settings = settings;
    _ui->setupUi(this);
}

Board::~Board()
{
    _clear();
    delete _ui;

    if (_eventManager) {
        if (_eh_midi > 0) {
            _eventManager->unsubscribe(MIDI::EVENT_MIDI_INPUT, _eh_midi);
            _eh_midi = 0;
        }

        if (_eh_setting > 0) {
            _eventManager->unsubscribe(Setting::Guitar::EVENT_SAVE, _eh_setting);
            _eh_setting = 0;
        }
    }
}

unsigned char Board::frets()
{
    return _frets;
}

unsigned char Board::strings()
{
    return _strings;
}

const unsigned char *Board::tuning()
{
    return _tuning;
}

void Board::build()
{
    QLayout *stringBoard = stringFrame()->layout();
    QGridLayout *dotBoard = (QGridLayout *) dotFrame()->layout();

    _clear();
    _loadSettings(_settings);
    _midi_strings = new int[_strings];
    _string_widgets = new String*[_strings];
    _dot_widgets = new QLabel*[_frets * 3];

    for (int i = _strings - 1; i >= 0; i--) {
        _string_widgets[i] = new String(_frets, _tuning[i], _eventManager, this);
        _string_widgets[i]->setObjectName(QString::number(i));
        stringBoard->addWidget(_string_widgets[i]);
        _midi_strings[i] = -1;
    }

    /* @fixme The dots doesn't fit exactly on very big board */
    for (unsigned char i = 0; i < _frets; i++) {
        for (unsigned char j = 0; j < 3; j++) {
            _dot_widgets[j + i * 3] = new QLabel();
            _dot_widgets[j + i * 3]->setText("•");
            _dot_widgets[j + i * 3]->setAlignment(Qt::AlignmentFlag::AlignCenter | Qt::AlignmentFlag::AlignHCenter);
            if (((i+1) % 2 == 0 && i > 2 && (i+1) % 12 && (i-1) % 12 && j == 1)
              || (i && i % 12 == 0 && j != 1)
            ) {
                _dot_widgets[j + i * 3]->setStyleSheet("font-size: 50px;padding-top:10px;color: rgba(255, 255, 255, .8)");
            } else {
                _dot_widgets[j + i * 3]->setStyleSheet("font-size: 50px;color: transparent");
            }
            _dot_widgets[j + i * 3]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            dotBoard->addWidget(_dot_widgets[j + i * 3], j, i);
        }
    }

    resize();
}

bool Board::canPlay(unsigned char note)
{
    for (unsigned char i = 0; i < _strings; i++) {
        if (string(i)->canPlay(note)) {
            return true;
        }
    }

    return false;
}

void Board::play(unsigned char string, unsigned char fret)
{
    if (string < _strings) {
        this->string(string)->play(fret);
    }
}

void Board::play(int chord[], unsigned char notes)
{
    release();

    for (unsigned char i = 0; i < notes; i++) {
        if (i < _strings && chord[i] >= 0) {
            play(i, chord[i]);
        }
    }
}

void Board::playMidi(unsigned char note)
{
    String *string;

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
}

void Board::playMidi(const unsigned char notes[], unsigned char length)
{
    unsigned char sorted[length];

    release();

    for (unsigned char i = 0; i < length; i++) sorted[i] = notes[i];

    std::sort(sorted, sorted + length);

    for (unsigned char i = 0; i < length; i++) {
        playMidi(sorted[i]);
    }
}

void Board::release()
{
    for (unsigned char string = 0; string < _strings; string++) {
        this->release(string);
    }
}

void Board::release(unsigned char string)
{
    if (string < _strings) {
        this->string(string)->release();
    }
}

void Board::releaseMidi(unsigned char note)
{
    for (unsigned char i = 0; i < _strings; i++) {
        if (_midi_strings[i] == note) {
            string(i)->release();
            _midi_strings[i] = -1;
        }
    }
}

void Board::resize()
{
    stringFrame()->resize(width(), height());
    dotFrame()->resize(stringFrame()->width() - 30, stringFrame()->height());

    stringFrame()->move(0, 0);
    dotFrame()->move(30, 0);

    dotFrame()->setAttribute(Qt::WA_TransparentForMouseEvents);
}


QFrame *Board::dotFrame()
{
    return this->findChild<QFrame *>("dot");
}

String *Board::string(QString string)
{
    return stringFrame()->findChild<String *>(string);
}

String *Board::string(unsigned char string)
{
    return stringFrame()->findChild<String *>(QString::number(string));
}

QFrame *Board::stringFrame()
{
    return this->findChild<QFrame *>("string");
}

void Board::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    build();

    if (_eventManager != nullptr) {
        _eh_midi = _eventManager->subscribe(MIDI::EVENT_MIDI_INPUT, (event_handler_t *) &_on_midi_input, this);
        _eh_setting = _eventManager->subscribe(Setting::Guitar::EVENT_SAVE, (event_handler_t *) &_on_setting_save, this);
    }
}

void Board::_clear()
{
    QLayout *stringBoard = stringFrame()->layout();
    QLayout *dotBoard = dotFrame()->layout();

    if (_midi_strings != nullptr) {
        delete[] _midi_strings;
        _midi_strings = nullptr;
    }

    if (_string_widgets != nullptr) {
        for (unsigned char i = 0; i < _strings; i++) {
            stringBoard->removeWidget(_string_widgets[i]);
            delete _string_widgets[i];
        }

        delete[] _string_widgets;
        _string_widgets = nullptr;
    }

    if (_dot_widgets != nullptr) {
        for (unsigned char i = 0; i < _frets; i++) {
            for (unsigned char j = 0; j < 3; j++) {
                dotBoard->removeWidget(_dot_widgets[j + i * 3]);
                delete _dot_widgets[j + i * 3];
            }
        }

        delete[] _dot_widgets;
        _dot_widgets = nullptr;
    }

    _strings = 0;
    _frets = 0;
}

void Board::_loadSettings(QSettings *settings)
{
    unsigned char strings = Setting::Guitar::strings(settings);
    unsigned char *tuning = Setting::Guitar::tuning(settings);

    _frets = Setting::Guitar::frets(settings);
    _setTuning(strings, tuning);
    delete[] tuning;
}

void Board::_setTuning(unsigned char strings, const unsigned char tuning[])
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
}

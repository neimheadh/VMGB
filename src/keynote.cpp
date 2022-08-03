#include "keynote.h"
#include "ui_keynote.h"

#include <iostream>
#include <QTime>

const char *KeyNote::MIDI_MAP[] = {
    "C-2", "C#-2", "D-2", "D#-2", "E-2", "F-2", "F#-2", "G-2", "G#-2", "A-2", "A#-2", "B-2",
    "C-1", "C#-1", "D-1", "D#-1", "E-1", "F-1", "F#-1", "G-1", "G#-1", "A-1", "A#-1", "B-1",
    "C0",  "C#0",  "D0",  "D#0",  "E0",  "F0",  "F#0",  "G0",  "G#0",  "A0",  "A#0",  "B0",
    "C1",  "C#1",  "D1",  "D#1",  "E1",  "F1",  "F#1",  "G1",  "G#1",  "A1",  "A#1",  "B1",
    "C2",  "C#2",  "D2",  "D#2",  "E2",  "F2",  "F#2",  "G2",  "G#2",  "A2",  "A#2",  "B2",
    "C3",  "C#3",  "D3",  "D#3",  "E3",  "F3",  "F#3",  "G3",  "G#3",  "A3",  "A#3",  "B3",
    "C4",  "C#4",  "D4",  "D#4",  "E4",  "F4",  "F#4",  "G4",  "G#4",  "A4",  "A#4",  "B4",
    "C5",  "C#5",  "D5",  "D#5",  "E5",  "F5",  "F#5",  "G5",  "G#5",  "A5",  "A#5",  "B5",
    "C6",  "C#6",  "D6",  "D#6",  "E6",  "F6",  "F#6",  "G6",  "G#6",  "A6",  "A#6",  "B6",
    "C7",  "C#7",  "D7",  "D#7",  "E7",  "F7",  "F#7",  "G7",  "G#7",  "A7",  "A#7",  "B7",
    "C8",  "C#8",  "D8",  "D#8",  "E8",  "F8",  "F#8",  "G8",
};

KeyNote::KeyNote(QWidget *parent, unsigned char note) :
    QWidget(parent),
    _ui(new Ui::KeyNote)
{
    _ui->setupUi(this);

    _note = note > MIDI_MAX ? MIDI_MAX : note;

    button()->setText(KeyNote::MIDI_MAP[note]);
}

KeyNote::~KeyNote()
{
    delete _ui;
}

KeyNote *KeyNote::play()
{
    button()->setDown(true);

    return this;
}

KeyNote *KeyNote::release()
{
    button()->setDown(false);

    return this;
}

KeyNote *KeyNote::setFirst()
{
    QPushButton *button = this->button();

    button->setText("");
    button->setStyleSheet("QPushButton {background: #ccc;} QPushButton:pressed {background: #f00;}");

    return this;
}

QPushButton *KeyNote::button()
{
    return this->findChild<QPushButton *>("button");
}

void KeyNote::onNoteChange(note_handler_t handler, void *args...)
{
    _note_handlers.push_back({
                                 .handler = handler,
                                 .args = args
                             });
}


unsigned char KeyNote::note()
{
    return _note;
}

void KeyNote::on_button_pressed()
{
    _noteEvent({.note = _note, .released = false});
    play();
}


void KeyNote::on_button_released()
{
    _noteEvent({.note = _note, .released = true});
    release();
}

void KeyNote::_noteEvent(struct NoteChangeEvent event)
{
    for (auto it = std::begin(_note_handlers); it != std::end(_note_handlers); ++it) {
        it->handler(event, it->args);
    }
}


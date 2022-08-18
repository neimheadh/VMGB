#ifndef GUITARSTRING_H
#define GUITARSTRING_H

#include <QWidget>
#include <QLabel>
#include <vector>

#include "eventmanager.h"
#include "guitar/fret.h"

namespace Ui { class GuitarString; }

namespace Guitar {

/**
 * @brief A guitar string
 */
class String : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief GuitarString constructor.
     * @param em     Event manager.
     * @param frets  Guitar board fret count.
     * @param note   String base MIDI note.
     * @param parent Parent ui.
     */
    explicit String(unsigned char frets = 24, unsigned char note = 0, EventManager *em = nullptr, QWidget *parent = nullptr);
    ~String();

    /**
     * @brief Get the guitar board fret number.
     * @return The guitar board fret number.
     */
    unsigned char frets();
    /**
     * @brief Get the string base MIDI note.
     * @return The base MIDI note.
     */
    unsigned char note();
    /**
     * @brief Get the string playing fret.
     * @return The playing fret. -1 if none.
     */
    int playing();

    /**
     * @brief Get if the string can play the given note.
     * @param note A MIDI note.
     * @return True if the string can play the given note.
     */
    bool canPlay(unsigned char note);
    /**
     * @brief Play a fret.
     * @param fret Fret number.
     */
    void play(unsigned char fret);
    /**
     * @brief Play a MIDI note.
     * @param note The MIDI note.
     */
    void playMidi(unsigned char note);
    /**
     * @brief Release the playing note.
     */
    void release();

    /**
     * @brief Get a key note.
     * @param key The key number.
     * @return A key note.
     */
    Fret *fret(QString fret);
    /**
     * @brief Get a key note.
     * @param key The key number.
     * @return A key note.
     */
    Fret *fret(unsigned char fret);
    /**
     * @brief Get string label.
     * @return The string QLabel.
     */
    QLabel *label();

private:
    /**
     * @brief Event manager.
     */
    EventManager *_em;
    /**
     * @brief Guitar board fret count.
     */
    unsigned char _frets;
    /**
     * @brief Guitar string base MIDI note.
     */
    unsigned char _note;
    /**
     * @brief The playing fret. -1 if none.
     */
    int _playing = -1;
    /**
     * @brief Guitar string ui.
     */
    Ui::GuitarString* _ui;

    /**
     * @brief Build the guitar string ui.
     */
    void _build();
    /**
     * @brief Release the given fret note.
     * @param fret A fret number.
     */
    void _release(unsigned char fret);
};

}

#endif // GUITARSTRING_H

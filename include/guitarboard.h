#ifndef GUITARBOARD_H
#define GUITARBOARD_H

#include <QFrame>
#include <QWidget>
#include "guitarstring.h"

namespace Ui { class GuitarBoard; }

/**
 * @brief The guitar board.
 */
class GuitarBoard : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Guitar board default tuning.
     */
    static const unsigned char DEFAULT_TUNING[];

    /**
     * @brief GuitarBoard constructor.
     * @param parent  Parent ui.
     * @param frets   Number of guitarboard frets.
     * @param strings Number of guitar strings.
     * @param tuning  Guitar tuning.
     */
    explicit GuitarBoard(
            QWidget *parent = nullptr,
            unsigned char frets = 24,
            unsigned char strings = 6,
            const unsigned char tuning[] = nullptr
    );
    ~GuitarBoard();

    /**
     * @brief Get number of guitarboard frets.
     * @return Number of guitarboard frets.
     */
    unsigned char frets();
    /**
     * @brief Get number of guitar strings.
     * @return Number of guitar strings.
     */
    unsigned char strings();
    /**
     * @brief Get guitar tuning.
     * @return Guitar tuning.
     */
    const unsigned char *tuning();

    /**
     * @brief Get if the guitar board can play the given MIDI note.
     * @param note The MIDI note.
     * @return True if the board can play the given note.
     */
    bool canPlay(unsigned char note);
    /**
     * @brief Play a fret on a string.
     * @param string The string number.
     * @param fret   The fret number.
     * @return this
     */
    GuitarBoard *play(unsigned char string, unsigned char fret);
    /**
     * @brief Play a chord.
     * @param chord  The chord. You should put a negative value to silent a note.
     * @param length The number of notes in the chord, including the silent notes.
     * @return this
     */
    GuitarBoard *play(int chord[], unsigned char length);
    /**
     * @brief Play a MIDI note.
     * @param note The MIDI note.
     * @return this
     */
    GuitarBoard *playMidi(unsigned char note);
    /**
     * @brief Play an ensemble of MIDI notes.
     * @param notes  MIDI notes.
     * @param length The played MIDI notes count.
     * @return
     */
    GuitarBoard *playMidi(const unsigned char notes[], unsigned char length);
    /**
     * @brief Release all playing notes.
     * @return this
     */
    GuitarBoard *release();
    /**
     * @brief Release note played on a string.
     * @param string The string number.
     * @return this
     */
    GuitarBoard *release(unsigned char string);
    /**
     * @brief Release notes on severals strings.
     * @param strings The string list.
     * @param length  The string count.
     * @return this
     */
    GuitarBoard *release(int strings[], unsigned char length);
    /**
     * @brief Release a previously played midi note.
     * @param note The note.
     * @return
     */
    GuitarBoard *releaseMidi(unsigned char note);
    /**
     * @brief Resize the board.
     * @return this
     */
    GuitarBoard *resize();

    /**
     * @brief Get dots frame.
     * @return Dot frame.
     */
    QFrame *dotFrame();
    /**
     * @brief Get a string.
     * @param string The string number
     * @return A string.
     */
    GuitarString *string(QString string);
    /**
     * @brief Get a string.
     * @param string The string number
     * @return A string.
     */
    GuitarString *string(unsigned char string);
    /**
     * @brief Get strings frame.
     * @return String frame.
     */
    QFrame *stringFrame();

    /**
     * @brief Add note change handler.
     * @param handler The handler.
     * @param args Handler additionnal args.
     */
    void onNoteChange(note_handler_t handler, void *args...);

private:
    /**
     * @brief The guitar board ui.
     */
    Ui::GuitarBoard *_ui;
    /**
     * @brief The guitar board frets count.
     */
    unsigned char _frets;
    /**
     * @brief Midi note played by strings.
     */
    int *_midi_strings = nullptr;
    /**
     * @brief The guitar string count.
     */
    unsigned char _strings;
    /**
     * @brief The guitar tuning.
     */
    unsigned char *_tuning = nullptr;

    /**
     * @brief Build the guitar board.
     * @return this
     */
    GuitarBoard *_build();

    /**
     * @brief Set the guitar tuning.
     * @param strings The number of guitar string.
     * @param tuning  The tuning.
     * @return this
     */
    GuitarBoard *_setTuning(unsigned char strings, const unsigned char tuning[]);
};

#endif // GUITARBOARD_H

#ifndef GUITARBOARD_H
#define GUITARBOARD_H

#include <QFrame>
#include <QSettings>
#include <QWidget>
#include "eventmanager.h"
#include "guitar/string.h"

namespace Ui { class GuitarBoard; }

namespace Guitar {
/**
 * @brief The guitar board.
 */
class Board : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief GuitarBoard constructor.
     * @param settings VMGB application settings.
     */
    explicit Board(
            QSettings *settings,
            EventManager *eventManager = nullptr,
            QWidget *parent = nullptr
    );
    ~Board();

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
     * @brief Build the guitar board.
     */
    void build();
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
     */
    void play(unsigned char string, unsigned char fret);
    /**
     * @brief Play a chord.
     * @param chord  The chord. You should put a negative value to silent a note.
     * @param length The number of notes in the chord, including the silent notes.
     */
    void play(int chord[], unsigned char length);
    /**
     * @brief Play a MIDI note.
     * @param note The MIDI note.
     */
    void playMidi(unsigned char note);
    /**
     * @brief Play an ensemble of MIDI notes.
     * @param notes  MIDI notes.
     * @param length The played MIDI notes count.
     */
    void playMidi(const unsigned char notes[], unsigned char length);
    /**
     * @brief Release all playing notes.
     */
    void release();
    /**
     * @brief Release note played on a string.
     * @param string The string number.
     */
    void release(unsigned char string);
    /**
     * @brief Release notes on severals strings.
     * @param strings The string list.
     * @param length  The string count.
     */
    void release(int strings[], unsigned char length);
    /**
     * @brief Release a previously played midi note.
     * @param note The note.
     */
    void releaseMidi(unsigned char note);
    /**
     * @brief Resize the board.
     */
    void resize();

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
    String *string(QString string);
    /**
     * @brief Get a string.
     * @param string The string number
     * @return A string.
     */
    String *string(unsigned char string);
    /**
     * @brief Get strings frame.
     * @return String frame.
     */
    QFrame *stringFrame();

    /**
     * @inherits
     */
    void showEvent(QShowEvent *event);

private:
    /**
     * @brief Event manager.
     */
    EventManager *_eventManager;
    /**
     * @brief The guitar board ui.
     */
    Ui::GuitarBoard *_ui;
    /**
     * @brief Application settings.
     */
    QSettings *_settings;

    /**
     * @brief Board dots widgets.
     */
    QLabel **_dot_widgets = nullptr;
    /**
     * @brief The guitar board frets count.
     */
    unsigned char _frets;
    /**
     * @brief Midi note played by strings.
     */
    int *_midi_strings = nullptr;
    /**
     * @brief List of string widgets.
     */
    String **_string_widgets = nullptr;
    /**
     * @brief The guitar string count.
     */
    unsigned char _strings;
    /**
     * @brief The guitar tuning.
     */
    unsigned char *_tuning = nullptr;

    unsigned int _eh_midi = 0;
    unsigned int _eh_setting = 0;

    /**
     * @brief Clean the guitar board.
     */
    void _clear();
    /**
     * @brief Load application settings.
     * @param settings Application settings.
     */
    void _loadSettings(QSettings *settings);

    /**
     * @brief Set the guitar tuning.
     * @param strings The number of guitar string.
     * @param tuning  The tuning.
     */
    void _setTuning(unsigned char strings, const unsigned char tuning[]);
};

}

#endif // GUITARBOARD_H

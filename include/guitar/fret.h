#ifndef NOTE_H
#define NOTE_H

#include <unistd.h>
#include <QWidget>
#include <QPushButton>
#include <QSettings>

#include "eventmanager.h"

struct NoteChangeEvent {
    unsigned char note;
    bool released;
};

typedef void (*note_handler_t)(struct NoteChangeEvent, ...);

namespace Ui {
class GuitarFret;
}

namespace Guitar {

/**
 * @brief A note on the guitar board.
 */
class Fret : public QWidget
{
    Q_OBJECT

public:
    static const char *EVENT_FRET_PRESS;
    static const char *EVENT_FRET_RELEASE;

    /**
     * @brief Fret constructor.
     * @param settings Application settings.
     * @param note The MIDI note number.
     * @param eventManager Event manager.
     * @param parent Parent UI.
     */
    explicit Fret(
            QSettings *settings,
            unsigned char note,
            EventManager *eventManager = nullptr,
            QWidget *parent = nullptr
    );
    ~Fret();

    /**
     * @brief Get the MIDI note number.
     * @return The MIDI note number.
     */
    unsigned char note();

    /**
     * @brief Play the note.
     */
    void play();
    /**
     * @brief Release the note.
     */
    void release();
    /**
     * @brief Set the note button to be displayed as the first string note.
     */
     void setFirst();

    /**
     * @brief Get the note button.
     * @return The note button.
     */
    QPushButton *button();

private slots:

    void on_button_pressed();

    void on_button_released();

private:
    struct _note_handler_v {
        note_handler_t handler;
        void *args;
    };

    /**
     * @brief Event manager.
     */
    EventManager *_em;
    /**
     * @brief The MIDI note.
     */
    unsigned char _note;
    /**
     * @brief Application settings.
     */
    QSettings *_settings;
    /**
     * @brief The note UI.
     */
    Ui::GuitarFret *_ui;
};

}

#endif // NOTE_H

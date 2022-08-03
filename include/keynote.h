#ifndef NOTE_H
#define NOTE_H

#include <unistd.h>
#include <QWidget>
#include <QPushButton>

struct NoteChangeEvent {
    unsigned char note;
    bool released;
};

typedef void (*note_handler_t)(struct NoteChangeEvent, ...);

namespace Ui {
class KeyNote;
}

/**
 * @brief A note on the guitar board.
 */
class KeyNote : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief The map between MIDI notes number & name.
     */
    static const char* MIDI_MAP[];
    /**
     * @brief The MIDI note maximum value.
     */
    static const char MIDI_MAX = 127;

    /**
     * @brief KeyNote constructor.
     * @param parent Parent UI.
     * @param note The MIDI note number.
     */
    explicit KeyNote(QWidget *parent = nullptr, unsigned char note = 0);
    ~KeyNote();

    /**
     * @brief Get the MIDI note number.
     * @return The MIDI note number.
     */
    unsigned char note();

    /**
     * @brief Play the note.
     * @return this
     */
    KeyNote *play();
    /**
     * @brief Release the note.
     * @return this
     */
    KeyNote *release();
    /**
     * @brief Set the note button to be displayed as the first string note.
     * @return this
     */
    KeyNote *setFirst();

    /**
     * @brief Get the note button.
     * @return The note button.
     */
    QPushButton *button();

    /**
     * @brief Add note change handler.
     * @param handler The handler.
     * @param args Handler additionnal args.
     */
    void onNoteChange(note_handler_t handler, void *args...);

private slots:

    void on_button_pressed();

    void on_button_released();

private:
    struct _note_handler_v {
        note_handler_t handler;
        void *args;
    };

    /**
     * @brief The MIDI note.
     */
    unsigned char _note;
    /**
     * @brief Subscribed note-press handlers.
     */
    std::vector<struct _note_handler_v> _note_handlers;
    /**
     * @brief The note UI.
     */
    Ui::KeyNote *_ui;

    /**
     * @brief Throw a note change event.
     * @param event The event.
     */
    void _noteEvent(struct NoteChangeEvent event);
};

#endif // NOTE_H

#ifndef NOTE_H
#define NOTE_H

#include <unistd.h>
#include <QWidget>
#include <QPushButton>

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

private slots:

    void on_button_pressed();

    void on_button_released();

private:
    /**
     * @brief The note UI.
     */
    Ui::KeyNote *_ui;
    /**
     * @brief The MIDI note.
     */
    unsigned char _note;
};

#endif // NOTE_H

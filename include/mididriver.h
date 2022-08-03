#ifndef MIDIDRIVER_H
#define MIDIDRIVER_H

class Midi;

class MidiDriver {
public:
    MidiDriver(Midi *manager);

    /**
     * @brief Free the MIDI driver.
     */
    virtual void free() = 0;
    /**
     * @brief Initialize the MIDI driver.
     */
    virtual void init() = 0;
    /**
     * @brief Send a note off signal to the MIDI sequencer.
     * @param note    The MIDI note.
     * @param channel The channel.
     */
    virtual void noteOff(unsigned char note, unsigned char channel = 0) = 0;
    /**
     * @brief Send a note on signal to the MIDI sequencer.
     * @param note    The MIDI note.
     * @param channel The channel.
     */
    virtual void noteOn(unsigned char note, unsigned char channel = 0) = 0;
    /**
     * @brief Process the midi driver read events.
     *
     * This function is called inside the infinite thread loop.
     */
    virtual void process() = 0;

protected:
    Midi *_manager;
};

#endif // MIDIDRIVER_H

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
    /**     * @brief Process the midi driver read events.
     *
     * This function is called inside the infinite thread loop.
     */
    virtual void process() = 0;

protected:
    Midi *_manager;
};

#endif // MIDIDRIVER_H

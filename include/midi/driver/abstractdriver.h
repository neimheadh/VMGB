#ifndef MIDIDRIVER_H
#define MIDIDRIVER_H

#include "midi/midi.h"

namespace MIDI {
class Manager;

namespace Driver {

class AbstractDriver {
public:
    AbstractDriver(MIDI::Manager *manager);

    /**
     * @brief Free the MIDI driver.
     */
    virtual void free() = 0;
    /**
     * @brief Initialize the MIDI driver.
     */
    virtual void init() = 0;
    /**
     * @brief Process the midi driver read events.
     *
     * This function is called inside the infinite thread loop.
     */
    virtual void process() = 0;

    /**
     * @brief Handle a midi event reception.
     * @param event The event.
     */
    void recieve(struct MidiEvent event);
    /**
     * @brief Send a MIDI event.
     * @param event The event.
     */
    virtual void send(struct MidiEvent event) = 0;

protected:
    Manager *_manager;
};

}
}

#endif // MIDIDRIVER_H

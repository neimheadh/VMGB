#ifndef MIDI_MANAGER_H
#define MIDI_MANAGER_H

#include <alsa/asoundlib.h>
#include <functional>
#include <thread>
#include <vector>

#include "eventmanager.h"
#include "midi/_driver.h"
#include "midi/midi.h"

namespace MIDI {

class Manager
{
    public:
        Manager(_Driver *driver = nullptr, EventManager *eventManager = nullptr);
        Manager(EventManager *eventManager);
        ~Manager();

        /**
         * @brief Start the MIDI listening thread.
         */
        void start();

        /**
         * @brief Stop & free the MIDI listening thread.
         */
        void end();

        /**
         * @brief Handle a MIDI input event.
         * @param event The event.
         */
        void inputEvent(struct MidiEvent event);

        /**
         * @brief Send a midi event.
         * @param event The event.
         */
        void send(const struct MidiEvent *event);

    private:
        _Driver *_driver;
        const _Driver *__driver;
        EventManager *_eventManager;
        std::thread *_thread;

        unsigned int _midi_send_evt_h = 0;

        void _process();
};

}

#endif // MIDI_MANAGER_H

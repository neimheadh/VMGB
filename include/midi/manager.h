#ifndef MIDI_MANAGER_H
#define MIDI_MANAGER_H

#include <QSettings>
#include <alsa/asoundlib.h>
#include <functional>
#include <thread>
#include <vector>

#include "eventmanager.h"
#include "midi/driver/abstractdriver.h"
#include "midi/midi.h"

namespace MIDI {

class Manager
{
    public:
        Manager(QSettings *settings, EventManager *eventManager);
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
         * @brief Recieve a midi event.
         * @param event The event.
         */
        void recieve(struct MidiEvent event);
        /**
         * @brief Send a midi event.
         * @param event The event.
         */
        void send(struct MidiEvent event);

    private:
        MIDI::Driver::AbstractDriver *_driver = nullptr;
        EventManager *_eventManager = nullptr;
        QSettings *_settings = nullptr;
        std::thread *_thread = nullptr;

        unsigned int _midi_send_evt_h = 0;

        void _process();
};

}

#endif // MIDI_MANAGER_H

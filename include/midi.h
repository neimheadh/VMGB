#ifndef MIDI_H
#define MIDI_H

#include <alsa/asoundlib.h>
#include <functional>
#include <thread>
#include <vector>
#include "mididriver.h"

enum MidiEventType {
    NOTE_ON,
    NOTE_OFF,
};

struct MidiEvent {
    enum MidiEventType type;
    unsigned char channel;
    unsigned char value;
};

typedef void (*midi_handler_t)(struct MidiEvent, ...);

class Midi
{
    public:
        Midi(MidiDriver *driver = nullptr);
        ~Midi();

        /**
         * @brief Start the MIDI listening thread.
         */
        void start();

        /**
         * @brief Stop & free the MIDI listening thread.
         */
        void end();

        /**
         * @brief Send a midi event.
         * @param event The event.
         */
        void event(struct MidiEvent event);

        /**
         * @brief Subscribe a new midi handler.
         * @param handler The handler.
         */
        void onMidiEvent(midi_handler_t handler, void *args...);

    private:
        struct _midi_handler_v {
            midi_handler_t handler;
            void *args;
        };

        MidiDriver *_driver;
        const MidiDriver *__driver;
        std::vector<struct _midi_handler_v> _handlers;
        std::thread *_thread;

        void _process();
};

#endif // MIDI_H

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
         * @brief Follow a midi event.
         * @param event The event.
         */
        void followEvent(struct MidiEvent event);
        /**
         * @brief Subscribe a new midi handler.
         * @param handler The handler.
         */
        void onMidiEvent(midi_handler_t handler, void *args...);

        /**
         * @brief Send a note off signal to the MIDI sequencer.
         * @param note    The MIDI note.
         * @param channel The channel.
         */
        void noteOff(unsigned char note, unsigned char channel = 0);
        /**
         * @brief Send a note on signal to the MIDI sequencer.
         * @param note    The MIDI note.
         * @param channel The channel.
         */
        void noteOn(unsigned char note, unsigned char channel = 0);

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

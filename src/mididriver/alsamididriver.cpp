#include "mididriver/alsamididriver.h"

#include <string.h>
#include "midi.h"

AlsaMidiDriver::AlsaMidiDriver(Midi *manager): MidiDriver(manager)
{
    size_t size = strlen(CLIENT_NAME) + strlen(INPUT_PORT_SUFFIX);
    _in_port_name = new char[size + 1];
    strcpy(_in_port_name, CLIENT_NAME);
    strcat(_in_port_name, INPUT_PORT_SUFFIX);

    size = strlen(CLIENT_NAME) + strlen(OUTPUT_PORT_SUFFIX);
    _out_port_name = new char[size + 1];
    strcpy(_out_port_name, CLIENT_NAME);
    strcat(_out_port_name, OUTPUT_PORT_SUFFIX);
}

AlsaMidiDriver::~AlsaMidiDriver()
{
    AlsaMidiDriver::free();
}

void AlsaMidiDriver::free()
{
    if (_handle != nullptr) {
        snd_seq_close(_handle);
    }

    _in_port = -1;
    _out_port = -1;
    _handle = nullptr;

    delete[] _in_port_name;
    delete[] _out_port_name;
}

void AlsaMidiDriver::init()
{
    int err = snd_seq_open(&_handle,
                           "default",
                           SND_SEQ_OPEN_DUPLEX,
                           0);

    if (err == 0) {
        snd_seq_set_client_name(_handle, CLIENT_NAME);
        _in_port = snd_seq_create_simple_port(_handle,
                                              _in_port_name,
                                              SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
                                              SND_SEQ_PORT_TYPE_MIDI_GENERIC);
        _out_port = snd_seq_create_simple_port(_handle,
                                               _out_port_name,
                                               SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
                                               SND_SEQ_PORT_TYPE_MIDI_GENERIC);
    }
}

void AlsaMidiDriver::process()
{
    snd_seq_event_t *ev;

    do {
        snd_seq_event_input(_handle, &ev);
        switch (ev->type) {
            case SND_SEQ_EVENT_CONTROLLER:
                break;
            case SND_SEQ_EVENT_NOTEOFF:
                _manager->event({
                                    .type = MidiEventType::NOTE_OFF,
                                    .channel = ev->data.note.channel,
                                    .value = ev->data.note.note
                                });
                break;
            case SND_SEQ_EVENT_NOTEON:
                _manager->event({
                                    .type = MidiEventType::NOTE_ON,
                                    .channel = ev->data.note.channel,
                                    .value = ev->data.note.note
                            });
                break;
            case SND_SEQ_EVENT_PITCHBEND:
                break;
        }
    } while(snd_seq_event_input_pending(_handle, 0));
}

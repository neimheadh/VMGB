#include "midi/driver/alsa.h"

#include <iostream>
#include <string.h>

#include "midi/manager.h"

using namespace MIDI::Driver;

Alsa::Alsa(Manager *manager, const char *name): AbstractDriver(manager)
{
    size_t port_len, name_len = strlen(name);

    _name = new char[name_len + 1];
    strcpy(_name, name);

    port_len = name_len + strlen(INPUT_PORT_SUFFIX);
    _in_port_name = new char[port_len + 1];
    strcpy(_in_port_name, _name);
    strcat(_in_port_name, INPUT_PORT_SUFFIX);

    port_len = name_len + strlen(OUTPUT_PORT_SUFFIX);
    _out_port_name = new char[port_len + 1];
    strcpy(_out_port_name, _name);
    strcat(_out_port_name, OUTPUT_PORT_SUFFIX);
}

Alsa::~Alsa()
{
    Alsa::free();
}

void Alsa::free()
{
    if (_snd_seq != nullptr) {
        snd_seq_close(_snd_seq);
    }

    _in_port = -1;
    _out_port = -1;
    _snd_seq = nullptr;

    delete[] _in_port_name;
    delete[] _out_port_name;
}

void Alsa::init()
{
    int err = snd_seq_open(&_snd_seq,
                           "default",
                           SND_SEQ_OPEN_DUPLEX,
                           0);

    if (err == 0) {
        snd_seq_set_client_name(_snd_seq, _name);
        _in_port = snd_seq_create_simple_port(_snd_seq,
                                              _in_port_name,
                                              SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
                                              SND_SEQ_PORT_TYPE_MIDI_GENERIC);
        _out_port = snd_seq_create_simple_port(_snd_seq,
                                               _out_port_name,
                                               SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
                                               SND_SEQ_PORT_TYPE_MIDI_GENERIC);
    }
}

void Alsa::process()
{
    snd_seq_event_t *ev;

    do {
        snd_seq_event_input(_snd_seq, &ev);
        switch (ev->type) {
            case SND_SEQ_EVENT_CONTROLLER:
                break;
            case SND_SEQ_EVENT_NOTEOFF:
                recieve({.type = MidiEventType::NOTE_OFF, .channel = ev->data.note.channel, .value = ev->data.note.note});
                break;
            case SND_SEQ_EVENT_NOTEON:
                recieve({.type = MidiEventType::NOTE_ON, .channel = ev->data.note.channel, .value = ev->data.note.note});
                break;
            case SND_SEQ_EVENT_PITCHBEND:
                break;
        }
        snd_seq_free_event(ev);
    } while(snd_seq_event_input_pending(_snd_seq, 0));
}

void Alsa::send(struct MidiEvent event)
{
    switch (event.type) {
    case MidiEventType::NOTE_OFF:
        _noteOff(event.value, event.channel);
        break;
    case MidiEventType::NOTE_ON:
        _noteOn(event.value, event.channel);
        break;
    }
}

void Alsa::_noteOff(unsigned char note, unsigned char channel)
{
    if (_snd_seq != nullptr) {
        snd_seq_event_t ev = {};

        snd_seq_ev_set_noteoff(&ev, channel, note, 127);
        snd_seq_ev_set_direct(&ev);
        snd_seq_ev_set_subs(&ev);
        snd_seq_ev_set_source(&ev, _out_port);

        int err = snd_seq_event_output_direct(_snd_seq, &ev);

        if (err < 0) {
            char errBuff[255];
            sprintf(errBuff, "MIDI ERROR (%d): %s", err, snd_strerror(err));
            std::cout << errBuff << std::endl;
        }
    }
}

void Alsa::_noteOn(unsigned char note, unsigned char channel)
{
    if (_snd_seq != nullptr) {
        snd_seq_event_t ev = {};

        snd_seq_ev_set_noteon(&ev, channel, note, 127);
        snd_seq_ev_set_direct(&ev);
        snd_seq_ev_set_subs(&ev);
        snd_seq_ev_set_source(&ev, _out_port);

        int err = snd_seq_event_output_direct(_snd_seq, &ev);

        if (err < 0) {
            char errBuff[255];
            sprintf(errBuff, "MIDI ERROR (%d): %s", err, snd_strerror(err));
            std::cout << errBuff << std::endl;
        }
    }
}

#include "midi.h"

#include <iostream>
#include <QString>
#include "mididriver/alsamididriver.h"

Midi::Midi(MidiDriver *driver)
{
    __driver = driver;

    if (driver == nullptr) {
        _driver = new AlsaMidiDriver(this);
    } else {
        _driver = driver;
    }
}

Midi::~Midi()
{
    end();
}

void Midi::start()
{
    if (_thread == nullptr) {
        _thread = new std::thread(&Midi::_process, this);
    }
}

void Midi::end()
{
    if (_thread != nullptr) {
        _thread->detach();
        delete _thread;
        _thread = nullptr;
    }

    if (__driver == nullptr && _driver != nullptr) {
        delete (AlsaMidiDriver *) _driver;
        _driver = nullptr;
    }
}

void Midi::event(struct MidiEvent event)
{
    for(auto it = std::begin(_handlers); it != std::end(_handlers); ++it) {
        (it->handler)(event, it->args);
    }
}

void Midi::onMidiEvent(midi_handler_t handler, void *args...)
{
    struct _midi_handler_v midi_handler = {
        .handler = handler,
        .args = args
    };
    _handlers.push_back(midi_handler);
}

void Midi::_process()
{
    _driver->init();

    while (true) _driver->process();
}

#include "midi/manager.h"

#include <iostream>
#include <QString>
#include "midi/driver/alsa.h"

using namespace MIDI;
using namespace MIDI::Driver;

void _on_midi_send(Manager *manager, const struct MidiEvent *event)
{
    manager->send(event);
}

Manager::Manager(_Driver *driver, EventManager *eventManager)
{
    __driver = driver;

    if (driver == nullptr) {
        _driver = new Alsa(this);
    } else {
        _driver = driver;
    }

    _eventManager = eventManager;
}

Manager::Manager(EventManager *eventManager)
{
    __driver = nullptr;
    _driver = new Alsa(this);
    _eventManager = eventManager;
}

Manager::~Manager()
{
    end();

    if (__driver == nullptr && _driver != nullptr) {
        delete (Alsa *) _driver;
        _driver = nullptr;
    }
}

void Manager::start()
{
    if (_thread == nullptr) {
        _thread = new std::thread(&Manager::_process, this);
    }

    if (_eventManager) {
        _midi_send_evt_h = _eventManager->subscribe(EVENT_MIDI_SEND, (event_handler_t *) &_on_midi_send, this);
    }
}

void Manager::end()
{
    if (_thread != nullptr) {
        _thread->detach();
        delete _thread;
        _thread = nullptr;
    }
}

void Manager::inputEvent(struct MidiEvent event)
{
    if (_eventManager != nullptr) {
        _eventManager->dispatch(EVENT_MIDI_INPUT, &event);
    }
}

void Manager::send(const struct MidiEvent *event)
{
    switch (event->type) {
    case MidiEventType::NOTE_OFF:
        _driver->noteOff(event->value, event->channel);
        break;
    case MidiEventType::NOTE_ON:
        _driver->noteOn(event->value, event->channel);
        break;
    }

    if (_eventManager) {
        _eventManager->dispatch(EVENT_MIDI_OUTPUT, (void *) event);
    }
}

void Manager::_process()
{
    _driver->init();

    while (true) _driver->process();
}

#include "midi/manager.h"

#include <iostream>
#include <QString>
#include "setting/driver.h"
#include "midi/driver/alsa.h"

using namespace MIDI;
using namespace MIDI::Driver;

void _on_midi_send(Manager *manager, const struct MidiEvent *event)
{
    manager->send(*event);
}

Manager::Manager(QSettings *settings, EventManager *eventManager)
{
    _driver = nullptr;

    _eventManager = eventManager;
    _settings = settings;
}

Manager::~Manager()
{
    end();
}

void Manager::start()
{   
    // @todo Only ALSA for the moment, see settings in future.
    _driver = new Alsa(this, Setting::Driver::name(_settings).toStdString().data());

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

void Manager::recieve(struct MidiEvent event)
{
    if (_eventManager) {
        _eventManager->dispatch(EVENT_MIDI_INPUT, &event);
    }
}

void Manager::send(struct MidiEvent event)
{
    _driver->send(event);

    if (_eventManager) {
        _eventManager->dispatch(EVENT_MIDI_OUTPUT, &event);
    }
}

void Manager::_process()
{
    _driver->init();

    while (true) _driver->process();
}

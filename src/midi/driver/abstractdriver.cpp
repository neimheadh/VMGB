#include "midi/driver/abstractdriver.h"

#include "midi/manager.h"

using namespace MIDI::Driver;

AbstractDriver::AbstractDriver(MIDI::Manager *manager)
{
    _manager = manager;
}

void AbstractDriver::recieve(struct MidiEvent event)
{
    _manager->recieve(event);
}

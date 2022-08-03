#include "mididriver.h"

#include <QString>
#include <iostream>
#include "midi.h"

MidiDriver::MidiDriver(Midi *manager)
{
    _manager = manager;
}

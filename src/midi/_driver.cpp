#include "midi/_driver.h"

#include <QString>
#include <iostream>
#include "midi/manager.h"

using namespace MIDI;

_Driver::_Driver(Manager *manager)
{
    _manager = manager;
}

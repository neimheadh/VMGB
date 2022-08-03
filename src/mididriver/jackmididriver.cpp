#include "mididriver/jackmididriver.h"

JackMidiDriver::JackMidiDriver(Midi *manager): MidiDriver(manager)
{

}

JackMidiDriver::~JackMidiDriver()
{
    JackMidiDriver::free();
}

void JackMidiDriver::free()
{
    // @TODO
}

void JackMidiDriver::init()
{
    // @TODO
}

void JackMidiDriver::process()
{
    // @TODO
}

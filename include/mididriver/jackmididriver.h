#ifndef JACKMIDIDRIVER_H
#define JACKMIDIDRIVER_H

#include "mididriver.h"

class JackMidiDriver : public MidiDriver
{
public:
    JackMidiDriver(Midi *manager);
    ~JackMidiDriver();

    void free();
    void init();
    void process();
};

#endif // JACKMIDIDRIVER_H

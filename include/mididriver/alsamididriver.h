#ifndef ALSAMIDIDRIVER_H
#define ALSAMIDIDRIVER_H

#include <alsa/asoundlib.h>
#include "mididriver.h"

class AlsaMidiDriver : public MidiDriver
{
public:
    static constexpr char CLIENT_NAME[] = "VMGB";
    static constexpr char INPUT_PORT_SUFFIX[] = "_In";
    static constexpr char OUTPUT_PORT_SUFFIX[] = "_Out";

    AlsaMidiDriver(Midi *manager);
    ~AlsaMidiDriver();

    /**
     * @inherits
     */
    void free();
    /**
     * @inherits
     */
    void init();
    /**
     * @inherits
     */
    void process();

private:
    snd_seq_t *_handle = nullptr;
    int _in_port = -1;
    char *_in_port_name;
    int _out_port = -1;
    char *_out_port_name;
};

#endif // ALSAMIDIDRIVER_H

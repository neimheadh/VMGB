#ifndef MIDI_DRIVER_ALSA_H
#define MIDI_DRIVER_ALSA_H

#include <alsa/asoundlib.h>
#include "midi/_driver.h"

namespace MIDI {
namespace Driver {

class Alsa : public _Driver
{
public:
    static constexpr char CLIENT_NAME[] = "VMGB";
    static constexpr char INPUT_PORT_SUFFIX[] = "_In";
    static constexpr char OUTPUT_PORT_SUFFIX[] = "_Out";

    Alsa(Manager *manager);
    ~Alsa();

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
    void noteOff(unsigned char note, unsigned char channel);
    /**
     * @inherits
     */
    void noteOn(unsigned char note, unsigned char channel);
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

}
}

#endif // MIDI_DRIVER_ALSA_H

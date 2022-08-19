#ifndef MIDI_DRIVER_ALSA_H
#define MIDI_DRIVER_ALSA_H

#include <alsa/asoundlib.h>
#include "midi/driver/abstractdriver.h"

namespace MIDI {
namespace Driver {

class Alsa : public AbstractDriver
{
public:
    static constexpr char NAME[] = "ALSA";

    static constexpr char INPUT_PORT_SUFFIX[] = "_In";
    static constexpr char OUTPUT_PORT_SUFFIX[] = "_Out";

    Alsa(Manager *manager, const char *name);
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
    void process();

    /**
     * @inherits
     */
    void send(struct MidiEvent event);

private:
    int _in_port = -1;
    char *_in_port_name;
    char *_name;
    int _out_port = -1;
    char *_out_port_name;
    snd_seq_t *_snd_seq = nullptr;

    void _noteOff(unsigned char note, unsigned char channel);
    void _noteOn(unsigned char note, unsigned char channel);
};

}
}

#endif // MIDI_DRIVER_ALSA_H

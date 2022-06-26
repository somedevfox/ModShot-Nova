#ifndef AUDIO_H
#define AUDIO_H

#include "util.h"
#include <fmod_studio.h>

struct RGSSThreadData;

class Audio {
public:
    /*
    * Reset is supposed to stop all playing tracks but that's not super easy with FMOD,
    * so it does nothing for no
    */
    void reset();
    /*
    * Update updates the FMOD Studio System
    */
    void update();

    /*
    * studio_system is the FMOD Studio System used for well, calling FMOD Studio functions
    * core_system is the FMOD System (not studio!) used for FMOD Core functions
    */
    FMOD_STUDIO_SYSTEM *studio_system;
    FMOD_SYSTEM *core_system;

private:

    /*
    * Accepts RGSSThreadData as an argument to get
    * the member system from it
    */
    Audio(RGSSThreadData &rtData);
    /*
    * Custom deconstructor to free the FMOD system.
    */
    ~Audio();

    /*
    * Only allow SharedState to construct this class
    */
    friend struct SharedStatePrivate;
};

#endif

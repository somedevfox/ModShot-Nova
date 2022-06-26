#include "audio.h"
#include "eventthread.h"
#include "exception.h"

#include <fmod_studio.h>
#include <fmod_errors.h>

Audio::Audio(RGSSThreadData &rtData)
    : studio_system(rtData.fmodSystem),
      core_system(NULL)
{
    /*
    * Get the core system
    * We don't restrict people to just using FMOD Studio
    */
    FMOD_RESULT result = FMOD_Studio_System_GetCoreSystem(studio_system, &core_system);
    if (result != FMOD_OK) {
        throw Exception(Exception::MKXPError, "Failed to get core system from FMOD studio system: %s", result);
    }
}

Audio::~Audio() {
    FMOD_Studio_System_Release(studio_system);
}

void Audio::reset()
{
    // TODO
}

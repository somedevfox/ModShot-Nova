#ifndef AUDIO_H
#define AUDIO_H

#include "util.h"

struct RGSSThreadData;

class Audio {
public:
    void reset();

private:
    Audio(RGSSThreadData &rtData);

    friend struct SharedStatePrivate;
};

#endif
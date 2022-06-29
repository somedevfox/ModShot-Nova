#include "binding-util.h"
#include "sharedstate.h"
#include "audio.h"
#include "fmod_bindings.h"

#include <fmod_studio.h>
#include <iostream>

void fmodStudioBindingInit()
{
    rb_mFMOD_Studio = rb_define_module_under(rb_mFMOD, "Studio");

    bindFmodStudioBank();
}

#include "fmod_bindings.h"
#include "binding-util.h"
#include "debugwriter.h"

DEF_TYPE(CommandReplay);
CommandReplay::~CommandReplay()
{
    #ifdef AUTO_CLEAN_FMOD
    Debug() << "Warning: auto releasing command replay (garbage collected?)";
    Debug() << "Command replay release result: " << FMOD_Studio_CommandReplay_Release(p);
    #endif
}

VALUE rb_cCommandReplay = Qnil;

void bindFmodStudioCommandReplay() {
    rb_cCommandReplay = rb_define_class_under(
        rb_mFMOD_Studio, "CommandReplay", rb_cObject);
    rb_define_alloc_func(rb_cCommandReplay, classAllocate<&CommandReplayType>);
}

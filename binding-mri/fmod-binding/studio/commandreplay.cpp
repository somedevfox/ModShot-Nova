#include "fmod_bindings.h"
#include "binding-util.h"
#ifdef AUTO_CLEAN_FMOD
#include "debugwriter.h"
#include <fmod_errors.h>
#endif

DEF_TYPE(CommandReplay);
CommandReplay::~CommandReplay()
{
    #ifdef AUTO_CLEAN_FMOD
    Debug() << "Warning: auto releasing command replay (garbage collected?)";
    Debug() << "Command replay release result: " << FMOD_ErrorString(FMOD_Studio_CommandReplay_Release(p));
    #endif
}

VALUE rb_cCommandReplay = Qnil;

FMOD_VALID_FUNC(FMOD_Studio_CommandReplay, CommandReplay);

RB_METHOD(commandReplayGetSystem) {
    RB_UNUSED_PARAM;

    CommandReplay *b = getPrivateData<CommandReplay>(self);
    FMOD_STUDIO_SYSTEM *system;

    FMOD_RESULT result = FMOD_Studio_CommandReplay_GetSystem(b->p, &system);


    FMOD_RESULT_WRAP(system, StudioSystem);
}

void bindFmodStudioCommandReplay() {
    rb_cCommandReplay = rb_define_class_under(
        rb_mFMOD_Studio, "CommandReplay", rb_cObject);
    rb_define_alloc_func(rb_cCommandReplay, classAllocate<&CommandReplayType>);
    _rb_define_method(rb_cCommandReplay, "initialize", fmodErrorInit);

    _rb_define_method(rb_cCommandReplay, "is_valid", fmodIsValid);
    _rb_define_method(rb_cCommandReplay, "get_system", commandReplayGetSystem);
}

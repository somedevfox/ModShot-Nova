#include "fmod_bindings.h"
#include "binding-util.h"
#ifdef AUTO_CLEAN_FMOD
#include "debugwriter.h"
#include <fmod_errors.h>
#endif

DEF_TYPE(EventInstance);
EventInstance::~EventInstance()
{
    #ifdef AUTO_CLEAN_FMOD
    Debug() << "Warning: auto releasing event instance (garbage collected?)";
    Debug() << "Event instance release result: " << FMOD_ErrorString(FMOD_Studio_EventInstance_Release(p));
    #endif
}

VALUE rb_cEventInstance = Qnil;

void bindFmodStudioEventInstance() {
    rb_cEventInstance = rb_define_class_under(
        rb_mFMOD_Studio, "EventInstance", rb_cObject);
    rb_define_alloc_func(rb_cEventInstance, classAllocate<&EventInstanceType>);
    _rb_define_method(rb_cEventInstance, "initialize", fmodErrorInit);
}

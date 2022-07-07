#include "fmod_bindings.h"
#include "binding-util.h"
#include "debugwriter.h"

DEF_TYPE(EventInstance);
EventInstance::~EventInstance()
{
    #if AUTO_RELEASE
    Debug() << "Warning: auto releasing event instance (garbage collected?)";
    Debug() << "Event instance release result: " << FMOD_Studio_EventInstance_Release(p);
    #endif
}

VALUE rb_cEventInstance = Qnil;

void bindFmodStudioEventInstance() {
    rb_cEventInstance = rb_define_class_under(
        rb_mFMOD_Studio, "EventInstance", rb_cObject);
    rb_define_alloc_func(rb_cEventInstance, classAllocate<&EventInstanceType>);
}

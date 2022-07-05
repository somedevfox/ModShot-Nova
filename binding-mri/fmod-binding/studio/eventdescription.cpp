#include "fmod_bindings.h"
#include "binding-util.h"
#include "debugwriter.h"

DEF_TYPE(EventDescription);
EventDescription::~EventDescription()
{
}

FMOD_VALID_FUNC(FMOD_Studio_EventDescription, EventDescription);

FMOD_ID_FUNC(FMOD_Studio_EventDescription, EventDescription);

FMOD_PATH_FUNC(FMOD_Studio_EventDescription, EventDescription);

FMOD_USERDATA_FUNC(FMOD_Studio_EventDescription, EventDescription);

VALUE rb_cEventDescription = Qnil;

void bindFmodEventdescription()
{
    rb_cEventDescription = rb_define_class_under(
        rb_mFMOD_Studio, "EventDescription", rb_cObject);
    rb_define_alloc_func(rb_cEventDescription, classAllocate<&EventDescriptionType>);

    _rb_define_method(rb_cEventDescription, "is_valid", fmodIsValid);
    _rb_define_method(rb_cEventDescription, "get_id", fmodGetID);
    _rb_define_method(rb_cEventDescription, "get_path", fmodGetPath);
}
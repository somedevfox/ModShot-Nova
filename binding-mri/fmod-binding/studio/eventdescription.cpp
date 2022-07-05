#include "fmod_bindings.h"
#include "binding-util.h"
#include "debugwriter.h"

DEF_TYPE(EventDescription);
EventDescription::~EventDescription()
{
}

VALUE rb_cEventDescription = Qnil;

FMOD_VALID_FUNC(FMOD_Studio_EventDescription, EventDescription);

FMOD_ID_FUNC(FMOD_Studio_EventDescription, EventDescription);

FMOD_PATH_FUNC(FMOD_Studio_EventDescription, EventDescription);

FMOD_USERDATA_FUNC(FMOD_Studio_EventDescription, EventDescription);

RB_METHOD(descriptionGetParameterDescriptionCount)
{
    RB_UNUSED_PARAM;

    EventDescription *b = getPrivateData<EventDescription>(self);
    int count;

    FMOD_RESULT result = FMOD_Studio_EventDescription_GetParameterDescriptionCount(
        b->p, &count);

    FMOD_RESULT_CONVERT(count, INT2NUM);
}

void bindFmodEventdescription()
{
    rb_cEventDescription = rb_define_class_under(
        rb_mFMOD_Studio, "EventDescription", rb_cObject);
    rb_define_alloc_func(rb_cEventDescription, classAllocate<&EventDescriptionType>);

    _rb_define_method(rb_cEventDescription, "is_valid", fmodIsValid);
    _rb_define_method(rb_cEventDescription, "get_id", fmodGetID);
    _rb_define_method(rb_cEventDescription, "get_path", fmodGetPath);
    _rb_define_method(rb_cEventDescription, "get_user_data", fmodGetUserData);
    _rb_define_method(rb_cEventDescription, "set_user_data", fmodSetUserData);
    _rb_define_method(rb_cEventDescription, "get_parameterdescription_count", descriptionGetParameterDescriptionCount);
}

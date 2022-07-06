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

RB_METHOD(descriptionGetParameterCount)
{
    RB_UNUSED_PARAM;

    EventDescription *b = getPrivateData<EventDescription>(self);
    int count;

    FMOD_RESULT result = FMOD_Studio_EventDescription_GetParameterDescriptionCount(
        b->p, &count);

    FMOD_RESULT_CONVERT(count, INT2NUM);
}

RB_METHOD(descriptionGetParameterIndex) {
    int index;
    rb_get_args(argc, argv, "i", &index RB_ARG_END);

    EventDescription *b = getPrivateData<EventDescription>(self);
    FMOD_STUDIO_PARAMETER_DESCRIPTION *description = new FMOD_STUDIO_PARAMETER_DESCRIPTION();

    FMOD_RESULT result = FMOD_Studio_EventDescription_GetParameterDescriptionByIndex(
        b->p, index, description
    );

    FMOD_RESULT_NO_WRAP(description, FMOD_STUDIO_PARAMETER_DESCRIPTION);
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
    _rb_define_method(rb_cEventDescription, "get_parameter_description_count", descriptionGetParameterCount);
    _rb_define_method(rb_cEventDescription, "get_parameter_description_by_index", descriptionGetParameterIndex);
}

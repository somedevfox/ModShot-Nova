#include "binding-util.h"
#include "fmod_bindings.h"
#include "debugwriter.h"

DEF_TYPE(VCA);
VCA::~VCA()
{
    Debug() << "Warning: VCA deconstructed (garbage collected?)";
}

VALUE rb_cVCA = Qnil;

FMOD_VALID_FUNC(FMOD_Studio_VCA, VCA);

FMOD_ID_FUNC(FMOD_Studio_VCA, VCA);

FMOD_PATH_FUNC(FMOD_Studio_VCA, VCA);

RB_METHOD(vcaGetVolume)
{
    RB_UNUSED_PARAM;

    VCA *b = getPrivateData<VCA>(self);
    float volume;
    float finalvolume;

    FMOD_RESULT result = FMOD_Studio_VCA_GetVolume(b->p, &volume, &finalvolume);

    //? I am questioning not adding a dual result macro now
    FMOD_RESULT_BASE;
    if (result == FMOD_OK)
    {
        rb_ary_push(return_ary, DBL2NUM(volume));
        rb_ary_push(return_ary, DBL2NUM(finalvolume));
    }
    FMOD_RESULT_RET;
}

RB_METHOD(vcaSetVolume)
{
    float volume;

    rb_get_args(argc, argv, "f", &volume RB_ARG_END);

    VCA *b = getPrivateData<VCA>(self);

    FMOD_RESULT result = FMOD_Studio_VCA_SetVolume(b->p, volume);

    FMOD_RESULT_SIMPLE;
}

void bindFmodStudioVCA()
{
    rb_cVCA = rb_define_class_under(rb_mFMOD_Studio, "VCA", rb_cObject);
    rb_define_alloc_func(rb_cVCA, classAllocate<&VCAType>);

    _rb_define_method(rb_cVCA, "is_valid", fmodIsValid);
    _rb_define_method(rb_cVCA, "get_id", fmodGetID);
    _rb_define_method(rb_cVCA, "get_path", fmodGetPath);
    _rb_define_method(rb_cVCA, "get_volume", vcaGetVolume);
    _rb_define_method(rb_cVCA, "set_volume", vcaSetVolume);
}
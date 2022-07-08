#include "binding-util.h"
#include "fmod_bindings.h"
#include "debugwriter.h"

DEF_TYPE(StudioSystem);
StudioSystem::~StudioSystem()
{
    #ifdef AUTO_CLEAN_FMOD
    Debug() << "Warning: auto releasing studio system (garbage collected?)";
    Debug() << "Studio system release result: " << FMOD_Studio_System_Release(p);
    #endif
}

//! Need to define custom new function because Ruby
//! does not allow for custom return values from initialize.
RB_METHOD(systemNew) {
    int maxchannels;
    int studioflags;
    int flags;
    rb_get_args(argc, argv, "iii", &maxchannels, &studioflags, &flags RB_ARG_END);

    FMOD_STUDIO_SYSTEM *system = NULL;
    FMOD_RESULT result = FMOD_Studio_System_Create(&system, FMOD_VERSION);

    if (result == FMOD_OK) {
        result = FMOD_Studio_System_Initialize(system, maxchannels, studioflags, flags, NULL);
    }

    FMOD_RESULT_BASE;
    if (result == FMOD_OK) {
        VALUE self = rb_obj_alloc(rb_cStudioSystem); //! Manually allocate the object
        setPrivateData(self, new StudioSystem(system));
        rb_ary_push(return_ary, self);
    }
    FMOD_RESULT_RET;
}

FMOD_VALID_FUNC(FMOD_Studio_System, StudioSystem);

RB_METHOD(systemLoadBankFile) {
    const char* filename;
    int flags;

    rb_get_args(argc, argv, "zi", &filename, &flags RB_ARG_END);

    StudioSystem *b = getPrivateData<StudioSystem>(self);

    //? Initialize bank as a NULL pointer
    FMOD_STUDIO_BANK* bank = NULL;
    //? Load bank
    FMOD_RESULT result = FMOD_Studio_System_LoadBankFile(
        b->p, filename, flags, &bank
    );

    //? Wrap bank result and return it
    FMOD_RESULT_WRAP(bank, Bank);
}

VALUE rb_cStudioSystem = Qnil;

void bindFmodStudioSystem() {
    rb_cStudioSystem = rb_define_class_under(rb_mFMOD_Studio, "System", rb_cObject);
    rb_define_alloc_func(rb_cStudioSystem, classAllocate<&StudioSystemType>);

    _rb_define_module_function(rb_cStudioSystem, "new", systemNew);
    _rb_define_method(rb_cStudioSystem, "is_valid", fmodIsValid);
    _rb_define_method(rb_cStudioSystem, "load_bank_file", systemLoadBankFile);
}

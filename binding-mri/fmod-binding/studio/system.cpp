#include "binding-util.h"
#include "fmod_bindings.h"
#include "sharedstate.h"

RB_METHOD(systemInitialize) {
    int maxchannels;
    int studioflags;
    int flags;
    rb_get_args(argc, argv, "iii", &maxchannels, &studioflags, &flags RB_ARG_END);

    FMOD_RESULT result = FMOD_Studio_System_Initialize(
        shState->studio_system(), maxchannels, studioflags, flags, 0
    );

    FMOD_RESULT_SIMPLE;
}

RB_METHOD(systemIsValid) {
    return BOOL2RB(FMOD_Studio_System_IsValid(shState->studio_system()));
}

RB_METHOD(systemLoadBankFile) {
    const char* filename;
    int flags;

    rb_get_args(argc, argv, "zi", &filename, &flags RB_ARG_END);

    //? Initialize bank as a NULL pointer
    FMOD_STUDIO_BANK* bank = NULL;
    //? Load bank
    FMOD_RESULT result = FMOD_Studio_System_LoadBankFile(
        shState->studio_system(), filename, flags, &bank
    );

    //? Wrap bank result and return it
    FMOD_RESULT_WRAP(bank, Bank);
}

void bindFmodStudioSystem() {
    VALUE module = rb_define_module_under(rb_mFMOD_Studio, "System");

    _rb_define_module_function(module, "initialize", systemInitialize);
    _rb_define_module_function(module, "is_valid", systemIsValid);
    _rb_define_module_function(module, "load_bank_file", systemLoadBankFile);
}

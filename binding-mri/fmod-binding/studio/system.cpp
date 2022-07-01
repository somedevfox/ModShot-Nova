#include "binding-util.h"
#include "fmod_bindings.h"
#include "sharedstate.h"
#include "audio.h"

RB_METHOD(loadBankFile) {
    const char* filename;
    int flags;

    rb_get_args(argc, argv, "zi", &filename, &flags RB_ARG_END);

    //? Initialize bank as a NULL pointer
    FMOD_STUDIO_BANK* bank = NULL;
    //? Load bank
    FMOD_RESULT result = FMOD_Studio_System_LoadBankFile(
        shState->audio().studio_system, filename, flags, &bank
    );

    //? Wrap bank result and return it
    FMOD_RESULT_WRAP(bank, Bank);
}

void bindFmodStudioSystem() {
    VALUE module = rb_define_module_under(rb_mFMOD_Studio, "System");

    _rb_define_module_function(module, "load_bank_file", loadBankFile);
}
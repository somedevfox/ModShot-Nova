#include "binding-util.h"
#include "sharedstate.h"
#include "audio.h"

//#include <rice/rice.hpp>
#include <ruby/ruby.h>
#include <fmod_studio.h>
#include <iostream>

using namespace Rice;

class BankWrapper
{
public:
    FMOD_STUDIO_BANK *bank;

    BankWrapper(FMOD_STUDIO_BANK *bank)
        : bank(bank)
    {
    }
};

VALUE fmodStudioParseID(const char *idstring)
{
    FMOD_GUID *guid = NULL;

    FMOD_RESULT result = FMOD_Studio_ParseID(idstring, guid);

    Array a;
    a.push((int)result);
    if (guid)
        a.push(guid);

    return a;
}

VALUE fmodStudioLoadBankFile(const char *filename, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
{
    FMOD_STUDIO_BANK *bank = NULL;

    FMOD_RESULT result = FMOD_Studio_System_LoadBankFile(
        shState->audio().studio_system, filename, flags, &bank);

    Array a;
    a.push((int)result);
    if (bank)
        a.push(BankWrapper(bank));

    return a;
}

void fmodStudioBindingInit()
{
    /*
    Module rb_mFMOD = Module(rb_const_get(rb_cObject, rb_intern("FMOD")));

    Module rb_mFMOD_Studio = define_module_under(rb_mFMOD, "Studio")
                                 .define_module_function("parse_id", &fmodStudioParseID, Return().takeOwnership());

    Data_Type<BankWrapper> rb_cFMOD_Studio_Bank =
        define_class_under<BankWrapper>(rb_mFMOD_Studio, "Bank");

    Module rb_mFMOD_System = define_module_under(rb_mFMOD_Studio, "System");
    */
}

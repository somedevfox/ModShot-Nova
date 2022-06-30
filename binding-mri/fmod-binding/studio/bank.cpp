#include "binding-util.h"
#include "fmod_bindings.h"

DEF_TYPE(Bank);
VALUE rb_cBank = Qnil;

RB_METHOD(bankIsValid) {
    RB_UNUSED_PARAM;

    //? Get wrapper
    Bank *b = getPrivateData<Bank>(self);

    //? Get result of IsValid (an int for some reason?) and convert it to a ruby bool
    //? BOOL2RB is defined in fmod_bindings.h since ruby doesn't ship it
    return BOOL2RB(FMOD_Studio_Bank_IsValid(b->p));
}

void bindFmodStudioBank() {
    rb_cBank = rb_define_class_under(rb_mFMOD_Studio, "Bank", rb_cObject);
    rb_define_alloc_func(rb_cBank, classAllocate<&BankType>);

    _rb_define_method(rb_cBank, "is_valid", bankIsValid);
}

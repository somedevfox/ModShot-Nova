#include "binding-util.h"
#include "fmod_bindings.h"

DEF_TYPE(Bank);

RB_METHOD(bankIsValid) {
    RB_UNUSED_PARAM;

    Bank *b = getPrivateData<Bank>(self);

    return BOOL2RB(FMOD_Studio_Bank_IsValid(b->p));
}

void bindFmodStudioBank() {
    VALUE klass = rb_define_class_under(rb_mFMOD_Studio, "Bank", rb_cObject);
    rb_define_alloc_func(klass, classAllocate<&BankType>);

    _rb_define_method(klass, "is_valid", bankIsValid);
}

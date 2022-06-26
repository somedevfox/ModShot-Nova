#include "binding-util.h"
#include "exception.h"

#include <rice/rice.hpp>
#include <fmod_studio.h>

using namespace Rice;

String fmodGuidData4(Object self) {
    Data_Object<FMOD_GUID> data(self);

    return String((const char*) data->Data4);
}

void fmodStructsBindingInit() {
    Module rb_mFMOD = Module(rb_const_get(rb_cObject, rb_intern("FMOD")));

    Data_Type<FMOD_GUID> rb_cFMOD_GUID =
        define_class_under<FMOD_GUID>(rb_mFMOD, "GUID")
        .define_attr("data1", &FMOD_GUID::Data1, AttrAccess::Read)
        .define_attr("data2", &FMOD_GUID::Data2, AttrAccess::Read)
        .define_attr("data3", &FMOD_GUID::Data3, AttrAccess::Read)
        .define_method("data4", &fmodGuidData4);
}

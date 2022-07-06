#include "common_structs.h"
#include "fmod_bindings.h"
#include <fmod_studio.h>

VALUE rb_cGUID = Qnil;

DECLARE_FMOD2RB(FMOD_GUID, rb_cGUID);
FMOD2RB_NAME(data1, Data1, UINT2NUM);
FMOD2RB_NAME(data2, Data2, UINT2NUM);
FMOD2RB_NAME(data3, Data3, UINT2NUM);
FMOD2RB_CAST(data4, Data4, char*, rb_str_new_cstr);
FMOD2RB_END;

void bindFmodCoreStructs()
{
    rb_cGUID = rb_define_class_under(rb_mFMOD_Core, "GUID", rb_cObject);
    ATTR(rb_cGUID, data1);
    ATTR(rb_cGUID, data2);
    ATTR(rb_cGUID, data3);
    ATTR(rb_cGUID, data4);
}

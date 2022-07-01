#include "common_structs.h"
#include "fmod_bindings.h"
#include <fmod_studio.h>

DEF_TYPE(FMOD_GUID);
VALUE rb_cGUID = Qnil;

DEFINE_NAMED_ATTR_INT(FMOD_GUID, data1, Data1);
DEFINE_NAMED_ATTR_INT(FMOD_GUID, data2, Data2);
DEFINE_NAMED_ATTR_INT(FMOD_GUID, data3, Data3);
DEFINE_NAMED_ATTR_STR(FMOD_GUID, data4, Data4, 8);

void bindFmodStudioStructs()
{
    rb_cGUID = rb_define_class_under(rb_mFMOD, "GUID", rb_cObject);
    rb_define_alloc_func(rb_cGUID, classAllocate<&FMOD_GUIDType>);
    EXPOSE_ATTRIBUTE(rb_cGUID, FMOD_GUID, data1);
    EXPOSE_ATTRIBUTE(rb_cGUID, FMOD_GUID, data2);
    EXPOSE_ATTRIBUTE(rb_cGUID, FMOD_GUID, data3);
    EXPOSE_ATTRIBUTE(rb_cGUID, FMOD_GUID, data4);
}
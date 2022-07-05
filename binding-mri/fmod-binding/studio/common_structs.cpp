#include "common_structs.h"
#include "fmod_bindings.h"
#include <fmod_studio.h>

DEF_TYPE(FMOD_STUDIO_MEMORY_USAGE);
VALUE rb_cMemoryUsage = Qnil;

DEFINE_ATTR_INT(FMOD_STUDIO_MEMORY_USAGE, exclusive);
DEFINE_ATTR_INT(FMOD_STUDIO_MEMORY_USAGE, inclusive);
DEFINE_ATTR_INT(FMOD_STUDIO_MEMORY_USAGE, sampledata);

DEF_TYPE(FMOD_STUDIO_PARAMETER_DESCRIPTION);
VALUE rb_cParameterDescription = Qnil;
DEFINE_ATTR_STR(FMOD_STUDIO_PARAMETER_DESCRIPTION, name);
DEFINE_ATTR_NOWRAP(FMOD_STUDIO_PARAMETER_DESCRIPTION, id, rb_cParameterID);
DEFINE_ATTR_FLOAT(FMOD_STUDIO_PARAMETER_DESCRIPTION, minimum);
DEFINE_ATTR_FLOAT(FMOD_STUDIO_PARAMETER_DESCRIPTION, maximum);
DEFINE_ATTR_FLOAT(FMOD_STUDIO_PARAMETER_DESCRIPTION, defaultvalue);
DEFINE_ATTR_INT(FMOD_STUDIO_PARAMETER_DESCRIPTION, type);
DEFINE_ATTR_UINT(FMOD_STUDIO_PARAMETER_DESCRIPTION, flags);
DEFINE_ATTR_NOWRAP(FMOD_STUDIO_PARAMETER_DESCRIPTION, guid, rb_cGUID);

DEF_TYPE(FMOD_STUDIO_PARAMETER_ID);
VALUE rb_cParameterID = Qnil;
DEFINE_ATTR_UINT(FMOD_STUDIO_PARAMETER_ID, data1);
DEFINE_ATTR_UINT(FMOD_STUDIO_PARAMETER_ID, data2);

void bindFmodStudioStructs()
{
    rb_cMemoryUsage = rb_define_class_under(rb_mFMOD_Studio, "MemoryUsage", rb_cObject);
    rb_define_alloc_func(rb_cMemoryUsage, classAllocate<&FMOD_STUDIO_MEMORY_USAGEType>);
    EXPOSE_ATTRIBUTE(rb_cMemoryUsage, FMOD_STUDIO_MEMORY_USAGE, exclusive);
    EXPOSE_ATTRIBUTE(rb_cMemoryUsage, FMOD_STUDIO_MEMORY_USAGE, inclusive);
    EXPOSE_ATTRIBUTE(rb_cMemoryUsage, FMOD_STUDIO_MEMORY_USAGE, sampledata);

    rb_cParameterDescription = rb_define_class_under(
        rb_mFMOD_Studio, "ParameterDescription", rb_cObject);
    rb_define_alloc_func(
        rb_cParameterDescription, classAllocate<&FMOD_STUDIO_PARAMETER_DESCRIPTIONType>);
    EXPOSE_ATTRIBUTE(rb_cMemoryUsage, FMOD_STUDIO_PARAMETER_DESCRIPTION, name);
    EXPOSE_ATTRIBUTE(rb_cMemoryUsage, FMOD_STUDIO_PARAMETER_DESCRIPTION, id);
    EXPOSE_ATTRIBUTE(rb_cMemoryUsage, FMOD_STUDIO_PARAMETER_DESCRIPTION, minimum);
    EXPOSE_ATTRIBUTE(rb_cMemoryUsage, FMOD_STUDIO_PARAMETER_DESCRIPTION, maximum);
    EXPOSE_ATTRIBUTE(rb_cMemoryUsage, FMOD_STUDIO_PARAMETER_DESCRIPTION, defaultvalue);
    EXPOSE_ATTRIBUTE(rb_cMemoryUsage, FMOD_STUDIO_PARAMETER_DESCRIPTION, type);
    EXPOSE_ATTRIBUTE(rb_cMemoryUsage, FMOD_STUDIO_PARAMETER_DESCRIPTION, flags);
    EXPOSE_ATTRIBUTE(rb_cMemoryUsage, FMOD_STUDIO_PARAMETER_DESCRIPTION, guid);

    rb_cParameterID = rb_define_class_under(rb_mFMOD_Studio, "ParameterID", rb_cObject);
    rb_define_alloc_func(rb_cParameterID, classAllocate<&FMOD_STUDIO_PARAMETER_IDType>);
    EXPOSE_ATTRIBUTE(rb_cParameterID, FMOD_STUDIO_PARAMETER_ID, data1);
    EXPOSE_ATTRIBUTE(rb_cParameterID, FMOD_STUDIO_PARAMETER_ID, data2);
}

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

void bindFmodStudioStructs() {
    rb_cMemoryUsage = rb_define_class_under(rb_mFMOD_Studio, "MemoryUsage", rb_cObject);
    EXPOSE_ATTRIBUTE(rb_cMemoryUsage, FMOD_STUDIO_MEMORY_USAGE, exclusive);
    EXPOSE_ATTRIBUTE(rb_cMemoryUsage, FMOD_STUDIO_MEMORY_USAGE, inclusive);
    EXPOSE_ATTRIBUTE(rb_cMemoryUsage, FMOD_STUDIO_MEMORY_USAGE, sampledata);
}
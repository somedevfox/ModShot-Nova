#include "common_structs.h"
#include "fmod_bindings.h"
#include <fmod_studio.h>

VALUE rb_cMemoryUsage = Qnil;
DECLARE_FMOD2RB(FMOD_STUDIO_MEMORY_USAGE, rb_cMemoryUsage);
FMOD2RB(exclusive, INT2NUM);
FMOD2RB(inclusive, INT2NUM);
FMOD2RB(sampledata, INT2NUM);
FMOD2RB_END;

VALUE rb_cParameterDescription = Qnil;
DECLARE_FMOD2RB(FMOD_STUDIO_PARAMETER_DESCRIPTION, rb_cParameterDescription);
FMOD2RB(name, rb_str_new_cstr);
FMOD2RB_STRUCT(id, FMOD_STUDIO_PARAMETER_ID);
FMOD2RB(minimum, DBL2NUM);
FMOD2RB(maximum, DBL2NUM);
FMOD2RB_NAME(default_value, defaultvalue, DBL2NUM);
FMOD2RB(type, INT2NUM);
FMOD2RB(flags, INT2NUM);
FMOD2RB_STRUCT(guid, FMOD_GUID);
FMOD2RB_END;

VALUE rb_cParameterID = Qnil;
DECLARE_FMOD2RB(FMOD_STUDIO_PARAMETER_ID, rb_cParameterID);
FMOD2RB(data1, UINT2NUM);
FMOD2RB(data2, UINT2NUM);
FMOD2RB_END;

DECLARE_RB2FMOD(FMOD_STUDIO_PARAMETER_ID, rb_cParameterID);
RB2FMOD(data1, NUM2UINT);
RB2FMOD(data2, NUM2UINT);
RB2FMOD_END;

VALUE rb_cUserProperty = Qnil;
DECLARE_FMOD2RB(FMOD_STUDIO_USER_PROPERTY, rb_cUserProperty);
FMOD2RB(name, rb_str_new_cstr);
FMOD2RB(type, INT2NUM);
switch (p->type)
{
    case FMOD_STUDIO_USER_PROPERTY_TYPE_INTEGER:
        rb_iv_set(self, "@value", INT2NUM(p->intvalue));
        break;
    case FMOD_STUDIO_USER_PROPERTY_TYPE_BOOLEAN:
        rb_iv_set(self, "@value", BOOL2RB(p->boolvalue));
        break;
    case FMOD_STUDIO_USER_PROPERTY_TYPE_FLOAT:
        rb_iv_set(self, "@value", DBL2NUM(p->floatvalue));
        break;
    case FMOD_STUDIO_USER_PROPERTY_TYPE_STRING:
        rb_iv_set(self, "@value", rb_str_new_cstr(p->stringvalue));
        break;
    default:
        rb_raise(rb_eRuntimeError, "User property type is invalid");
        break;
}
FMOD2RB_END;

void bindFmodStudioStructs()
{
    rb_cMemoryUsage = rb_define_class_under(rb_mFMOD_Studio, "MemoryUsage", rb_cObject);
    ATTR(rb_cMemoryUsage, exclusive);
    ATTR(rb_cMemoryUsage, inclusive);
    ATTR(rb_cMemoryUsage, sampledata);

    rb_cParameterDescription = rb_define_class_under(
        rb_mFMOD_Studio, "ParameterDescription", rb_cObject);
    ATTR(rb_cParameterDescription, name);
    ATTR(rb_cParameterDescription, id);
    ATTR(rb_cParameterDescription, minimum);
    ATTR(rb_cParameterDescription, maximum);
    ATTR(rb_cParameterDescription, default_value);
    ATTR(rb_cParameterDescription, type);
    ATTR(rb_cParameterDescription, flags);
    ATTR(rb_cParameterDescription, guid);

    rb_cParameterID = rb_define_class_under(rb_mFMOD_Studio, "ParameterID", rb_cObject);
    ATTR(rb_cParameterID, data1);
    ATTR(rb_cParameterID, data2);

    rb_cUserProperty = rb_define_class_under(rb_mFMOD_Studio, "UserProperty", rb_cObject);
    ATTR(rb_cUserProperty, name);
    ATTR(rb_cUserProperty, type);
    ATTR(rb_cUserProperty, value);
}

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

DECLARE_RB2FMOD(FMOD_GUID, rb_cGUID);
RB2FMOD_NAME(data1, Data1, NUM2UINT);
RB2FMOD_NAME(data2, Data2, NUM2UINT);
RB2FMOD_NAME(data3, Data3, NUM2UINT);
if (RSTRING_LEN(rb_iv_get(self, "@data4")) != 8)
{
    rb_raise(rb_eArgError, "data4 must be 8 bytes long");
}
strcpy((char*) p->Data4, RSTRING_PTR(rb_iv_get(self, "@data4"))); //! BAD UNSAFE BAD
RB2FMOD_END;

VALUE rb_c3D_Attributes = Qnil;
DECLARE_FMOD2RB(FMOD_3D_ATTRIBUTES, rb_c3D_Attributes);
FMOD2RB_STRUCT(position, FMOD_VECTOR);
FMOD2RB_STRUCT(velocity, FMOD_VECTOR);
FMOD2RB_STRUCT(forward, FMOD_VECTOR);
FMOD2RB_STRUCT(up, FMOD_VECTOR);
FMOD2RB_END;

DECLARE_RB2FMOD(FMOD_3D_ATTRIBUTES, rb_c3D_Attributes);
RB2FMOD_STRUCT(position, FMOD_VECTOR);
RB2FMOD_STRUCT(velocity, FMOD_VECTOR);
RB2FMOD_STRUCT(forward, FMOD_VECTOR);
RB2FMOD_STRUCT(up, FMOD_VECTOR);
RB2FMOD_END;

VALUE rb_cVector = Qnil;
DECLARE_FMOD2RB(FMOD_VECTOR, rb_cVector);
FMOD2RB(x, DBL2NUM);
FMOD2RB(y, DBL2NUM);
FMOD2RB(z, DBL2NUM);
FMOD2RB_END;

DECLARE_RB2FMOD(FMOD_VECTOR, rb_cVector);
RB2FMOD(x, NUM2DBL);
RB2FMOD(y, NUM2DBL);
RB2FMOD(z, NUM2DBL);
RB2FMOD_END;

void bindFmodCoreStructs()
{
    rb_cGUID = rb_define_class_under(rb_mFMOD_Core, "GUID", rb_cObject);
    ATTR(rb_cGUID, data1);
    ATTR(rb_cGUID, data2);
    ATTR(rb_cGUID, data3);
    ATTR(rb_cGUID, data4);

    rb_c3D_Attributes = rb_define_class_under(rb_mFMOD_Core, "_3DAttributes", rb_cObject);
    ATTR(rb_c3D_Attributes, position);
    ATTR(rb_c3D_Attributes, velocity);
    ATTR(rb_c3D_Attributes, forward);
    ATTR(rb_c3D_Attributes, up);

    rb_cVector = rb_define_class_under(rb_mFMOD_Core, "Vector", rb_cObject);
    ATTR(rb_cVector, x);
    ATTR(rb_cVector, y);
    ATTR(rb_cVector, z);
}

#include "binding-util.h"

void fmodCoreBindingInit() {
    VALUE rb_mFMOD = rb_define_module("FMOD");

    VALUE rb_mFMOD_Core = rb_define_module_under(rb_mFMOD, "Core");
    VALUE rb_mFMOD_System = rb_define_module_under(rb_mFMOD_Core, "System");
}

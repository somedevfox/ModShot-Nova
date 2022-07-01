#ifndef COMMON_STRUCTS_H
#define COMMON_STRUCTS_H

#include "binding-util.h"

#define DEFINE_NAMED_ATTR(type, name, path, vtype, args, convert) \
    RB_METHOD(type##get##name)                                    \
    {                                                             \
        RB_UNUSED_PARAM;                                          \
        type *p = getPrivateData<type>(self);                     \
        return convert(p->path);                                  \
    }                                                             \
                                                                  \
    RB_METHOD(type##set##name)                                    \
    {                                                             \
        vtype v;                                                  \
        rb_get_args(argc, argv, args, &v RB_ARG_END);             \
        type *p = getPrivateData<type>(self);                     \
        p->path = v;                                              \
        return Qnil;                                              \
    }

#define DEFINE_NAMED_ATTR_INT(type, name, path) \
    DEFINE_NAMED_ATTR(type, name, path, int, "i", INT2NUM)

#define DEFINE_NAMED_ATTR_FLOAT(type, name, path) \
    DEFINE_NAMED_ATTR(type, name, path, float, "f", DBL2NUM)

#define DEFINE_NAMED_ATTR_STR(type, name, path, len)                                             \
    RB_METHOD(type##get##name)                                                                   \
    {                                                                                            \
        RB_UNUSED_PARAM;                                                                         \
        type *p = getPrivateData<type>(self);                                                    \
        return rb_str_new_cstr((char *)p->path);                                                 \
    }                                                                                            \
                                                                                                 \
    RB_METHOD(type##set##name)                                                                   \
    {                                                                                            \
        char *v;                                                                                 \
        rb_get_args(argc, argv, "z", &v RB_ARG_END);                                             \
        type *p = getPrivateData<type>(self);                                                    \
        if (strlen(v) > len)                                                                     \
        {                                                                                        \
            rb_raise(rb_eArgError, "The length of the string may not exceed %d bytes", len - 1); \
        }                                                                                        \
        strcpy((char *)p->path, v);                                                              \
        return Qnil;                                                                             \
    }

#define DEFINE_ATTR_INT(type, name) DEFINE_NAMED_ATTR_INT(type, name, name)
#define DEFINE_ATTR_FLOAT(type, name) DEFINE_NAMED_ATTR_FLOAT(type, name, name)
#define DEFINE_ATTR_STR(type, name, len) DEFINE_NAMED_ATTR_STR(type, name, name, len)

#define EXPOSE_ATTRIBUTE(klass, type, name)           \
    _rb_define_method(klass, #name, type##get##name); \
    _rb_define_method(klass, #name "=", type##set##name);

DECL_TYPE(FMOD_GUID);

#endif
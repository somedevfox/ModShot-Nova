#ifndef COMMON_STRUCTS_H
#define COMMON_STRUCTS_H

#include "binding-util.h"

#define DEFINE_NAMED_ATTR(type, name, path, convert) \
    RB_METHOD(type##get##name)                       \
    {                                                \
        RB_UNUSED_PARAM;                             \
        type *p = getPrivateData<type>(self);        \
        return convert(p->path);                     \
    }

#define DEFINE_NAMED_ATTR_INT(type, name, path) \
    DEFINE_NAMED_ATTR(type, name, path, INT2NUM)

#define DEFINE_NAMED_ATTR_FLOAT(type, name, path) \
    DEFINE_NAMED_ATTR(type, name, path, DBL2NUM)

#define DEFINE_NAMED_ATTR_UINT(type, name, path) \
    DEFINE_NAMED_ATTR(type, name, path, UINT2NUM)

#define DEFINE_NAMED_ATTR_STR(type, name, path)  \
    RB_METHOD(type##get##name)                   \
    {                                            \
        RB_UNUSED_PARAM;                         \
        type *p = getPrivateData<type>(self);    \
        return rb_str_new_cstr((char *)p->path); \
    }

#define DEFINE_NAMED_ATTR_NOWRAP(type, name, path, wrap)  \
    RB_METHOD(type##get##name)                            \
    {                                                     \
        RB_UNUSED_PARAM;                                  \
        type *p = getPrivateData<type>(self);             \
        VALUE ret = rb_class_new_instance(0, NULL, wrap); \
        setPrivateData(ret, &p->path);                     \
        return ret;                                       \
    }

#define DEFINE_NAMED_ATTR_WRAP(type, name, path, wrap, wrapklass) \
    RB_METHOD(type##get##name)                                    \
    {                                                             \
        RB_UNUSED_PARAM;                                          \
        type *p = getPrivateData<type>(self);                     \
        VALUE ret = rb_class_new_instance(0, NULL, wrapp);        \
        setPrivateData(ret, new wrapklass(p->path));              \
        return ret;                                               \
    }

#define DEFINE_ATTR_INT(type, name) DEFINE_NAMED_ATTR_INT(type, name, name)
#define DEFINE_ATTR_FLOAT(type, name) DEFINE_NAMED_ATTR_FLOAT(type, name, name)
#define DEFINE_ATTR_STR(type, name) DEFINE_NAMED_ATTR_STR(type, name, name)
#define DEFINE_ATTR_UNINT(type, name) DEFINE_NAMED_ATTR_UINT(type, name, name)
#define DEFINE_ATTR_NOWRAP(type, name, wrap) \
    DEFINE_NAMED_ATTR_NOWRAP(type, name, name, wrap)
#define DEFINE_ATTR_WRAP(type, name, wrap, wrapklass) \
    DEFINE_NAMED_ATTR_WRAP(type, name, name, wrap, wrapklass)

#define EXPOSE_ATTRIBUTE(klass, type, name) \
    _rb_define_method(klass, #name, type##get##name);

DECL_TYPE(FMOD_GUID);
DECL_TYPE(FMOD_STUDIO_MEMORY_USAGE);
DECL_TYPE(FMOD_STUDIO_PARAMETER_DESCRIPTION);

#endif
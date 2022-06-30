#ifndef FMOD_BINDINGS_H
#define FMOD_BINDINGS_H

#include "binding-util.h"
#include <fmod_studio.h>

//* Bool to ruby converter
#define BOOL2RB(val) ((val) ? Qtrue : Qfalse)
#define RB2BOOL(val) (val == Qtrue)

/*
 * We use wrapper classes since we can't set the private data
 * of the ruby object using setPrivateData<Klass>(klass, self)
 * with most FMOD types (Like FMOD_STUDIO_BANK, etc) since
 * their members are not defined anywhere
 */

#define DEF_FMOD_WRAPPER(Klass, wrap_type) \
    class Klass                            \
    {                                      \
    public:                                \
        wrap_type *p;                      \
                                           \
        Klass(wrap_type *p)                \
            : p(p) {}                      \
                                           \
        ~Klass()                           \
        {                                  \
            /* free(p); */                 \
        }                                  \
    };

/*
* Return result macros
* These macros cover returning results of function calls in fmod
* Most functions have 2 (or should have 2) return values,
* ordered by an FMOD_RESULT and then the actual return value.
*
* FMOD_RESULT_WRAP is for when we need to wrap an FMOD struct pointer
! but the struct has no defined members (I.e FMOD_STUDIO_BANK)
*
* FMOD_RESULT_CONVERT is for when we need to convert the return value
* using a standard ruby conversion macro (like INT2NUM)
*
* FMOD_RESULT_NO_WRAP is for when we need to wrap an FMOD struct pointer,
! but the struct has defined members (I.e. FMOD_GUID)
*/
#define FMOD_RESULT_BASE             \
    VALUE return_ary = rb_ary_new(); \
    rb_ary_push(return_ary, INT2NUM(result));

#define FMOD_RESULT_RET return return_ary;

#define FMOD_RESULT_WRAP(val, wrap)                                    \
    FMOD_RESULT_BASE                                                   \
    if (val)                                                           \
    {                                                                  \
        VALUE return_val = rb_class_new_instance(0, NULL, rb_c##wrap); \
        setPrivateData(return_val, new wrap(val));                     \
        rb_ary_push(return_ary, return_val);                           \
    }                                                                  \
    FMOD_RESULT_RET

#define FMOD_RESULT_CONVERT(val, convert)       \
    FMOD_RESULT_BASE                            \
    if (val)                                    \
    {                                           \
        rb_ary_push(return_array, convert(val)) \
    }                                           \
    FMOD_RESULT_RET

#define FMOD_RESULT_NO_WRAP(val, klass)                           \
    FMOD_RESULT_BASE                                              \
    if (val)                                                      \
    {                                                             \
        VALUE return_val = rb_class_new_instance(0, NULL, klass); \
        setPrivateData(return_val, val);                          \
        rb_ary_push(return_ary, return_val);                      \
    }                                                             \
    FMOD_RESULT_RET

//? Define wrapper for Bank
DEF_FMOD_WRAPPER(Bank, FMOD_STUDIO_BANK);
//? Define ruby data type for Bank
DECL_TYPE(Bank);

// * Modules and classes to be defined under
extern VALUE rb_mFMOD;
extern VALUE rb_mFMOD_Core;
extern VALUE rb_mFMOD_Studio;

extern VALUE rb_cBank;

void bindFmodStudioBank();
void bindFmodStudioSystem();

#endif

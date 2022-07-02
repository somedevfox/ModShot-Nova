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
        ~Klass();                          \
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
* FMOD_RESULT_VALUE is for when we want to add a VALUE to the
* return array.
*
* FMOD_RESULT_NO_CHECK is for when we are almost always returning a value
* and the value could be 0, it also acts like FMOD_RESULT_CONVERT
*
* FMOD_RESULT_NO_WRAP is for when we need to wrap an FMOD struct pointer,
! but the struct has defined members (I.e. FMOD_GUID)
*
* FMOD_RESULT_SIMPLE just returns the result. That's it.
*/
#define FMOD_RESULT_BASE             \
    VALUE return_ary = rb_ary_new(); \
    rb_ary_push(return_ary, INT2NUM(result));

#define FMOD_RESULT_RET return return_ary;

#define FMOD_RESULT_WRAP(val, wrap)                                    \
    FMOD_RESULT_BASE                                                   \
    if (val || result == FMOD_OK)                                      \
    {                                                                  \
        VALUE return_val = rb_class_new_instance(0, NULL, rb_c##wrap); \
        setPrivateData(return_val, new wrap(val));                     \
        rb_ary_push(return_ary, return_val);                           \
    }                                                                  \
    FMOD_RESULT_RET

#define FMOD_RESULT_CONVERT(val, convert)      \
    FMOD_RESULT_BASE                           \
    if (val || result == FMOD_OK)              \
    {                                          \
        rb_ary_push(return_ary, convert(val)); \
    }                                          \
    FMOD_RESULT_RET

#define FMOD_RESULT_NO_CHECK(val, convert) \
    FMOD_RESULT_BASE                       \
    rb_ary_push(return_ary, convert(val)); \
    FMOD_RESULT_RET

#define FMOD_RESULT_VALUE(val)        \
    FMOD_RESULT_BASE                  \
    if (val || result == FMOD_OK)     \
    {                                 \
        rb_ary_push(return_ary, val); \
    }                                 \
    FMOD_RESULT_RET

#define FMOD_RESULT_NO_WRAP(val, klass)                           \
    FMOD_RESULT_BASE                                              \
    if (val || result == FMOD_OK)                                 \
    {                                                             \
        VALUE return_val = rb_class_new_instance(0, NULL, klass); \
        setPrivateData(return_val, val);                          \
        rb_ary_push(return_ary, return_val);                      \
    }                                                             \
    FMOD_RESULT_RET

#define FMOD_RESULT_SIMPLE \
    FMOD_RESULT_BASE       \
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
extern VALUE rb_cGUID;

void bindFmodStudioBank();
void bindFmodStudioSystem();
void bindFmodStudioStructs();

//? These functions are common, so we share them
//? in the header
//? Why do these need to be inline? I have no idea!
inline RB_METHOD(fmodGetUserData)
{
    RB_UNUSED_PARAM;

    //? Get wrapper
    Bank *b = getPrivateData<Bank>(self);
    //? Set up variables
    VALUE data;

    //? Call function, I hate the void** cast but whatever
    FMOD_RESULT result = FMOD_Studio_Bank_GetUserData(
        b->p, (void **) &data);

    //? Return the result, you know the drill
    FMOD_RESULT_BASE;
    if (data) {
        rb_ary_push(return_ary, data);
    }
    FMOD_RESULT_RET;
}

inline RB_METHOD(fmodSetUserData)
{
    VALUE arg;
    rb_get_args(argc, argv, "o", &arg RB_ARG_END);

    //? Set the arg as an instance variable so ruby doesn't garbage
    //? collect
    rb_iv_set(self, "userdata_dont_touch_please", arg);
    //! If someone touches this I'm gonna yell
    //! because they won't know why their user data mutated
    //! into something else

    //? Get wrapper
    Bank *b = getPrivateData<Bank>(self);

    //? Call function
    FMOD_RESULT result = FMOD_Studio_Bank_SetUserData(
        b->p, (void *)arg);

    //? Only need to return the result!
    FMOD_RESULT_SIMPLE;
}

#endif

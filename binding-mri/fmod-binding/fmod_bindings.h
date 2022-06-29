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
            free(p);                       \
        }                                  \
    };

/*
* Usage:
* Klass -> EventDescription (name of class to be defined)
* wrap_type -> FMOD_STUDIO_EVENTDESCRIPTION (Type we are wrapping)
? DEF_FMOD_WRAPPER(EventDescription, FMOD_STUDIO_EVENTDESCRIPTION);
*/

//? Define wrapper for Bank
DEF_FMOD_WRAPPER(Bank, FMOD_STUDIO_BANK);
//? Define ruby data type for Bank
DECL_TYPE(Bank);

// * Modules and classes to be defined under
extern VALUE rb_mFMOD;
extern VALUE rb_mFMOD_Core;
extern VALUE rb_mFMOD_Studio;

void bindFmodStudioBank();

#endif

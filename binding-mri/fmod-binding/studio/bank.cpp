#include "binding-util.h"
#include "fmod_bindings.h"
#include "debugwriter.h"

/*
? Bank bindings
? Why is RB_UNUSED_PARAM used everywhere? I have no idea!
? MKXP has them *everywhere*, and I don't know why!
?
? Most of these abuse macros and the C preprocessor to
? shrink code. You may want to read up on these macros
? first.
*/

DEF_TYPE(Bank);
/*
? Custom bank destructor
? Automatically unloads the bank when
? the bank is garbage collected from ruby
*/
Bank::~Bank()
{
    Debug() << "Warning: auto unloading bank (garbage collected from ruby?)";
    Debug() << "Bank unloading result:" << FMOD_Studio_Bank_Unload(p);
    /* free(p) */
}

VALUE rb_cBank = Qnil;

RB_METHOD(bankIsValid)
{
    RB_UNUSED_PARAM;

    //? Get wrapper
    Bank *b = getPrivateData<Bank>(self);

    //? Get result of IsValid (an int for some reason?) and convert it to a ruby bool
    //? BOOL2RB is defined in fmod_bindings.h since ruby doesn't ship it
    return BOOL2RB(FMOD_Studio_Bank_IsValid(b->p));
}

RB_METHOD(bankGetID)
{
    RB_UNUSED_PARAM;

    //? Get wrapper
    Bank *b = getPrivateData<Bank>(self);
    //? Create FMOD_GUID struct
    FMOD_GUID *guid = new FMOD_GUID();

    //? Get the GUID
    FMOD_RESULT result = FMOD_Studio_Bank_GetID(b->p, guid);

    //? Return the result and guid
    FMOD_RESULT_NO_WRAP(guid, rb_cGUID);
}

RB_METHOD(bankGetPath)
{
    RB_UNUSED_PARAM;

    //? Get wrapper
    Bank *b = getPrivateData<Bank>(self);
    int retrieved;
    char *path = NULL;
    //? Get the size of the path, this function modifies retrieved
    FMOD_RESULT result = FMOD_Studio_Bank_GetPath(b->p, NULL, 0, &retrieved);
    //? If function was successful
    if (result == FMOD_OK)
    {
        //? Allocate memory for the string based on the size of the path
        //? We got from the first function call
        path = new char[retrieved];
        /*
        ! This could be a memory leak if ruby doesn't clean
        ! the string up
        */

        //? Finally set the path
        result = FMOD_Studio_Bank_GetPath(b->p, path, retrieved, &retrieved);
    }

    //? Return the path
    FMOD_RESULT_CONVERT(path, rb_str_new_cstr);
}

RB_METHOD(bankUnload)
{
    RB_UNUSED_PARAM;

    //? Get wrapper
    Bank *b = getPrivateData<Bank>(self);

    //? Unload bank
    FMOD_RESULT result = FMOD_Studio_Bank_Unload(b->p);

    //? Return only the result
    FMOD_RESULT_SIMPLE;
}

RB_METHOD(bankLoadSampleData)
{
    RB_UNUSED_PARAM;

    //? Get wrapper
    Bank *b = getPrivateData<Bank>(self);

    //? Load sample data
    FMOD_RESULT result = FMOD_Studio_Bank_LoadSampleData(b->p);

    //? Return only the result
    FMOD_RESULT_SIMPLE;
}

RB_METHOD(bankUnloadSampleData)
{
    RB_UNUSED_PARAM;

    //? Get wrapper
    Bank *b = getPrivateData<Bank>(self);

    //? Unload sample data
    FMOD_RESULT result = FMOD_Studio_Bank_UnloadSampleData(b->p);

    //? Return only the result
    FMOD_RESULT_SIMPLE;
}

RB_METHOD(bankGetLoadingState)
{
    RB_UNUSED_PARAM;

    //? Get wrapper
    Bank *b = getPrivateData<Bank>(self);
    FMOD_STUDIO_LOADING_STATE state;

    //? Call function
    FMOD_RESULT result = FMOD_Studio_Bank_GetLoadingState(b->p, &state);

    //? Return result & value
    FMOD_RESULT_CONVERT(state, INT2NUM);
}

RB_METHOD(bankGetSampleLoadingState)
{
    RB_UNUSED_PARAM;

    //? Get wrapper
    Bank *b = getPrivateData<Bank>(self);
    FMOD_STUDIO_LOADING_STATE state;

    //? Call function
    FMOD_RESULT result = FMOD_Studio_Bank_GetSampleLoadingState(b->p, &state);

    //? Return result & value
    FMOD_RESULT_CONVERT(state, INT2NUM);
}

RB_METHOD(bankGetStringCount)
{
    RB_UNUSED_PARAM;
    //? Get wrapper
    Bank *b = getPrivateData<Bank>(self);
    int count;

    //? Call function
    FMOD_RESULT result = FMOD_Studio_Bank_GetStringCount(b->p, &count);

    //? Return result & value
    FMOD_RESULT_CONVERT(count, INT2NUM);
}

//! Oh boy
RB_METHOD(bankGetStringInfo) {
    //? Get the index we are using
    int index;
    rb_get_args(argc, argv, "i", &index RB_ARG_END);

    //? Get wrapper
    Bank *b = getPrivateData<Bank>(self);
    //? Set up variables, guid and path are intentionally null
    //? so we don't allocate memory for them if the function is
    //? unsuccessful
    int retrieved;
    char* path = NULL;
    FMOD_GUID *guid = NULL;

    FMOD_RESULT result = FMOD_Studio_Bank_GetStringInfo(
        b->p, index, NULL, NULL, 0, &retrieved
    );

    //? If function was successful
    if (result == FMOD_OK) {
        //? Allocate memory for the string like in GetPath
        path = new char[retrieved];
        //? Create the GUID
        guid = new FMOD_GUID();

        //? Finally call the function for real
        //? Oh boy that was horrible
        result = FMOD_Studio_Bank_GetStringInfo(
            b->p, index, guid, path, retrieved, &retrieved
        );
    }

    //? Not many functions need more than 1 return value
    //? and the functions that do are specialized, so no
    //? fancy macro here unfortunately
    FMOD_RESULT_BASE;
    if (guid) {
        VALUE return_val = rb_class_new_instance(0, NULL, rb_cGUID);
        setPrivateData(return_val, guid);
        rb_ary_push(return_ary, return_val);
    }
    if (path) {
        rb_ary_push(return_ary, rb_str_new_cstr(path));
    }
    //? Hell is over!
    FMOD_RESULT_RET;
}

FMOD_USERDATA_FUNC(FMOD_Studio_Bank, Bank);

void bindFmodStudioBank()
{
    rb_cBank = rb_define_class_under(rb_mFMOD_Studio, "Bank", rb_cObject);
    rb_define_alloc_func(rb_cBank, classAllocate<&BankType>);

    _rb_define_method(rb_cBank, "is_valid", bankIsValid);
    _rb_define_method(rb_cBank, "get_id", bankGetID);
    _rb_define_method(rb_cBank, "get_path", bankGetPath);
    _rb_define_method(rb_cBank, "unload", bankUnload);
    _rb_define_method(rb_cBank, "load_sample_data", bankLoadSampleData);
    _rb_define_method(rb_cBank, "unload_sample_data", bankUnloadSampleData);
    _rb_define_method(rb_cBank, "get_loading_state", bankGetLoadingState);
    _rb_define_method(rb_cBank, "get_sample_loading_state", bankGetSampleLoadingState);
    _rb_define_method(rb_cBank, "get_string_count", bankGetStringCount);
    _rb_define_method(rb_cBank, "get_string_info", bankGetStringInfo);
    _rb_define_method(rb_cBank, "get_user_data", fmodGetUserData);
    _rb_define_method(rb_cBank, "set_user_data", fmodSetUserData);
}

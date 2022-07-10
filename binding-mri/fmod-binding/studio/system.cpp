#include "binding-util.h"
#include "fmod_bindings.h"
#ifdef AUTO_CLEAN_FMOD
#include "debugwriter.h"
#include <fmod_errors.h>
#endif

DEF_TYPE(StudioSystem);
StudioSystem::~StudioSystem()
{
    #ifdef AUTO_CLEAN_FMOD
    Debug() << "Warning: auto releasing studio system (garbage collected?)";
    Debug() << "Studio system release result: " << FMOD_ErrorString(FMOD_Studio_System_Release(p));
    #endif
}

FMOD_VALID_FUNC(FMOD_Studio_System, StudioSystem);

RB_METHOD(systemSetAdvancedSettings) {
    VALUE rb_settings;
    rb_get_args(argc, argv, "o", &rb_settings);

    FMOD_STUDIO_ADVANCEDSETTINGS settings = rb2FMOD_STUDIO_ADVANCEDSETTINGS(rb_settings);
    StudioSystem *b = getPrivateData<StudioSystem>(self);

    FMOD_RESULT result = FMOD_Studio_System_SetAdvancedSettings(b->p, &settings);

    FMOD_RESULT_SIMPLE;
}

RB_METHOD(systemGetAdvancedSettings) {
    RB_UNUSED_PARAM;

    FMOD_STUDIO_ADVANCEDSETTINGS settings = FMOD_STUDIO_ADVANCEDSETTINGS();
    StudioSystem *b = getPrivateData<StudioSystem>(self);

    FMOD_RESULT result = FMOD_Studio_System_GetAdvancedSettings(b->p, &settings);

    FMOD_RESULT_NO_WRAP(settings, FMOD_STUDIO_ADVANCEDSETTINGS);
}

//! Need to define custom new function because Ruby
//! does not allow for custom return values from initialize.
RB_METHOD(systemNew) {
    int maxchannels;
    int studioflags;
    int flags;
    rb_get_args(argc, argv, "iii", &maxchannels, &studioflags, &flags RB_ARG_END);

    FMOD_STUDIO_SYSTEM *system = NULL;
    FMOD_RESULT result = FMOD_Studio_System_Create(&system, FMOD_VERSION);

    if (result == FMOD_OK) {
        result = FMOD_Studio_System_Initialize(system, maxchannels, studioflags, flags, NULL);
    }

    FMOD_RESULT_BASE;
    if (result == FMOD_OK) {
        VALUE self = rb_obj_alloc(rb_cStudioSystem); //! Manually allocate the object
        setPrivateData(self, new StudioSystem(system));
        rb_ary_push(return_ary, self);
    }
    FMOD_RESULT_RET;
}

RB_METHOD(systemRelease) {
    RB_UNUSED_PARAM;

    StudioSystem *b = getPrivateData<StudioSystem>(self);

    FMOD_RESULT result = FMOD_Studio_System_Release(b->p);

    FMOD_RESULT_SIMPLE;
}

RB_METHOD(systemUpdate) {
    RB_UNUSED_PARAM;

    StudioSystem *b = getPrivateData<StudioSystem>(self);

    FMOD_RESULT result = FMOD_Studio_System_Update(b->p);

    FMOD_RESULT_SIMPLE;
}

RB_METHOD(systemGetCoreSystem) {
    RB_UNUSED_PARAM;

    StudioSystem *b = getPrivateData<StudioSystem>(self);
    FMOD_SYSTEM *system = NULL;

    FMOD_RESULT result = FMOD_Studio_System_GetCoreSystem(b->p, &system);

    FMOD_RESULT_WRAP(system, System);
}

#define GET_PATHORID \
char* pathorid; \
rb_get_args(argc, argv, "z", &pathorid RB_ARG_END);

RB_METHOD(systemGetEvent) {
    GET_PATHORID;

    StudioSystem *b = getPrivateData<StudioSystem>(self);
    FMOD_STUDIO_EVENTDESCRIPTION *event = NULL;

    FMOD_RESULT result = FMOD_Studio_System_GetEvent(b->p, pathorid, &event);

    FMOD_RESULT_WRAP(event, EventDescription);
}

RB_METHOD(systemGetBus) {
    GET_PATHORID;

    StudioSystem *b = getPrivateData<StudioSystem>(self);
    FMOD_STUDIO_BUS *bus = NULL;

    FMOD_RESULT result = FMOD_Studio_System_GetBus(b->p, pathorid, &bus);

    FMOD_RESULT_WRAP(bus, Bus);
}

RB_METHOD(systemGetVCA) {
    GET_PATHORID;

    StudioSystem *b = getPrivateData<StudioSystem>(self);
    FMOD_STUDIO_VCA *vca = NULL;

    FMOD_RESULT result = FMOD_Studio_System_GetVCA(b->p, pathorid, &vca);

    FMOD_RESULT_WRAP(vca, VCA);
}

RB_METHOD(systemGetBank) {
    GET_PATHORID;

    StudioSystem *b = getPrivateData<StudioSystem>(self);
    FMOD_STUDIO_BANK *bank = NULL;

    FMOD_RESULT result = FMOD_Studio_System_GetBank(b->p, pathorid, &bank);

    FMOD_RESULT_WRAP(bank, Bank);
}

#define GET_ID \
VALUE rb_id; \
rb_get_args(argc, argv, "o", &rb_id RB_ARG_END); \
FMOD_GUID id = rb2FMOD_GUID(rb_id);

RB_METHOD(systemGetEventByID) {
    GET_ID;

    StudioSystem *b = getPrivateData<StudioSystem>(self);
    FMOD_STUDIO_EVENTDESCRIPTION *event = NULL;

    FMOD_RESULT result = FMOD_Studio_System_GetEventByID(b->p, &id, &event);

    FMOD_RESULT_WRAP(event, EventDescription);
}

RB_METHOD(systemGetBusByID) {
    GET_ID;

    StudioSystem *b = getPrivateData<StudioSystem>(self);
    FMOD_STUDIO_BUS *bus = NULL;

    FMOD_RESULT result = FMOD_Studio_System_GetBusByID(b->p, &id, &bus);

    FMOD_RESULT_WRAP(bus, Bus);
}

RB_METHOD(systemGetVCAByID) {
    GET_ID;

    StudioSystem *b = getPrivateData<StudioSystem>(self);
    FMOD_STUDIO_VCA *vca = NULL;

    FMOD_RESULT result = FMOD_Studio_System_GetVCAByID(b->p, &id, &vca);

    FMOD_RESULT_WRAP(vca, VCA);
}

RB_METHOD(systemGetBankByID) {
    GET_ID;

    StudioSystem *b = getPrivateData<StudioSystem>(self);
    FMOD_STUDIO_BANK *bank = NULL;

    FMOD_RESULT result = FMOD_Studio_System_GetBankByID(b->p, &id, &bank);

    FMOD_RESULT_WRAP(bank, Bank);
}

RB_METHOD(systemGetSoundInfo) {
    char* key;
    rb_get_args(argc, argv, "z", &key RB_ARG_END);

    StudioSystem *b = getPrivateData<StudioSystem>(self);
    FMOD_STUDIO_SOUND_INFO info = FMOD_STUDIO_SOUND_INFO();

    FMOD_RESULT result = FMOD_Studio_System_GetSoundInfo(b->p, key, &info);

    FMOD_RESULT_NO_WRAP(info, FMOD_STUDIO_SOUND_INFO);
}

RB_METHOD(systemGetParameterDescriptionName) {
    char* name;
    rb_get_args(argc, argv, "z", &name RB_ARG_END);

    StudioSystem *b = getPrivateData<StudioSystem>(self);
    FMOD_STUDIO_PARAMETER_DESCRIPTION description = FMOD_STUDIO_PARAMETER_DESCRIPTION();

    FMOD_RESULT result = FMOD_Studio_System_GetParameterDescriptionByName(b->p, name, &description);

    FMOD_RESULT_NO_WRAP(description, FMOD_STUDIO_PARAMETER_DESCRIPTION);
}

RB_METHOD(systemGetParameterDescriptionID) {
    VALUE rb_id;
    rb_get_args(argc, argv, "o", &rb_id RB_ARG_END);

    FMOD_STUDIO_PARAMETER_ID id = rb2FMOD_STUDIO_PARAMETER_ID(rb_id);
    StudioSystem *b = getPrivateData<StudioSystem>(self);
    FMOD_STUDIO_PARAMETER_DESCRIPTION description = FMOD_STUDIO_PARAMETER_DESCRIPTION();

    FMOD_RESULT result = FMOD_Studio_System_GetParameterDescriptionByID(b->p, id, &description);

    FMOD_RESULT_NO_WRAP(description, FMOD_STUDIO_PARAMETER_DESCRIPTION);
}

RB_METHOD(systemGetParameterLabelByName) {
    char* name;
    int labelindex;
    rb_get_args(argc, argv, "zi", &name, &labelindex RB_ARG_END);

    StudioSystem *b = getPrivateData<StudioSystem>(self);
    char* label = NULL;
    int retrieved;

    FMOD_RESULT result = FMOD_Studio_System_GetParameterLabelByName(b->p, name, labelindex, NULL, 0 , &retrieved);

    if (result = FMOD_OK) {
        label = new char[retrieved];

        result = FMOD_Studio_System_GetParameterLabelByName(b->p, name, labelindex, label, retrieved, &retrieved);
    }

    FMOD_RESULT_CONVERT(label, rb_str_new_cstr);
}

RB_METHOD(systemGetParameterLabelByID) {
    VALUE rb_id;
    int labelindex;
    rb_get_args(argc, argv, "zoi", &rb_id, &labelindex RB_ARG_END);

    FMOD_STUDIO_PARAMETER_ID id = rb2FMOD_STUDIO_PARAMETER_ID(rb_id);
    StudioSystem *b = getPrivateData<StudioSystem>(self);
    char* label = NULL;
    int retrieved;

    FMOD_RESULT result = FMOD_Studio_System_GetParameterLabelByID(b->p, id, labelindex, NULL, 0 , &retrieved);

    if (result = FMOD_OK) {
        label = new char[retrieved];

        result = FMOD_Studio_System_GetParameterLabelByID(b->p, id, labelindex, label, retrieved, &retrieved);
    }

    FMOD_RESULT_CONVERT(label, rb_str_new_cstr);
}

RB_METHOD(systemLoadBankFile) {
    const char* filename;
    int flags;

    rb_get_args(argc, argv, "zi", &filename, &flags RB_ARG_END);

    StudioSystem *b = getPrivateData<StudioSystem>(self);

    //? Initialize bank as a NULL pointer
    FMOD_STUDIO_BANK* bank = NULL;
    //? Load bank
    FMOD_RESULT result = FMOD_Studio_System_LoadBankFile(
        b->p, filename, flags, &bank
    );

    //? Wrap bank result and return it
    FMOD_RESULT_WRAP(bank, Bank);
}

VALUE rb_cStudioSystem = Qnil;

void bindFmodStudioSystem() {
    rb_cStudioSystem = rb_define_class_under(rb_mFMOD_Studio, "System", rb_cObject);
    rb_define_alloc_func(rb_cStudioSystem, classAllocate<&StudioSystemType>);

    _rb_define_module_function(rb_cStudioSystem, "new", systemNew);
    _rb_define_method(rb_cStudioSystem, "is_valid", fmodIsValid);
    _rb_define_method(rb_cStudioSystem, "set_advanced_settings", systemSetAdvancedSettings);
    _rb_define_method(rb_cStudioSystem, "get_advanced_settings", systemGetAdvancedSettings);
    _rb_define_method(rb_cStudioSystem, "release", systemRelease);
    _rb_define_method(rb_cStudioSystem, "update", systemUpdate);
    _rb_define_method(rb_cStudioSystem, "get_core_system", systemGetCoreSystem);
    _rb_define_method(rb_cStudioSystem, "get_event", systemGetEvent);
    _rb_define_method(rb_cStudioSystem, "get_bus", systemGetBus);
    _rb_define_method(rb_cStudioSystem, "get_vca", systemGetVCA);
    _rb_define_method(rb_cStudioSystem, "get_bank", systemGetBank);
    _rb_define_method(rb_cStudioSystem, "get_event_by_id", systemGetEventByID);
    _rb_define_method(rb_cStudioSystem, "get_bus_by_id", systemGetBusByID);
    _rb_define_method(rb_cStudioSystem, "get_vca_by_id", systemGetVCAByID);
    _rb_define_method(rb_cStudioSystem, "get_bank_by_id", systemGetBankByID);
    _rb_define_method(rb_cStudioSystem, "get_sound_info", systemGetSoundInfo);
    _rb_define_method(rb_cStudioSystem, "get_parameter_description_by_name", systemGetParameterDescriptionName);
    _rb_define_method(rb_cStudioSystem, "get_parameter_description_by_id", systemGetParameterDescriptionID);
    _rb_define_method(rb_cStudioSystem, "get_parameter_label_by_name", systemGetParameterLabelByName);
    _rb_define_method(rb_cStudioSystem, "get_parameter_label_by_id", systemGetParameterLabelByID);
    _rb_define_method(rb_cStudioSystem, "load_bank_file", systemLoadBankFile);
}

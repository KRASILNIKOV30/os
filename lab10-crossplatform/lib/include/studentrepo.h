#pragma once
// И версию с макросами, где pragma once нет

#ifdef STUDENTREPO_LIBRARY_BUILD
    // Building the library
    #ifdef _WIN32
        // Use the Windows-specific export attribute
        #define STUDENTREPO_LIBRARY_EXPORT __declspec(dllexport)
    #elif __GNUC__ >= 4
        // Use the GCC-specific export attribute
        #define STUDENTREPO_LIBRARY_EXPORT __attribute__((visibility("default")))
    #else
        // Assume that no export attributes are needed
        #define STUDENTREPO_LIBRARY_EXPORT
    #endif
#else
    // Using (including) the library
    #ifdef _WIN32
        // Use the Windows-specific import attribute
        #define STUDENTREPO_LIBRARY_EXPORT __declspec(dllimport)
    #else
        // Assume that no import attributes are needed
        #define STUDENTREPO_LIBRARY_EXPORT
    #endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include <cstddef>

struct STUDENTREPO_LIBRARY_EXPORT SRString
{
    const char* chars;
    size_t length;
};

struct STUDENTREPO_LIBRARY_EXPORT SRStudent 
{
    unsigned id;
    SRString name;
    SRString branch;
    unsigned enteringYear;
};

// везде можно указатели
//Передавать id репозитория
STUDENTREPO_LIBRARY_EXPORT bool SRMakeString(const char* chars, SRString& str);

STUDENTREPO_LIBRARY_EXPORT void SRFreeString(SRString* str);

STUDENTREPO_LIBRARY_EXPORT bool SRMakeStudent(unsigned id, const char* name, const char* branch, unsigned enteringYear, SRStudent& s);

STUDENTREPO_LIBRARY_EXPORT void SRFreeStudent(SRStudent* s);

STUDENTREPO_LIBRARY_EXPORT bool SRAddStudent(SRStudent student);

STUDENTREPO_LIBRARY_EXPORT bool SRRemoveStudent(unsigned id);

STUDENTREPO_LIBRARY_EXPORT void SRForEachStudentWithName(SRString name, bool (*callback)(const SRStudent*, void*), void* context);

STUDENTREPO_LIBRARY_EXPORT const SRStudent* SRGetStudent(unsigned id);

#ifdef __cplusplus
}
#endif
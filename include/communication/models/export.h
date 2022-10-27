#ifndef KJARNI_EXPORT_H
#define KJARNI_EXPORT_H

#ifdef _WIN32
#if defined KJARNI_DLL
#define KJARNI_EXPORT __declspec(dllexport)
#else
#define KJARNI_EXPORT __declspec(dllimport)
#endif
#else
#define KJARNI_EXPORT
#endif

#endif

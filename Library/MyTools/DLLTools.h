#pragma once


#define DLL_EXPORT_API __declspec(dllexport)
#define DLL_IMPORT_API __declspec(dllimport)



#ifdef DLL_EXPORT
#define DLL_API DLL_EXPORT_API
#elif DLL_IMPORT
#define DLL_API DLL_IMPORT_API
#else
#define DLL_API
#endif
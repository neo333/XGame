#ifndef __CONFIG_CORE_XGAME__HPP
#define __CONFIG_CORE_XGAME__HPP

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#define XGAME_WINDOWS_OS

#else

#if  defined(linux) || defined(_linux) || defined(__linux__)
#define DLL_EXPORT		//TODO: controllato?!
#define DLL_IMPORT
#define XGAME_LINUX_OS
#endif

#endif

#ifdef _MSC_VER
#define XGAME_MSVC
#endif


#ifdef XGAME_BUILD_DLL_CORE
#define XGAME_API_CORE DLL_EXPORT
#else
#define XGAME_API_CORE DLL_IMPORT
#endif

#endif
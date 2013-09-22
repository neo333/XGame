#ifndef __CONFIG__HPP
#define __CONFIG__HPP

#ifdef XGAME_BUILD_DLL
#define XGAME_API __declspec(dllexport)
#else
#define XGAME_API __declspec(dllimport)
#endif

#endif
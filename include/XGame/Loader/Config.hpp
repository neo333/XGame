#ifndef __CONFIG_LOADER_XGAME__HPP
#define __CONFIG_LOADER_XGAME__HPP

#include <XGame/Core/Config.hpp>



#ifdef XGAME_BUILD_DLL_LOADER
#define XGAME_API_LOADER DLL_EXPORT
#else
#define XGAME_API_LOADER DLL_IMPORT
#endif

#endif
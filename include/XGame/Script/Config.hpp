#ifndef __CONFIG_SCRIPT_XGAME__HPP
#define __CONFIG_SCRIPT_XGAME__HPP

#include <XGame/Core/Config.hpp>



#ifdef XGAME_BUILD_DLL_SCRIPT
#define XGAME_API_SCRIPT DLL_EXPORT
#else
#define XGAME_API_SCRIPT DLL_IMPORT
#endif

#endif
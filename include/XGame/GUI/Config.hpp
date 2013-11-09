#ifndef __CONFIG_GUI_XGAME__HPP
#define __CONFIG_GUI_XGAME__HPP

#include <XGame/Core/Config.hpp>



#ifdef XGAME_BUILD_DLL_GUI
#define XGAME_API_GUI DLL_EXPORT
#else
#define XGAME_API_GUI DLL_IMPORT
#endif

#endif
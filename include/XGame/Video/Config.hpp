#ifndef __CONFIG_VIDEO_XGAME__HPP
#define __CONFIG_VIDEO_XGAME__HPP

#include <XGame/Core/Config.hpp>



#ifdef XGAME_BUILD_DLL_VIDEO
#define XGAME_API_VIDEO DLL_EXPORT
#else
#define XGAME_API_VIDEO DLL_IMPORT
#endif

#endif
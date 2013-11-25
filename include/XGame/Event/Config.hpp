#ifndef __CONFIG_EVENT_XGAME__HPP
#define __CONFIG_EVENT_XGAME__HPP

#include <XGame/Core/Config.hpp>
#include <list>
#include <SDL2/SDL.h>



#ifdef XGAME_BUILD_DLL_EVENT
#define XGAME_API_EVENT DLL_EXPORT
#else
#define XGAME_API_EVENT DLL_IMPORT
#endif

namespace xgame{
	typedef std::list<SDL_Event> ListEvents;
}

#endif
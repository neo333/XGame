#ifndef __OBJECT_INTERACTIVE__XGAME__HPP
#define __OBJECT_INTERACTIVE__XGAME__HPP

#include <XGame/Event/Config.hpp>
#include <SDL/SDL.h>

namespace xgame{
	class XGAME_API_EVENT ObjectInteractive{
	public:
		//! Distruttore. Deassocia la registrazione presso l'EventManager.
		virtual ~ObjectInteractive();

	protected:
		/*! Questo metodo viene chiamato automaticamente dall'EventManager quando si scatena un evento.*/
		virtual void NotificationEvent(const SDL_Event& event) =0;
		
		//! Costruisce un oggetto interattivo registrandolo presso l'EventManager
		ObjectInteractive();
		
	private:
		friend class EventManager;
	};
}

#endif
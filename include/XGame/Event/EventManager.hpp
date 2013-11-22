#ifndef __EVENT_MANAGER_XGAME__HPP
#define __EVENT_MANAGER_XGAME__HPP

#include <XGame/Event/Config.hpp>
#include <XGame/Core/noncopyable.hpp>
#include <XGame/Event/ObjectInteractive.hpp>
#include <XGame/Event/ObjectDynamic.hpp>
#include <SDL2/SDL.h>
#include <unordered_map>

#ifdef WIN32 
#pragma warning(disable:4251)
#pragma warning(disable:4275)
#endif

namespace xgame{
	class XGAME_API_EVENT EventManager : private noncopyable{
	public:
		//! \return L'istanza globale della classe.
		static inline EventManager& Get_GlobalInstance();

		/*! Registra un oggetto interattivo al gestore di eventi.

			\param [in] object		Un puntatore ad un oggetto interattivo.
			\note					Se l'oggetto è già registrato questo metodo non farà nulla!
		*/
		void RegisterObjectInteractive(ObjectInteractive* const object);

		/*! Deassocia una registrazione fatta in precedenza per un oggetto interattivo.

			\param [in] object		Un puntatore ad un oggetto interattivo che si vuole deassociare.
			\note					Se l'oggetto non è mai stato registrato questo metodo non farà nulla!
		*/
		void UnRegisterObjectInteractive(ObjectInteractive* const object) throw();
		

		/*! Registra un oggetto dinamico al gestore di eventi.

			\param [in] object		Un puntatore ad un oggetto dinamico.
			\note					Se l'oggetto è già registrato questo metodo non farà nulla!
		*/
		void RegisterObjectDynamic(ObjectDynamic* const object);

		/*! Deassocia una registrazione fatta in precedenza per un oggetto dinamico.

			\param [in] object		Un puntatore ad un oggetto dinamico che si vuole deassociare.
			\note					Se l'oggetto non è mai stato registrato questo metodo non farà nulla!
		*/
		void UnRegisterObjectDynamic(ObjectDynamic* const object) throw();



		/*! Chiama varie system-call per verificare tutti gli eventi mandati dal sistema operativo al processo.
			Questa funzione andrebbe chiamata a loop.
			Tutti gli oggetti interattivi veranno notificati tramite la chiamata dell'aposito metodo della loro classe.

			Inoltre successivamente vengono aggioranti tutti gli oggetti dinamici!
		*/
		void UpdateAll();


	private:
		EventManager();
		static EventManager* s_ptrInstance;
		SDL_Event m_event_sdl;
		
		struct InfoObject{
			inline InfoObject(){ }
		};
		typedef std::unordered_map<ObjectInteractive*, InfoObject> RegistroObjectsInteractive;
		typedef std::unordered_map<ObjectDynamic*,InfoObject> RegistroObjectDynamic;

		RegistroObjectsInteractive m_registro_interactive;
		RegistroObjectDynamic m_registro_dynamic;
	};


	inline EventManager& EventManager::Get_GlobalInstance(){
		if(EventManager::s_ptrInstance==nullptr) EventManager::s_ptrInstance = new EventManager();
		return *EventManager::s_ptrInstance;
	}
}

#define UpdateAllEvent() xgame::EventManager::Get_GlobalInstance().UpdateAll()

//TODO: fare una registrazione con filtri per tipologie di eventi da notificare

#endif
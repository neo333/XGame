#ifndef __EVENT_MANAGER_XGAME__HPP
#define __EVENT_MANAGER_XGAME__HPP

#include <XGame/Event/Config.hpp>
#include <XGame/Core/noncopyable.hpp>

namespace xgame{
	class XGAME_API_EVENT EventManager : private noncopyable{
	public:
		//! \return L'istanza globale della classe.
		static inline EventManager& Get_GlobalInstance();

		/*! Preleva un certo numero di eventi mandati dal sistema operativo all'applicazione.
			
			\param [out] out_events			Una lista di eventi alla quale verranno inseriti in testa
											i nuovi eventi.
			\param [in] n_max_pop			Il numero MASSIMO di eventi che si vogliono ricevere.
			
			\return							Il numero di eventi aggiunti dalla chiamata di questo metodo.

			\note							Questa funzione non distrugge gli eventi già presenti nella lista
											di input, ma semplicemente ne aggiungerà di nuovi in testa.
			\note							Questa funzione generalmente non è bloccante (ritorna 0 in caso
											non ci siano eventi), ma diventa bloccante mentre l'utente
											ridimensiona una finestra grafica.
		*/
		const size_t PopEvents_andADDtoList(ListEvents& out_events,const size_t n_max_pop =1);

		//! Distruttore.
		~EventManager() = default;

		//! Costruttore di move eliminato.
		EventManager(EventManager&&) = delete;

		//! Operatore di move eliminato.
		EventManager& operator=(EventManager&&) = delete;


	private:
		EventManager() = default;
		
		static EventManager* s_ptrInstance;
		SDL_Event m_event_sdl;
		
	};


	inline EventManager& EventManager::Get_GlobalInstance(){
		if(EventManager::s_ptrInstance==nullptr) EventManager::s_ptrInstance = new EventManager();
		return *EventManager::s_ptrInstance;
	}
}

#define Eventer xgame::EventManager::Get_GlobalInstance()

#endif
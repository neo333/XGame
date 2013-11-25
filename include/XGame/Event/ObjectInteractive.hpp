#ifndef __OBJECT_INTERACTIVE__XGAME__HPP
#define __OBJECT_INTERACTIVE__XGAME__HPP

#include <XGame/Event/Config.hpp>

namespace xgame{
	class XGAME_API_EVENT ObjectInteractive{
	public:
		//! Costruttore di default.
		ObjectInteractive() = default;

		//! Distruttore. Deassocia la registrazione presso l'EventManager.
		virtual ~ObjectInteractive() = default;

		//! Costruttore di copia.
		ObjectInteractive(const ObjectInteractive&) = default;

		//! Operatore di assegnazione.
		ObjectInteractive& operator=(const ObjectInteractive&) = default;

		/*! Aggiorna l'oggetto interattivo passandogli gli eventi verificatosi.
			\note	Generalmente un'oggetto interattivo cancella l'evento che lo ha interessato dalla lista.
		*/
		virtual void NotificationEvents(ListEvents& events) = 0;
	};
}

#endif
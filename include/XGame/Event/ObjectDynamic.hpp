#ifndef __OBJECT_DYNAMIC_XGAME__HPP
#define __OBJECT_DYNAMIC_XGAME__HPP

#include <XGame/Event/Config.hpp>

namespace xgame{
	class XGAME_API_EVENT ObjectDynamic{
	public:
		//! Costruttore di default.
		ObjectDynamic() = default;

		//! Distruttore.
		virtual ~ObjectDynamic() = default;

		//! Costruttore di copia.
		ObjectDynamic(const ObjectDynamic&) = default;

		//! Operatore di assegnazione.
		ObjectDynamic& operator=(const ObjectDynamic&) = default;

		//! Aggiorna l'oggetto dinamico.
		virtual void UpdateObject() = 0; 
	};
}

#endif
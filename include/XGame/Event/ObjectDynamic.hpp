#ifndef __OBJECT_DYNAMIC_XGAME__HPP
#define __OBJECT_DYNAMIC_XGAME__HPP

#include <XGame/Core/Config.hpp>

namespace xgame{
	class XGAME_API ObjectDynamic{
	public:
		//! Distruttore. Deassocia la registrazione presso l'EventManager.
		virtual ~ObjectDynamic() throw();

	protected:
		protected:
		/*! Questo metodo viene chiamato automaticamente dall'EventManager per aggiornare periodicamente l'oggetto*/
		virtual void UpdateObject() =0;
		
		//! Costruisce un oggetto dinamico registrandolo presso l'EventManager
		ObjectDynamic() throw();

	private:
		friend class EventManager;
	};
}

#endif
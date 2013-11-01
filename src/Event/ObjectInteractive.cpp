#include <XGame/Event/ObjectInteractive.hpp>
#include <XGame/Event/EventManager.hpp>

namespace xgame{
	ObjectInteractive::~ObjectInteractive(){
		EventManager::Get_GlobalInstance().UnRegisterObjectInteractive(this);
	}

	ObjectInteractive::ObjectInteractive(){
		EventManager::Get_GlobalInstance().RegisterObjectInteractive(this);
	}
}
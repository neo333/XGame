#include <XGame/Event/ObjectInteractive.hpp>
#include <XGame/Event/EventManager.hpp>

namespace xgame{
	ObjectInteractive::~ObjectInteractive(){
		EventManager::Get_GlobalInstance().UnRegisterObject(this);
	}

	ObjectInteractive::ObjectInteractive(){
		EventManager::Get_GlobalInstance().RegisterObject(this);
	}
}
#include <XGame/Event/ObjectDynamic.hpp>
#include <XGame/Event/EventManager.hpp>

namespace xgame{
	ObjectDynamic::ObjectDynamic() throw(){
		EventManager::Get_GlobalInstance().RegisterObjectDynamic(this);
	}

	ObjectDynamic::~ObjectDynamic() throw(){
		EventManager::Get_GlobalInstance().UnRegisterObjectDynamic(this);
	}
}
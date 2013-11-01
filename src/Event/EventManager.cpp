#include <XGame/Event/EventManager.hpp>

namespace xgame{
	EventManager* EventManager::s_ptrInstance = nullptr;

	EventManager::EventManager(){ }

	void EventManager::UpdateAll(){
		while(SDL_PollEvent(&this->m_event_sdl)){
			for(auto& object_registered : this->m_registro_interactive){
				object_registered.first->NotificationEvent(m_event_sdl);
			}
		}

		for(auto& object_register : this->m_registro_dynamic){
			object_register.first->UpdateObject();
		}
	}

	void EventManager::RegisterObjectInteractive(ObjectInteractive* const object) throw(){
		if(object==nullptr) return;
		RegistroObjectsInteractive::iterator find = this->m_registro_interactive.find(object);
		if(find == this->m_registro_interactive.end()) 
			this->m_registro_interactive.insert(std::pair<ObjectInteractive*,InfoObject>(object,InfoObject()));
	}

	void EventManager::UnRegisterObjectInteractive(ObjectInteractive* const object) throw(){
		if(object==nullptr) return;
		RegistroObjectsInteractive::iterator find = this->m_registro_interactive.find(object);
		if(find != this->m_registro_interactive.end())
			this->m_registro_interactive.erase(find);
	}

	void EventManager::RegisterObjectDynamic(ObjectDynamic* const object) throw(){
		if(object==nullptr) return;
		RegistroObjectDynamic::iterator find = this->m_registro_dynamic.find(object);
		if(find == this->m_registro_dynamic.end())
			this->m_registro_dynamic.insert(std::pair<ObjectDynamic*,InfoObject>(object,InfoObject()));
	}

	void EventManager::UnRegisterObjectDynamic(ObjectDynamic* const object) throw(){
		if(object==nullptr) return;
		RegistroObjectDynamic::iterator find = this->m_registro_dynamic.find(object);
		if(find != this->m_registro_dynamic.end())
			this->m_registro_dynamic.erase(find);
	}
}
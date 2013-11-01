#include <XGame/Event/EventManager.hpp>

namespace xgame{
	EventManager* EventManager::s_ptrInstance = nullptr;

	EventManager::EventManager(){ }

	void EventManager::UpdateAll(){
		while(SDL_PollEvent(&this->m_event_sdl)){
			for(auto& object_registered : this->m_registro){
				object_registered.first->NotificationEvent(m_event_sdl);
			}
		}
	}

	void EventManager::RegisterObject(ObjectInteractive* const object){
		RegistroObjects::iterator find = this->m_registro.find(object);
		if(find == this->m_registro.end()) 
			this->m_registro.insert(std::pair<ObjectInteractive*,InfoObject>(object,InfoObject()));
	}

	void EventManager::UnRegisterObject(ObjectInteractive* const object){
		RegistroObjects::iterator find = this->m_registro.find(object);
		if(find != this->m_registro.end())
			this->m_registro.erase(find);
	}
}
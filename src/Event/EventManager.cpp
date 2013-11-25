#include <XGame/Event/EventManager.hpp>

namespace xgame{
	EventManager* EventManager::s_ptrInstance = nullptr;

	const size_t EventManager::PopEvents_andADDtoList(ListEvents& out_events, const size_t n_max_pop){
		size_t pop_done = 0;
		bool no_event = false;
		while (pop_done < n_max_pop && no_event==false){
			if (SDL_PollEvent(&m_event_sdl) != 0){
				out_events.push_front(std::move(m_event_sdl));
				pop_done++;
			}
			else{
				no_event = true;
			}
		}
		return pop_done;
	}


}
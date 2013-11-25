#include <XGame/Video/ScreenVideo.hpp>

namespace xgame{
	ScreenVideo::ScreenVideo(const size_t w_size_renderer, const size_t h_size_renderer) throw():
		m_wsizeRenderer(w_size_renderer), m_hsizeRenderer(h_size_renderer)
	{

	}

	ScreenVideo::~ScreenVideo() throw(){
		this->Close();
	}

	void ScreenVideo::NotificationEvents(ListEvents& events){
		for (auto event = events.begin(); event != events.end(); ){
			if (m_window != nullptr) if (event->type == SDL_WINDOWEVENT) if (event->window.windowID == m_id_window){
				switch (event->window.event){
				case SDL_WINDOWEVENT_CLOSE:
					this->Close();
					event = events.erase(event);
					continue;
				}
			}
			event++;
		}
	}

	void ScreenVideo::Close() throw(){
		if(m_renderer!=nullptr){
			SDL_DestroyRenderer(m_renderer);
			m_renderer=nullptr;
		}
		if(m_window!=nullptr){
			SDL_DestroyWindow(m_window);
			m_window=nullptr;
			m_id_window=-1;
		}
		
	}

	void ScreenVideo::Open() throw(Error){
		this->Close();
		if(m_wsizeRenderer==0 || m_hsizeRenderer==0)
			throw Error("ScreenVideo","Open","Impossibile inizializzare un'area grafica a dimensioni nulle!");
		if (m_wsizeWindow == 0)
			m_wsizeWindow = m_wsizeRenderer;
		if (m_hsizeWindow == 0)
			m_hsizeWindow = m_hsizeRenderer;
		
		Uint32 flags_win = SDL_WINDOW_RESIZABLE;
		if(m_fullscreen){
			flags_win|=SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		m_window = SDL_CreateWindow(m_title_win.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,m_wsizeWindow,m_hsizeWindow,flags_win);
		if(m_window==nullptr)
			throw Error("ScreenVideo","Open","Impossibile inizializzare una finestra grafica!\n%s",SDL_GetError());
		m_renderer = SDL_CreateRenderer(m_window,-1,SDL_RENDERER_ACCELERATED);
		if(m_renderer==nullptr){
			SDL_DestroyWindow(m_window);
			m_window=nullptr;
			throw Error("ScreenVideo","Open","Impossibile inizializzare il renderer!\n%s",SDL_GetError());
		}

		m_id_window = SDL_GetWindowID(m_window);

		if(SDL_RenderSetLogicalSize(m_renderer,m_wsizeRenderer,m_hsizeRenderer)!=0)
			throw Error("ScreenVideo","Open","Impossibile dimensionare correttamente il renderer!\n%s",SDL_GetError());
	}

	void ScreenVideo::DrawTexture(const Texture& src_texture, const Point& xy_onRenderer, const Rect& area_renderer_active) throw(Error){
		if (src_texture.IsVoid()) return;
		SDL_Rect dest_rect;
		dest_rect.x = xy_onRenderer.Get_X_Component();
		dest_rect.y = xy_onRenderer.Get_Y_Component();
		dest_rect.w = src_texture.m_w_size_scaled;
		dest_rect.h = src_texture.m_h_size_scaled;

		if ((area_renderer_active.Get_Xcomponent() == 0 && area_renderer_active.Get_Ycomponent() == 0
			&& area_renderer_active.Get_Wcomponent() == -1 && area_renderer_active.Get_Hcomponent() == -1) == false){
			m_area_active_renderer = static_cast<const SDL_Rect>(area_renderer_active);
			if (m_area_active_renderer.w<0) m_area_active_renderer.w = m_wsizeRenderer - m_area_active_renderer.x;
			if (m_area_active_renderer.h<0) m_area_active_renderer.h = m_hsizeRenderer - m_area_active_renderer.y;
			SDL_RenderSetClipRect(m_renderer, &m_area_active_renderer);
		}

		if (SDL_RenderCopy(m_renderer, src_texture.m_texture, static_cast<const SDL_Rect*>(src_texture.m_drawnable_area), &dest_rect) != 0)
			throw Error("ScreenVideo", "DrawTexture", "Impossibile effettuare il rendering della texture richiesta!\n%s", SDL_GetError());
		SDL_RenderSetClipRect(m_renderer, nullptr);
	}

	void ScreenVideo::UpdateSize_Renderer(const size_t wSize_renderer, const size_t hSize_renderer) throw(Error){
		m_wsizeRenderer=wSize_renderer;
		m_hsizeRenderer=hSize_renderer;
		if(m_renderer==nullptr) return;
		if(SDL_RenderSetLogicalSize(m_renderer,wSize_renderer,hSize_renderer)!=0)
			throw Error("ScreenVideo","UpdateSize_Renderer","Impossibile dimensionare correttamente il renderer!\n%s",SDL_GetError());
	}

	void ScreenVideo::UpdateSize_GraphicWindow(const size_t wSize_window, const size_t hSize_window) throw(){
		m_wsizeWindow = wSize_window;
		m_hsizeWindow = hSize_window;
		if(m_window==nullptr) return;
		SDL_SetWindowSize(m_window,wSize_window,hSize_window);
	}

	void ScreenVideo::Set_FullScreenWindow(const bool fullscreen_active) throw(){
		m_fullscreen=fullscreen_active;
	}

	void ScreenVideo::SetHint_VSync(const bool vsync_active) throw(Error){
		if(vsync_active==true)
			if(SDL_SetHint(SDL_HINT_RENDER_VSYNC,"1")==SDL_FALSE)
				throw Error("ScreenVideo","SetHint_VSync","Impossibile abilitare il v-sync\n%s",SDL_GetError());
		if(vsync_active==false)
			if(SDL_SetHint(SDL_HINT_RENDER_VSYNC,"0")==SDL_FALSE)
				throw Error("ScreenVideo","SetHint_VSync","Impossibile disabilitare il v-sync\n%s",SDL_GetError());
	}

	void ScreenVideo::SetScaleQuality(const SCALING_QUALITY setting_quality) throw(Error){
		std::string _paramSDL;
		switch(setting_quality){
		case SCALING_QUALITY::LOW_QUALITY:
			_paramSDL = "0";
			break;
		case SCALING_QUALITY::MEDIUM_QUALITY:
			_paramSDL = "1";
			break;
		case SCALING_QUALITY::HIGH_QUALITY:
			_paramSDL = "2";
			break;
		}

		if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,_paramSDL.c_str())==SDL_FALSE)
			throw Error("ScreenVideo","SetScaleQuality","Impossibile settare la modalità di scaling\n%s",SDL_GetError());
	}

	const std::string ScreenVideo::GetName_VideoDriver() throw(){
		return std::string(SDL_GetCurrentVideoDriver());
	}

	std::vector<ScreenVideo::ptrSDL_DisplayMode> ScreenVideo::GetAll_AvailableDisplayMode(const int index_display) throw(Error){
		std::vector<ScreenVideo::ptrSDL_DisplayMode> rts;
		int n_mod = SDL_GetNumDisplayModes(index_display);
		if(n_mod<=0) 
			throw Error("ScreenVideo","GetAll_Available","Impossibile determinare le modalità grafiche supportate!");
		rts.resize(n_mod);
		for(int i=0; i<n_mod; i++){
			rts[i].reset(new SDL_DisplayMode);
			SDL_GetDisplayMode(index_display,i,rts[i].get());
		}
		return rts;
	}

	void ScreenVideo::UpdateName_GraphicWindow(const std::string& title_set) throw(){
		m_title_win = title_set;
		if(m_window==nullptr) return;
		SDL_SetWindowTitle(m_window,title_set.c_str());
	}

	void ScreenVideo::Set_FramesPerSecond_MAX(const size_t fps_max) throw(){
		if(fps_max!=0)
			m_ms_min_call_present = static_cast<Uint32>(1000.0f / (float)(fps_max));
		else
			m_ms_min_call_present = 0;
	}

	ScreenVideo::ptrSDL_DisplayMode ScreenVideo::GetCurrent_DisplayMode(const int index_display) throw(Error){
		ptrSDL_DisplayMode rts(new SDL_DisplayMode);
		if (SDL_GetCurrentDisplayMode(index_display, rts.get()) != 0)
			throw Error("ScreenVideo", "GetCurrent_DisplayMode", "Impossibile determinare la modalità grafica corrente!\n%s", SDL_GetError());
		return rts;
	}

	ScreenVideo::ptrSDL_DisplayMode ScreenVideo::GetClosest_DisplayModeTo(const ScreenVideo::ptrSDL_DisplayMode& input_near, const int index_display)
		throw(Error)
	{
		ptrSDL_DisplayMode rts(new SDL_DisplayMode);
		if (SDL_GetClosestDisplayMode(index_display, input_near.get(), rts.get()) == nullptr)
			throw Error("ScreenVideo", "GetClosest_DisplayModeTo", "Impossibile determinare la modalità grafica prossima!\n%s", SDL_GetError());
		return rts;
	}
}
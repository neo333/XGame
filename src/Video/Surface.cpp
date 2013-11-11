#include <XGame/Video/Surface.hpp>
#include <XGame/Core/Error.hpp>
#include <SDL/SDL_image.h>

namespace xgame{
	void Surface::Clean() throw(){
		if (m_surface){
			SDL_FreeSurface(m_surface);
			m_surface = nullptr;
		}
	}

	Surface::Surface(const size_t w, const size_t h) throw(...) :
		m_surface(SDL_CreateRGBSurface(0, w, h, 32, Surface::rmask, Surface::gmask, Surface::bmask, Surface::amask))
	{
		if (m_surface == nullptr)
			throw Error("Surface", "Surface", "Impossibile creare una surface!\n%s", SDL_GetError());
		SDL_SetSurfaceRLE(m_surface, 1);
		SDL_SetSurfaceBlendMode(m_surface, SDL_BLENDMODE_BLEND);
	}

	Surface::Surface(const size_t w, const size_t h, const Color& pixels_color) throw(...) :
		m_surface(SDL_CreateRGBSurface(0,w,h,32,Surface::rmask,Surface::gmask,Surface::bmask,Surface::amask))
	{
		if (m_surface == nullptr)
			throw Error("Surface", "Surface", "Impossibile creare una surface!\n%s", SDL_GetError());
		SDL_SetSurfaceRLE(m_surface, 1);
		SDL_SetSurfaceBlendMode(m_surface, SDL_BLENDMODE_BLEND);

		if(SDL_LockSurface(m_surface)!=0)
			throw Error("Surface", "Surface", "Impossibile inizializzare una surface!\n%s", SDL_GetError());
		for (register size_t row = 0; row < h; row++){
			for (register size_t index = 0; index < w; index++){
				static_cast<Uint32*>(this->m_surface->pixels)[index + row*m_surface->pitch / 4] = static_cast<Uint32>(pixels_color);
			}
		}
		SDL_UnlockSurface(m_surface);
	}

	Surface::Surface(const Surface& oth) throw(...){
		if (oth.m_surface)
			m_surface = SDL_ConvertSurfaceFormat(oth.m_surface, SDL_PIXELFORMAT_RGBA8888, 0);
	}

	Surface::Surface(Surface&& oth) throw():m_surface(oth.m_surface){
		oth.m_surface = nullptr;
	}

	Surface& Surface::operator=(const Surface& oth) throw(...){
		if (this != &oth){
			this->Clean();
			if (oth.m_surface)
				m_surface = SDL_ConvertSurfaceFormat(oth.m_surface, SDL_PIXELFORMAT_RGBA8888, 0);
		}
		return *this;
	}

	Surface& Surface::operator=(Surface&& oth) throw(){
		if (this != &oth){
			this->Clean();
			m_surface = oth.m_surface;
			oth.m_surface = nullptr;
		}
		return *this;
	}

	void Surface::LoadSurface_fromMemoryPage(const MemoryPage& page_input) throw(...){
		this->Clean();
		if (page_input.GetSize() == 0) return;
		SDL_RWops* data_access = SDL_RWFromConstMem(page_input.Get_PtrMemory(), page_input.GetSize());
		if (data_access == nullptr){
			throw Error("Surface", "LoadSurface_fromMemoryPage", "Pagina di memoria corrotta!\n%s", SDL_GetError());
		}
		else{
			SDL_Surface* image_load = IMG_Load_RW(data_access, 1);
			if (image_load == nullptr){
				throw Error("Surface", "LoadSurface_fromMemoryPage", "Impossibile caricare correttamente la surface richiesta\n%s", SDL_GetError());
			}
			else{
				m_surface = SDL_ConvertSurfaceFormat(image_load, SDL_PIXELFORMAT_RGBA8888, 0);
				SDL_FreeSurface(image_load);
				image_load = nullptr;
				if (m_surface == nullptr){
					throw Error("Surface", "LoadSurface_fromMemoryPage", "Impossibile ottimizzare correttamente la surface richiesta\n%s", SDL_GetError());
				}
				SDL_SetSurfaceRLE(m_surface, 1);
				SDL_SetSurfaceBlendMode(m_surface, SDL_BLENDMODE_BLEND);
			}
		}
	}

	void Surface::Set_ModAlpha_forThisColor(const Color& color_select, const Uint8 alpha_set) throw(...){
		if (m_surface == nullptr) return;
		if (SDL_LockSurface(m_surface) != 0)
			throw Error("Surface", "Set_ModAlpha_forThisColor", "Impossibile accedere ai dati della surface!\n%s", SDL_GetError());
		for (register int row = 0; row < m_surface->h; row++){
			for (register int index = 0; index < m_surface->w; index++){
				Uint32& pixel = static_cast<Uint32*>(this->m_surface->pixels)[index + row*m_surface->pitch / 4];
				if (((pixel&Surface::rmask)>>24 == color_select.Get_RedComponent()) &&
					((pixel&Surface::gmask)>>16 == color_select.Get_GreenComponent()) &&
					((pixel&Surface::bmask)>>8 == color_select.Get_BlueComponent()))
				{
					pixel = static_cast<Uint32>(Color(color_select.Get_RedComponent(), color_select.Get_GreenComponent(),
						color_select.Get_BlueComponent(), alpha_set));
				}
			}
		}
		SDL_UnlockSurface(m_surface);
	}
}
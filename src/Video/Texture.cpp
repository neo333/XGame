#include <XGame/Video/Texture.hpp>
#include <XGame/Video/ScreenVideo.hpp>
#include <SDL2/SDL_image.h>
#include <XGame/Video/TextSurface.hpp>
#include <chrono>

namespace xgame{

	Texture::Texture()throw():m_render(nullptr),m_texture(nullptr),m_w_size(0),m_h_size(0),m_w_size_scaled(0),m_h_size_scaled(0){

	}

	Texture::Texture(const size_t w_size, const size_t h_size, const ScreenVideo& makerVideo) throw(Error):
			m_render(makerVideo.m_renderer),m_w_size(w_size),m_h_size(h_size),m_w_size_scaled(w_size),m_h_size_scaled(h_size),
			m_drawnable_area(Rect(0,0,w_size,h_size)),
			m_texture(SDL_CreateTexture(makerVideo.m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w_size, h_size))
	{
		if (m_render == nullptr)
			throw Error("Texture", "Texture", "Impossibile inizializzare una texture con uno specifico renderer nullo!");
		if (m_texture == nullptr)
			throw Error("Texture", "Texture", "Impossibile creare una texture grafica!\n%s", SDL_GetError());

		SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
	}

	void Texture::Clean() throw(){
		if(this->m_texture!=nullptr){
			SDL_DestroyTexture(this->m_texture);
			this->m_texture=nullptr;
			this->m_w_size=0;
			this->m_h_size=0;
			this->m_w_size_scaled=0;
			this->m_h_size_scaled=0;
			this->m_drawnable_area.Set_AllComponent(0,0,0,0);
		}
	}

	Texture::~Texture() throw(){
		this->Clean();
	}


	void Texture::LoadTexture_fromMemoryPage(const MemoryPage& input_page, const ScreenVideo& makerVideo) throw(Error){
		this->Clean();
		if(makerVideo.m_renderer==nullptr)
			throw Error("Texture","LoadTexture_fromMemoryPage","Impossibile caricare una texture con uno specifico renderer nullo!");
		if(makerVideo.m_renderer!=m_render) m_render = makerVideo.m_renderer;
		if(input_page.GetSize()==0) return;
		if(m_render==nullptr) 
			throw Error("Texture","LoadTexture_fromMemoryPage","Impossibile caricare la texture richiesta!\nRenderer non inizializzato!");

		SDL_RWops* data_access = SDL_RWFromConstMem(input_page.Get_PtrMemory(),input_page.GetSize());
		if(data_access==nullptr){
			throw Error("Texture","LoadTexture_fromMemoryPage","Pagina di memoria corrotta!\n%s",SDL_GetError());
		}else{
			SDL_Surface* image_load = IMG_Load_RW(data_access,1);
			if(image_load==nullptr){
				throw Error("Texture","LoadTexture_fromMemoryPage","Impossibile caricare correttamente la texture richiesta\n%s",SDL_GetError());
			}else{
				SDL_Surface* image_opt = SDL_ConvertSurfaceFormat(image_load,SDL_PIXELFORMAT_RGBA8888,0);
				SDL_FreeSurface(image_load);
				image_load=nullptr;
				if(image_opt==nullptr){
					throw Error("Texture","LoadTexture_fromMemoryPage","Impossibile ottimizzare correttamente la texture richiesta\n%s",SDL_GetError());
				}

				this->m_texture = SDL_CreateTexture(m_render,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_STREAMING,image_opt->w,image_opt->h);
				if(this->m_texture==nullptr){
					SDL_FreeSurface(image_opt);
					throw Error("Texture","LoadTexture_fromMemoryPage","Impossibile creare una texture grafica!\n%s",SDL_GetError());
				}

				void* data_pixel_texture;
				int pitch_pixel_texture;

				if(SDL_LockTexture(m_texture,NULL,&data_pixel_texture,&pitch_pixel_texture)!=0){
					SDL_FreeSurface(image_opt);
					SDL_DestroyTexture(m_texture);
					throw Error("Texture","LoadTexture_fromMemoryPage","Impossibile scrivere la texture grafica!\n%s",SDL_GetError());
				}
				memcpy(data_pixel_texture,image_opt->pixels,image_opt->pitch * image_opt->h);
				SDL_UnlockTexture(m_texture);
				data_pixel_texture=nullptr;

				m_w_size = image_opt->w;
				m_h_size = image_opt->h;
				m_w_size_scaled = image_opt->w;
				m_h_size_scaled = image_opt->h;
				m_drawnable_area.Set_AllComponent(0,0,image_opt->w,image_opt->h);
				
				SDL_FreeSurface(image_opt);
				image_opt=nullptr;

				SDL_SetTextureBlendMode(m_texture,SDL_BLENDMODE_BLEND);
			}
		}


	}

	void Texture::LoadTexture_fromSurface(const Surface& input_surface, const ScreenVideo& makerVideo, Rect& area_cut) throw(Error){
		this->Clean();
		if (makerVideo.m_renderer == nullptr)
			throw Error("Texture", "LoadTexture_fromMemoryPage", "Impossibile caricare una texture con uno specifico renderer nullo!");
		if (makerVideo.m_renderer != m_render) m_render = makerVideo.m_renderer;
		if (input_surface.Is_Void()) return;
		if (area_cut.Get_Xcomponent() < 0) area_cut.Set_Xcomponent(0);
		if (area_cut.Get_Ycomponent() < 0) area_cut.Set_Ycomponent(0);
		if (area_cut.Get_Wcomponent() < 0) area_cut.Set_Wcomponent(input_surface.Get_W() - area_cut.Get_Xcomponent());
		if (area_cut.Get_Hcomponent() < 0) area_cut.Set_Hcomponent(input_surface.Get_H() - area_cut.Get_Ycomponent());
		if (area_cut.Get_Wcomponent() + (size_t)area_cut.Get_Xcomponent() > input_surface.Get_W())
			area_cut.Set_Wcomponent(input_surface.Get_W() - area_cut.Get_Xcomponent());
		if (area_cut.Get_Hcomponent() + (size_t)area_cut.Get_Ycomponent() > input_surface.Get_H())
			area_cut.Set_Hcomponent(input_surface.Get_H() - area_cut.Get_Ycomponent());

		this->m_texture = SDL_CreateTexture(m_render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 
			area_cut.Get_Wcomponent(), area_cut.Get_Hcomponent());
		if (this->m_texture == nullptr)
			throw Error("Texture", "LoadTexture_fromSurface", "Impossibile creare una texture grafica!\n%s", SDL_GetError());

		void* data_texture;
		int pitch_texture;

		if (SDL_LockTexture(m_texture, NULL, &data_texture, &pitch_texture) != 0)
			throw Error("Texture", "LoadTexture_fromSurface", "Impossibile scrivere la texture grafica!\n%s", SDL_GetError());
		if (SDL_LockSurface(input_surface.m_surface) != 0){
			SDL_UnlockTexture(m_texture);
			throw Error("Texture", "LoadTexture_fromSurface", "Impossibile leggere la surface grafica!\n%s", SDL_GetError());
		}
			

		try{
			Uint8* texture_pixelb = static_cast<Uint8*>(data_texture);
			const Uint8* surface_pixelb = static_cast<Uint8*>(input_surface.m_surface->pixels) +
				area_cut.Get_Ycomponent() * input_surface.m_surface->pitch +
				area_cut.Get_Xcomponent() * input_surface.m_surface->format->BytesPerPixel;

			const int row_size = area_cut.Get_Wcomponent() * input_surface.m_surface->format->BytesPerPixel;

			for (register int row = 0; row < area_cut.Get_Hcomponent(); ++row){
				memcpy(texture_pixelb, surface_pixelb, row_size);
				texture_pixelb += pitch_texture;
				surface_pixelb += input_surface.m_surface->pitch;
			}
		}
		catch (const std::exception& err){
			SDL_UnlockTexture(m_texture);
			SDL_UnlockSurface(input_surface.m_surface);
			throw Error("Texture", "LoadTexture_fromSurface", "Impossibile copiare i dati nella destinazione!\n%s", err.what());
		}

		SDL_UnlockSurface(input_surface.m_surface);
		SDL_UnlockTexture(m_texture);
		data_texture = nullptr;

		m_w_size = area_cut.Get_Wcomponent();
		m_h_size = area_cut.Get_Hcomponent();
		m_w_size_scaled = m_w_size;
		m_h_size_scaled = m_h_size;
		m_drawnable_area.Set_AllComponent(0, 0, m_w_size, m_h_size);

		SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
	}

	Texture::Texture(const Texture& oth)throw(Error) :m_render(oth.m_render), m_w_size(oth.m_w_size), m_h_size(oth.m_h_size),
		m_texture(Texture::CopyInternalTexture(oth)),m_drawnable_area(oth.m_drawnable_area),m_w_size_scaled(oth.m_w_size_scaled),
		m_h_size_scaled(oth.m_h_size_scaled){

	}

	Texture& Texture::operator=(const Texture& oth) throw(Error){
		if(&oth!=this){
			this->Clean();
			this->m_render = oth.m_render;
			this->m_w_size = oth.m_w_size;
			this->m_h_size = oth.m_h_size;
			this->m_texture = Texture::CopyInternalTexture(oth);
			this->m_drawnable_area = oth.m_drawnable_area;
			this->m_w_size_scaled = oth.m_w_size_scaled;
			this->m_h_size_scaled = oth.m_h_size_scaled;
		}
		return *this;
	}

	Texture::Texture(Texture&& oth) throw():m_render(oth.m_render),m_w_size(oth.m_w_size),m_h_size(oth.m_h_size),m_texture(oth.m_texture),
		m_drawnable_area(oth.m_drawnable_area),m_w_size_scaled(oth.m_w_size_scaled),m_h_size_scaled(oth.m_h_size_scaled){
		oth.m_texture = nullptr;
		oth.m_w_size = 0;
		oth.m_h_size = 0;
		oth.m_drawnable_area.Set_AllComponent(0,0,0,0);
		oth.m_w_size_scaled=0;
		oth.m_h_size_scaled=0;
	}

	Texture& Texture::operator=(Texture&& oth) throw(){
		if(&oth!=this){
			this->Clean();
			this->m_render = oth.m_render;
			this->m_texture = oth.m_texture;
			this->m_w_size = oth.m_w_size;
			this->m_h_size = oth.m_h_size;
			this->m_drawnable_area = oth.m_drawnable_area;
			this->m_w_size_scaled = oth.m_w_size_scaled;
			this->m_h_size_scaled = oth.m_h_size_scaled;

			oth.m_texture = nullptr;
			oth.m_w_size =0;
			oth.m_h_size =0;
			oth.m_drawnable_area.Set_AllComponent(0,0,0,0);
			oth.m_w_size_scaled=0;
			oth.m_h_size_scaled=0;
		}
		return *this;
	}

	SDL_Texture* Texture::CopyInternalTexture(const Texture& src) throw(Error){
		if(src.m_texture==nullptr || src.m_render==nullptr) return nullptr;

		SDL_Texture* rts = SDL_CreateTexture(src.m_render,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_STREAMING,src.m_w_size,src.m_h_size);
		if(rts==nullptr)
			throw Error("Texture","CopyInternalTexture","Impossibile inizializzare una texture grafica!\n%s",SDL_GetError());

		void* src_pixel_data=nullptr;
		void* dest_pixel_data=nullptr;
		int src_pitch=0;
		int dest_pitch=0;

		if(SDL_LockTexture(src.m_texture,NULL,&src_pixel_data,&src_pitch)!=0){
			SDL_DestroyTexture(rts);
			throw Error("Texture","CopyInternalTexture","Impossibile accedere ai dati della sorgente!\n%s",SDL_GetError());
		}
		if(SDL_LockTexture(rts,NULL,&dest_pixel_data,&dest_pitch)!=0){
			SDL_UnlockTexture(src.m_texture);
			SDL_DestroyTexture(rts);
			throw Error("Texture","CopyInternalTexture","Impossibile accedere ai dati della destinazione!\n%s",SDL_GetError());
		}

		try{
			memcpy(dest_pixel_data,src_pixel_data,src_pitch * src.m_h_size);
		}catch(std::exception& err){
			SDL_UnlockTexture(src.m_texture);
			SDL_UnlockTexture(rts);
			SDL_DestroyTexture(rts);
			throw Error("Texture","CopyInternalTexture","Impossibile copiare i dati nella destinazione!\n%s",err.what());
		}
		SDL_UnlockTexture(src.m_texture);
		SDL_UnlockTexture(rts);
		src_pixel_data=nullptr;
		dest_pixel_data=nullptr;

		SDL_SetTextureBlendMode(rts,SDL_BLENDMODE_BLEND);
		return rts;
	}

	void Texture::LoadTexture_fromFont(const std::string& input_str, const Font& input_font, const Color& str_color, const ScreenVideo& makerVideo) throw(Error){
		this->Clean();
		if (makerVideo.m_renderer == nullptr)
			throw Error("Texture", "LoadTexture_fromMemoryPage", "Impossibile caricare una texture con uno specifico renderer nullo!");
		if (input_str.size() == 0) return;
		TextSurface temp_txt_surface;
		temp_txt_surface.LoadSurface_fromFont(input_font, input_str, str_color);
		this->LoadTexture_fromSurface(temp_txt_surface, makerVideo);
	}
}
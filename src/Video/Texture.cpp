#include <XGame/Video/Texture.hpp>
#include <XGame/Video/ScreenVideo.hpp>
#include <SDL/SDL_image.h>
#include <chrono>

namespace xgame{

	Texture::Texture()throw():m_render(nullptr),m_texture(nullptr),m_w_size(0),m_h_size(0),m_w_size_scaled(0),m_h_size_scaled(0){

	}

	Texture::Texture(const size_t w_size, const size_t h_size, const ScreenVideo& makerVideo) throw(...):
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

	Texture::~Texture(){
		this->Clean();
	}


	void Texture::LoadTexture_fromMemoryPage(const MemoryPage& input_page, const ScreenVideo& makerVideo) throw(...){
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

	void Texture::LoadTexture_fromSurface(const Surface& input_surface, const ScreenVideo& makerVideo, Rect& area_cut) throw(...){
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

	Texture::Texture(const Texture& oth):m_render(oth.m_render),m_w_size(oth.m_w_size),m_h_size(oth.m_h_size),
		m_texture(Texture::CopyInternalTexture(oth)),m_drawnable_area(oth.m_drawnable_area),m_w_size_scaled(oth.m_w_size_scaled),
		m_h_size_scaled(oth.m_h_size_scaled){

	}

	Texture& Texture::operator=(const Texture& oth){
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

	Texture::Texture(Texture&& oth):m_render(oth.m_render),m_w_size(oth.m_w_size),m_h_size(oth.m_h_size),m_texture(oth.m_texture),
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

	SDL_Texture* Texture::CopyInternalTexture(const Texture& src) throw(...){
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

	void Texture::UpdateTexture_withBlend(const Texture& input_texture, Rect& input_cut, Point& out_xy) throw(...){
		if (input_texture.IsVoid()) return;
		if (input_cut.Get_Xcomponent() < 0) input_cut.Set_Xcomponent(0);
		if (input_cut.Get_Ycomponent() < 0) input_cut.Set_Ycomponent(0);
		if (input_cut.Get_Xcomponent() > input_texture.m_w_size) input_cut.Set_Xcomponent(input_texture.m_w_size);
		if (input_cut.Get_Ycomponent() > input_texture.m_h_size) input_cut.Set_Ycomponent(input_texture.m_h_size);
		if (input_cut.Get_Wcomponent() < 0) input_cut.Set_Wcomponent(input_texture.m_w_size - input_cut.Get_Xcomponent());
		if (input_cut.Get_Hcomponent() < 0) input_cut.Set_Hcomponent(input_texture.m_h_size - input_cut.Get_Ycomponent());
		if (input_cut.Get_Wcomponent() + input_cut.Get_Xcomponent() > input_texture.m_w_size)
			input_cut.Set_Wcomponent(input_texture.m_w_size - input_cut.Get_Xcomponent());
		if (input_cut.Get_Hcomponent() + input_cut.Get_Ycomponent() > input_texture.m_h_size)
			input_cut.Set_Hcomponent(input_texture.m_h_size - input_cut.Get_Ycomponent());
		if (input_cut.Get_Wcomponent() > m_w_size - out_xy.Get_X_Component())
			input_cut.Set_Wcomponent(m_w_size - out_xy.Get_X_Component());
		if (input_cut.Get_Hcomponent() > m_h_size - out_xy.Get_Y_Component())
			input_cut.Set_Hcomponent(m_h_size - out_xy.Get_Y_Component());

		void* src_pixel_data = nullptr;
		void* dest_pixel_data = nullptr;
		int src_pitch = 0;
		int dest_pitch = 0;

		if (SDL_LockTexture(input_texture.m_texture, input_cut, &src_pixel_data, &src_pitch) != 0){
			throw Error("Texture", "UpdateTexture_withBlend", "Impossibile accedere ai dati della sorgente!\n%s", SDL_GetError());
		}
		if (SDL_LockTexture(m_texture, Rect(out_xy.Get_X_Component(), out_xy.Get_Y_Component(), input_cut.Get_Wcomponent(), input_cut.Get_Hcomponent()), 
			&dest_pixel_data, &dest_pitch) != 0)
		{
			SDL_UnlockTexture(input_texture.m_texture);
			throw Error("Texture", "UpdateTexture_withBlend", "Impossibile accedere ai dati della destinazione!\n%s", SDL_GetError());
		}

		try{
			const Uint32* p_src = static_cast<Uint32*>(src_pixel_data);
			Uint32* p_dest = static_cast<Uint32*>(dest_pixel_data);
			const int len = input_cut.Get_Wcomponent();
			const Uint8 alpha_mod_input = input_texture.Get_AlphaMod();
			for (register int row = 0; row < input_cut.Get_Hcomponent(); ++row){
				for (register int i = 0; i < len; i++){
					Color& pixel_dest= Color(p_dest[i]);
					Color& pixel_src = Color(p_src[i]);
					if (pixel_src.Get_AlphaComponent() != 0 && alpha_mod_input!=255){
						pixel_src.Set_AlphaComponent(pixel_src.Get_AlphaComponent() - (255 - alpha_mod_input));
					}

					pixel_dest.Set_RedComponent(static_cast<Uint8>((pixel_src.Get_RedComponent() * pixel_src.Get_AlphaComponentNormalized()) +
						(pixel_dest.Get_RedComponent()*(1.f - pixel_src.Get_AlphaComponentNormalized()))));
					pixel_dest.Set_GreenComponent((static_cast<Uint8>((pixel_src.Get_GreenComponent() * pixel_src.Get_AlphaComponentNormalized()) +
						(pixel_dest.Get_GreenComponent()*(1.f - pixel_src.Get_AlphaComponentNormalized())))));
					pixel_dest.Set_BlueComponent((static_cast<Uint8>((pixel_src.Get_BlueComponent() * pixel_src.Get_AlphaComponentNormalized()) +
						(pixel_dest.Get_BlueComponent()*(1.f - pixel_src.Get_AlphaComponentNormalized())))));

					pixel_dest.Set_AlphaComponentNormalized(pixel_src.Get_AlphaComponentNormalized() +
						(pixel_dest.Get_AlphaComponentNormalized()*(1.f - pixel_src.Get_AlphaComponentNormalized())));
					
					p_dest[i] = static_cast<Uint32>(pixel_dest);
				}
				p_src += src_pitch / 4;
				p_dest += dest_pitch / 4;
			}

		}
		catch (std::exception& err){
			SDL_UnlockTexture(input_texture.m_texture);
			SDL_UnlockTexture(m_texture);
			throw Error("Texture", "UpdateTexture_withBlend", "Impossibile copiare i dati nella destinazione!\n%s", err.what());
		}

		SDL_UnlockTexture(input_texture.m_texture);
		SDL_UnlockTexture(m_texture);
		src_pixel_data = nullptr;
		dest_pixel_data = nullptr;
	}

	void Texture::UpdateTexture_withoutAlphaMod(const Texture& input_texture, Rect& input_cut, Point& out_xy) throw(...){
		if (input_texture.IsVoid()) return;
		if (input_cut.Get_Xcomponent() < 0) input_cut.Set_Xcomponent(0);
		if (input_cut.Get_Ycomponent() < 0) input_cut.Set_Ycomponent(0);
		if (input_cut.Get_Xcomponent() > input_texture.m_w_size) input_cut.Set_Xcomponent(input_texture.m_w_size);
		if (input_cut.Get_Ycomponent() > input_texture.m_h_size) input_cut.Set_Ycomponent(input_texture.m_h_size);
		if (input_cut.Get_Wcomponent() < 0) input_cut.Set_Wcomponent(input_texture.m_w_size - input_cut.Get_Xcomponent());
		if (input_cut.Get_Hcomponent() < 0) input_cut.Set_Hcomponent(input_texture.m_h_size - input_cut.Get_Ycomponent());
		if (input_cut.Get_Wcomponent() + input_cut.Get_Xcomponent() > input_texture.m_w_size)
			input_cut.Set_Wcomponent(input_texture.m_w_size - input_cut.Get_Xcomponent());
		if (input_cut.Get_Hcomponent() + input_cut.Get_Ycomponent() > input_texture.m_h_size)
			input_cut.Set_Hcomponent(input_texture.m_h_size - input_cut.Get_Ycomponent());
		if (input_cut.Get_Wcomponent() > m_w_size - out_xy.Get_X_Component())
			input_cut.Set_Wcomponent(m_w_size - out_xy.Get_X_Component());
		if (input_cut.Get_Hcomponent() > m_h_size - out_xy.Get_Y_Component())
			input_cut.Set_Hcomponent(m_h_size - out_xy.Get_Y_Component());

		void* src_pixel_data = nullptr;
		void* dest_pixel_data = nullptr;
		int src_pitch = 0;
		int dest_pitch = 0;

		
		if (SDL_LockTexture(input_texture.m_texture, input_cut, &src_pixel_data, &src_pitch) != 0){
			throw Error("Texture", "UpdateTexture_withBlend", "Impossibile accedere ai dati della sorgente!\n%s", SDL_GetError());
		}
		if (SDL_LockTexture(m_texture, Rect(out_xy.Get_X_Component(),out_xy.Get_Y_Component(),input_cut.Get_Wcomponent(),input_cut.Get_Hcomponent()), 
					&dest_pixel_data, &dest_pitch) != 0)
		{
			SDL_UnlockTexture(input_texture.m_texture);
			throw Error("Texture", "UpdateTexture_withBlend", "Impossibile accedere ai dati della destinazione!\n%s", SDL_GetError());
		}

		try{
			const Uint32* p_src = static_cast<Uint32*>(src_pixel_data);
			Uint32* p_dest = static_cast<Uint32*>(dest_pixel_data);
			int len = input_cut.Get_Wcomponent();
			
			for (register int row = 0; row < input_cut.Get_Hcomponent(); ++row){
				for (register int i = 0; i < len; i++){
					if ((p_src[i] & 0x000000ff) != 0)
						p_dest[i] = p_src[i];
				}
				p_src += src_pitch / 4;
				p_dest += dest_pitch / 4;
			}

			
		}
		catch (std::exception& err){
			SDL_UnlockTexture(input_texture.m_texture);
			SDL_UnlockTexture(m_texture);
			throw Error("Texture", "UpdateTexture_withBlend", "Impossibile copiare i dati nella destinazione!\n%s", err.what());
		}

		SDL_UnlockTexture(input_texture.m_texture);
		SDL_UnlockTexture(m_texture);
	}
}
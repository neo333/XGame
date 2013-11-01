#include <XGame/Video/Texture.hpp>
#include <XGame/Video/ScreenVideo.hpp>
#include <SDL/SDL_image.h>

namespace xgame{

	Texture::Texture()throw():m_render(nullptr),m_texture(nullptr),m_w_size(0),m_h_size(0){

	}

	void Texture::Clean() throw(){
		if(this->m_texture!=nullptr){
			SDL_DestroyTexture(this->m_texture);
			this->m_texture=nullptr;
			this->m_w_size=0;
			this->m_h_size=0;
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
				m_drawnable_area.Set_AllComponent(0,0,image_opt->w,image_opt->h);
				
				SDL_FreeSurface(image_opt);
				image_opt=nullptr;

				SDL_SetTextureBlendMode(m_texture,SDL_BLENDMODE_BLEND);
			}
		}


	}

	Texture::Texture(const Texture& oth):m_render(oth.m_render),m_w_size(oth.m_w_size),m_h_size(oth.m_h_size),
		m_texture(Texture::CopyInternalTexture(oth)),m_drawnable_area(oth.m_drawnable_area){

	}

	Texture& Texture::operator=(const Texture& oth){
		if(&oth!=this){
			this->Clean();
			this->m_render = oth.m_render;
			this->m_w_size = oth.m_w_size;
			this->m_h_size = oth.m_h_size;
			this->m_texture = Texture::CopyInternalTexture(oth);
			this->m_drawnable_area = oth.m_drawnable_area;
		}
		return *this;
	}

	Texture::Texture(Texture&& oth):m_render(oth.m_render),m_w_size(oth.m_w_size),m_h_size(oth.m_h_size),m_texture(oth.m_texture),m_drawnable_area(oth.m_drawnable_area){
		oth.m_texture = nullptr;
		oth.m_w_size = 0;
		oth.m_h_size = 0;
		oth.m_drawnable_area.Set_AllComponent(0,0,0,0);
	}

	Texture& Texture::operator=(Texture&& oth) throw(){
		if(&oth!=this){
			this->Clean();
			this->m_render = oth.m_render;
			this->m_texture = oth.m_texture;
			this->m_w_size = oth.m_w_size;
			this->m_h_size = oth.m_h_size;
			this->m_drawnable_area = oth.m_drawnable_area;
			oth.m_texture = nullptr;
			oth.m_w_size =0;
			oth.m_h_size =0;
			oth.m_drawnable_area.Set_AllComponent(0,0,0,0);
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
}
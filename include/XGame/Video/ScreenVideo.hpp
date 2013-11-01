#ifndef __SCREEN_VIDEO_XGAME__HPP
#define __SCREEN_VIDEO_XGAME__HPP

#include <XGame/Core/Config.hpp>
#include <XGame/Core/Error.hpp>
#include <XGame/Event/ObjectInteractive.hpp>
#include <XGame/Video/Texture.hpp>
#include <XGame/Video/Color.hpp>
#include <XGame/Video/Point.hpp>
#include <XGame/Video/Rect.hpp>
#include <boost/noncopyable.hpp>
#include <SDL/SDL.h>

#ifdef WIN32 
#pragma warning(disable:4251)
#pragma warning(disable:4275)
#endif

namespace xgame{

	class XGAME_API ScreenVideo: public boost::noncopyable, public ObjectInteractive{
	public:
		//! Costruttore di default. Specificare i parametri dimensionali dell'area del RENDERER
		ScreenVideo(const size_t w_size_renderer =0, const size_t h_size_renderer =0);

		//! Distruttore di default.
		virtual ~ScreenVideo();

		/*! Crea una Texture vuota secondo le tipologie grafiche definite sul corrente ScreenVideo.
			@note		Lo screenVideo deve essere NECESSARIAMENTE 'open' prima di chiamare questo metodo!
			@return		Una Texture VUOTA.
		*/
		inline Texture MakeVoidTexture() const throw(...);

		//! Chiude lo ScreenVideo deallocando tutte le risorse dedicate ad esso.
		void Close() throw();

		//! @return		'true' se lo ScreenVideo è stato lanciato ed è attivo, 'false' altrimenti.
		inline const bool IsOpen() const throw();

		/*! Lancia lo ScreenVideo (finestra grafica applicativa che contiene il renderer grafico).
			@note			Prima di lanciare lo ScreenVideo, assicurarsi di settare tutte le impostazioni preferite.
			@throw	Error	In caso di errore critico!
		*/
		void Open() throw(...);

		//! @return		La larghezza in pixel del renderer.
		inline const size_t Get_WSizeRenderer() const throw();

		//! @return		L'altezza in pixel del renderer.
		inline const size_t Get_HSizeRenderer() const throw();

		/*! @return		L'ID (univoco) della finestra grafica.
			@note		Ritorna '-1' in caso la finestra grafica non sia stata ancora lanciata
		*/
		inline const Uint32 Get_ID_Window() const throw();

		/*! Modica e aggiorna la dimensione del renderer grafico.
			@throw	Error		In caso l'aggiornamento non vada a buonfine.
			@note	Se viene chiamato questo metodo quando lo ScreenVideo non è attivo le dimensione saranno effettive al prossimo avvio.
		*/
		void UpdateSize_Renderer(const size_t wSize_renderer, const size_t hSize_renderer) throw(...);

		//! Modifica e aggiorna la dimensione della finestra che contiene il renderer grafico.
		void UpdateSize_GraphicWindow(const size_t wSize_window, const size_t hSize_window) throw();

		//! Modifica ed aggiorna il titolo della finestra che contiene il render grafico.
		void UpdateName_GraphicWindow(const std::string& title_set) throw();

		/*! Setta la modalità 'fullscreen' o 'window' per la finestra grafica.
			@note	Se lo ScreenVideo è stato già stato aperto è necessario chiudere e riaprire lo ScreenVideo.
		*/
		void Set_FullScreenWindow(const bool fullscreen_active) throw();

		/*! Setta il numero di volte MASSIMO che il renderer può essere presentato sulla finestra grafica.
			@note	Inserire '0' come parametro per disattivare il controllo sui fps massimi (default)
		*/
		void Set_FramesPerSecond_MAX(const size_t fps_max) throw();

		/*! Abilita o meno il v-sync.
			@note	Probabilmente è necessario chiamare questo metodo all'avvio dell'applicazione.
			@throw	Error	In caso non il vsync non sia attivabile.
		*/
		void SetHint_VSync(const bool vsync_active) throw(...);

		/*! Setta la qualità dello scaling del renderer.
			@note	La qualità 'HIGH_QUALITY' è disponibile solo per driver grafici 'direct3D'; mentre la qualità 'MEDIUM_QUALITY'
					è supportata sia per 'opengl' che per 'direct3D'.
			@throw	Error	In caso non la qualità non sia supportata dal driver video.
		*/
		enum class SCALING_QUALITY{
			LOW_QUALITY,
			MEDIUM_QUALITY,
			HIGH_QUALITY
		};
		void SetScaleQuality(const SCALING_QUALITY setting_quality) throw(...);

		/*! Effettua la prensentazione del renderer all'interno della finestra grafica.
		*/
		inline void PresentRenderer() throw();

		/*! Pulisce il renderer dello ScreenVideo.
		*/
		inline void CleanRenderer() throw();

		//! Setta il colore di background del renderer.
		inline void SetColorBackgroundRenderer(const Color& color_setting) throw();

		//! @return		La stringa del nome del driver grafico.
		static const std::string GetName_VideoDriver() throw();
		
		/*! @param [in] index_display	L'indice del display di cui si vuole conoscere l'informazione.
			@return						Una lista di tutte le modalità supportate dal monitor.
			@throw						Se non esiste il monitor con l'indice espresso dal parametro di input.
		*/
		typedef std::shared_ptr<SDL_DisplayMode> ptrSDL_DisplayMode;
		static std::vector<ptrSDL_DisplayMode> GetAll_AvailableDisplayMode(const int index_display) throw(...);


		inline void DrawTexture(const Texture& src_texture, const Point xy_onRenderer, const Rect area_renderer_active = Rect(0,0,-1,-1)) throw(...);
		inline void DrawTextureScaled(const Texture& src_texture, const Point xy_onRenderer, const size_t w_size, const size_t h_size,const Rect area_renderer_active = Rect(0,0,-1,-1)) throw(...);

	private:
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;
		size_t m_wsizeRenderer;
		size_t m_hsizeRenderer;
		bool m_fullscreen;
		Uint32 m_id_window;
		std::string m_title_win;
		Uint32 m_ms_last_present_call;
		Uint32 m_ms_min_call_present;
	protected:
		virtual void NotificationEvent(const SDL_Event& event) override;
	};

	inline Texture ScreenVideo::MakeVoidTexture() const throw(...){
		if(this->m_renderer==nullptr) 
			throw Error("ScreenVideo","MakeVoidTexture","Impossibile instanziare una texture senza un 'renderer' definito!");
		return Texture(this->m_renderer);
	}

	inline const bool ScreenVideo::IsOpen() const throw(){
		if(m_window!=nullptr) return true;
		return false;
	}

	inline const size_t ScreenVideo::Get_WSizeRenderer() const throw(){
		return m_wsizeRenderer;
	}

	inline const size_t ScreenVideo::Get_HSizeRenderer() const throw(){
		return m_hsizeRenderer;
	}

	inline const Uint32 ScreenVideo::Get_ID_Window() const throw(){
		return m_id_window;
	}

	inline void ScreenVideo::PresentRenderer() throw(){
		if(m_ms_min_call_present!=0){
			const Uint32 delay = SDL_GetTicks() - m_ms_last_present_call;
			if(delay < m_ms_min_call_present ) SDL_Delay(m_ms_min_call_present - delay);
		}
		SDL_RenderPresent(m_renderer);
		m_ms_last_present_call = SDL_GetTicks();
	}

	inline void ScreenVideo::CleanRenderer() throw(){
		SDL_RenderClear(m_renderer);
	}

	inline void ScreenVideo::SetColorBackgroundRenderer(const Color& color_setting) throw(){
		SDL_SetRenderDrawColor(m_renderer,color_setting.Get_RedComponent(),
			color_setting.Get_GreenComponent(),color_setting.Get_BlueComponent(),color_setting.Get_AlphaComponent());
	}

	inline void ScreenVideo::DrawTexture(const Texture& src_texture, const Point xy_onRenderer, const Rect area_renderer_active) throw(...){
		if(src_texture.IsVoid()) return;
		SDL_Rect dest_rect;
		dest_rect.x = xy_onRenderer.Get_X_Component();
		dest_rect.y = xy_onRenderer.Get_Y_Component();
		dest_rect.w = src_texture.Get_WsizeDrawable();
		dest_rect.h = src_texture.Get_HsizeDrawable();

		if(area_renderer_active.Get_Xcomponent()==0 && area_renderer_active.Get_Ycomponent()==0 
			&& area_renderer_active.Get_Wcomponent()==-1 && area_renderer_active.Get_Hcomponent()==-1){
				SDL_RenderSetClipRect(m_renderer,NULL);
		}else{
			SDL_Rect clip = area_renderer_active;
			if(clip.w<0) clip.w = m_wsizeRenderer - clip.x;
			if(clip.h<0) clip.h = m_hsizeRenderer - clip.y;
			SDL_RenderSetClipRect(m_renderer,&clip);
		}
		

		if(SDL_RenderCopy(m_renderer,src_texture.m_texture,src_texture.m_drawnable_area,&dest_rect)!=0)
			throw Error("ScreenVideo","DrawTexture","Impossibile effettuare il rendering della texture richiesta!\n%s",SDL_GetError());
	}

	inline void ScreenVideo::DrawTextureScaled(const Texture& src_texture, const Point xy_onRenderer, const size_t w_size, const size_t h_size,const Rect area_renderer_active) throw(...){

	}
}

#endif
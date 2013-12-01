#ifndef __SCREEN_VIDEO_XGAME__HPP
#define __SCREEN_VIDEO_XGAME__HPP

#include <XGame/Video/Config.hpp>
#include <XGame/Core/Error.hpp>
#include <XGame/Core/noncopyable.hpp>
#include <XGame/Event/ObjectInteractive.hpp>
#include <XGame/Video/Texture.hpp>
#include <XGame/Video/Color.hpp>
#include <XGame/Video/Point.hpp>
#include <XGame/Video/Rect.hpp>
#include <SDL2/SDL.h>

#ifdef WIN32 
#pragma warning(disable:4251)
#pragma warning(disable:4275)
#endif

namespace xgame{

	class XGAME_API_VIDEO ScreenVideo : public noncopyable, public ObjectInteractive{
	public:
		//! Costruttore di default. Specificare i parametri dimensionali dell'area del RENDERER
		ScreenVideo(const size_t w_size_renderer =0, const size_t h_size_renderer =0) throw();


		//! Distruttore di default.
		virtual ~ScreenVideo() throw();


		//! Chiude lo ScreenVideo deallocando tutte le risorse dedicate ad esso.
		void Close() throw();


		//! \return		'true' se lo ScreenVideo è stato lanciato ed è attivo, 'false' altrimenti.
		inline const bool IsOpen() const throw();


		/*! Lancia lo ScreenVideo (finestra grafica applicativa che contiene il renderer grafico).
			\note			Prima di lanciare lo ScreenVideo, assicurarsi di settare tutte le impostazioni preferite.
			\throw	Error	In caso di errore critico!
		*/
		void Open() throw(Error);


		//! \return		La larghezza in pixel del renderer.
		inline const size_t Get_WSizeRenderer() const throw();


		//! \return		L'altezza in pixel del renderer.
		inline const size_t Get_HSizeRenderer() const throw();

		//!	\return		La larghezza in pixel della finestra grafica.
		inline const size_t Get_WSizeWindow() const throw();

		//!	\return		L'altezza in pixel della finestra grafica.
		inline const size_t Get_HSizeWindow() const throw();

		/*! \return		L'ID (univoco) della finestra grafica.
			\note		Ritorna '-1' in caso la finestra grafica non sia stata ancora lanciata
		*/
		inline const Uint32 Get_ID_Window() const throw();


		/*! Modica e aggiorna la dimensione del renderer grafico.
			\throw	Error		In caso l'aggiornamento non vada a buonfine.
			\note	Se viene chiamato questo metodo quando lo ScreenVideo non è attivo le dimensione saranno effettive al prossimo avvio.
		*/
		void UpdateSize_Renderer(const size_t wSize_renderer, const size_t hSize_renderer) throw(Error);


		//! Modifica e aggiorna la dimensione della finestra che contiene il renderer grafico.
		void UpdateSize_GraphicWindow(const size_t wSize_window, const size_t hSize_window) throw();


		//! Modifica ed aggiorna il titolo della finestra che contiene il render grafico.
		void UpdateName_GraphicWindow(const std::string& title_set) throw();


		/*! Setta la modalità 'fullscreen' o 'window' per la finestra grafica.
			\note	Se lo ScreenVideo è stato già stato aperto è necessario chiudere e riaprire lo ScreenVideo.
		*/
		void Set_FullScreenWindow(const bool fullscreen_active) throw();


		/*! Setta il numero di volte MASSIMO che il renderer può essere presentato sulla finestra grafica.
			\note	Inserire '0' come parametro per disattivare il controllo sui fps massimi (default)
		*/
		void Set_FramesPerSecond_MAX(const size_t fps_max) throw();


		/*! Abilita o meno il v-sync.
			\note	Probabilmente è necessario chiamare questo metodo all'avvio dell'applicazione.
			\throw	Error	In caso non il vsync non sia attivabile.
		*/
		void SetHint_VSync(const bool vsync_active) throw(Error);


		//! Opzioni di qualità per lo SCALING da applicare al motore grafico.
		enum class SCALING_QUALITY{
			LOW_QUALITY,
			MEDIUM_QUALITY,
			HIGH_QUALITY
		};
		/*! Setta la qualità dello scaling del renderer.
			\note				La qualità 'HIGH_QUALITY' è disponibile solo per driver grafici 'direct3D'; mentre la qualità 'MEDIUM_QUALITY'
								è supportata sia per 'opengl' che per 'direct3D'.
			\throw	Error		In caso non la qualità non sia supportata dal driver video.
		*/
		void SetScaleQuality(const SCALING_QUALITY setting_quality) throw(Error);


		//! Effettua la prensentazione del renderer all'interno della finestra grafica.
		inline void PresentRenderer();


		//! Pulisce il renderer dello ScreenVideo.
		inline void CleanRenderer();


		//! Setta il colore di background del renderer.
		inline void SetColorBackgroundRenderer(const Color& color_setting) throw();


		//! \return		La stringa del nome del driver grafico.
		static const std::string GetName_VideoDriver() throw();
		

		//! Puntatore ad un SDL_DisplayMode.
		typedef std::shared_ptr<SDL_DisplayMode> ptrSDL_DisplayMode;
		
		/*! \param [in] index_display	L'indice del display (monitor) di cui si vuole conoscere l'informazione.
			\return						Una lista di tutte le modalità supportate dal monitor.
			\throw						Se non esiste il monitor con l'indice espresso dal parametro di input.
		*/
		static std::vector<ptrSDL_DisplayMode> GetAll_AvailableDisplayMode(const int index_display =0) throw(Error);

		/*!	Calcola la risoluzione del monitor all'atto di questa chiamata (corrente).
			\param [in] index_display	L'indice del display (monitor) di cui si vuole conoscere l'informazione.
			\return						La modalità (risoluzione) corrente del display (monitor).
			\throw Error				Se non esiste il monitor con l'indice di input.
		*/
		static ptrSDL_DisplayMode GetCurrent_DisplayMode(const int index_display =0) throw(Error);

		/*!	Ritorna la risoluzione più 'vicina' (prossima) alla risoluzione di input.
			\param [in] input_near		La risoluzione che si vorrebbe valutare.
			\param [in] index_display	L'indice del display (monitor) di cui si vuole conoscere l'informazione.

			\return						La modalità (risoluzione) del display (monitor) più prossima a quella di ingresso.

			\throw Error				Se non esiste il monitor con l'indice di input.
										O se non esiste una modalità grafica prossima a quella in input.
		*/
		static ptrSDL_DisplayMode GetClosest_DisplayModeTo(const ptrSDL_DisplayMode& input_near,const int index_display = 0) throw(Error);


		/*!	Disegna una texture sul renderer. La texture sarà mostrata al prossimo 'PresentRenderer'.
			
			\param [in] src_texture				La texture di input da 'blittare' sul renderer.
			\param [in] xy_onRenderer			La posizione (in pixel) sul renderer che indica DOVE disegnare la texture.
												Nota: l'angolo in alto a sinistra rappresenta il pixel (0,0).
			\param [in] area_renderer_active	L'area del renderer abilitata al disegno per questo disegno.
												Se la texture non si troverà all'interno di quest area non sarà visibile.
												Specificare le componenti '-1' per indicare tutto lo schermo.
			
			\throw Error						In caso di errore video interno.
		*/
		void DrawTexture(const Texture& src_texture, const Point& xy_onRenderer, const Rect& area_renderer_active = Rect(0,0,-1,-1)) throw(Error);

		/*! Disegna un punto (un pixel) sul renderer.
			\param [in] src_point				Le coordinate sul renderer di dove si vuole disegnare il punto.
			\param [in] area_renderer_active	L'area del renderer abilitata al disegno per questo disegno.
												Se la texture non si troverà all'interno di quest area non sarà visibile.
												Specificare le componenti '-1' per indicare tutto lo schermo.

			\note								Il punto verrà disegnato col colore impostato precedentemente dal
												metodo ScreenVideo::SetColorBackgroundRenderer.
		*/
		void DrawPoint(const Point& src_point, const Rect& area_renderer_active = Rect(0, 0, -1, -1)) throw(Error);

		/*! Disegna un vettore di punti.
			\param [in] src_points				Un vettore di punti da disegnare.
			\param [in] area_renderer_active	L'area del renderer abilitata al disegno per questo disegno.
												Se la texture non si troverà all'interno di quest area non sarà visibile.
												Specificare le componenti '-1' per indicare tutto lo schermo.

			\note								Il punto verrà disegnato col colore impostato precedentemente dal
												metodo ScreenVideo::SetColorBackgroundRenderer.
			
			\see								ScreenVideo::DrawPoint.
		*/
		inline void DrawVectPoint(const std::vector<Point>& src_points, const Rect& area_renderer_active = Rect(0, 0, -1, -1)) throw(Error);

		//! Operatore di conversione per puntatore a SDL_Window.
		inline explicit operator SDL_Window*() throw();

		//! Operatore di conversione per puntatore a const SDL_Window.
		inline explicit operator const SDL_Window*() const throw();

		//! Operatore di conversione per puntatore a SDL_Renderer.
		inline explicit operator SDL_Renderer*() throw();

		//! Operatore di conversione per puntatore costante a SDL_Renderer.
		inline explicit operator const SDL_Renderer*() const throw();

		//! Costruttore di move eliminato!
		ScreenVideo(ScreenVideo&&) = delete;

		//! Operatore di move eliminato!
		ScreenVideo& operator=(ScreenVideo&&) = delete;

		//! Aggiorna la finestra grafica con gli eventi di input.
		virtual void NotificationEvents(ListEvents& events) override;

	private:
		SDL_Window* m_window =nullptr;
		SDL_Renderer* m_renderer =nullptr;
		size_t m_wsizeRenderer =0;
		size_t m_hsizeRenderer =0;
		size_t m_wsizeWindow =0;
		size_t m_hsizeWindow =0;
		bool m_fullscreen =false;
		Uint32 m_id_window =-1;
		std::string m_title_win;
		Uint32 m_ms_last_present_call =0;
		Uint32 m_ms_min_call_present =0;
		SDL_Rect m_area_active_renderer;

		friend class Texture;
	};

	inline const bool ScreenVideo::IsOpen() const throw(){
		if(m_window!=nullptr) return true;
		return false;
	}

	inline const size_t ScreenVideo::Get_WSizeRenderer() const throw(){
		if (m_renderer == nullptr) return 0;
		return m_wsizeRenderer;
	}

	inline const size_t ScreenVideo::Get_HSizeRenderer() const throw(){
		if (m_renderer == nullptr) return 0;
		return m_hsizeRenderer;
	}

	inline const size_t ScreenVideo::Get_WSizeWindow() const throw(){
		if (m_window == nullptr) return 0;
		int w, h;
		SDL_GetWindowSize(m_window, &w, &h);
		return static_cast<size_t>(w);
	}

	inline const size_t ScreenVideo::Get_HSizeWindow() const throw(){
		if (m_window == nullptr) return 0;
		int w, h;
		SDL_GetWindowSize(m_window, &w, &h);
		return static_cast<size_t>(h);
	}

	inline const Uint32 ScreenVideo::Get_ID_Window() const throw(){
		return m_id_window;
	}

	inline void ScreenVideo::PresentRenderer(){
		if(m_ms_min_call_present!=0){
			const Uint32 delay = SDL_GetTicks() - m_ms_last_present_call;
			if(delay < m_ms_min_call_present ) SDL_Delay(m_ms_min_call_present - delay);
		}
		SDL_RenderPresent(m_renderer);
		m_ms_last_present_call = SDL_GetTicks();
	}

	inline void ScreenVideo::CleanRenderer(){
		SDL_RenderClear(m_renderer);
	}

	inline void ScreenVideo::SetColorBackgroundRenderer(const Color& color_setting) throw(){
		SDL_SetRenderDrawColor(m_renderer,color_setting.Get_RedComponent(),
			color_setting.Get_GreenComponent(),color_setting.Get_BlueComponent(),color_setting.Get_AlphaComponent());
	}

	inline ScreenVideo::operator SDL_Window*() throw(){
		return this->m_window;
	}

	inline ScreenVideo::operator const SDL_Window*() const throw(){
		return this->m_window;
	}

	inline ScreenVideo::operator SDL_Renderer*() throw(){
		return this->m_renderer;
	}

	inline ScreenVideo::operator const SDL_Renderer*() const throw(){
		return this->m_renderer;
	}

	inline void ScreenVideo::DrawVectPoint(const std::vector<Point>& src_points, const Rect& area_renderer_active) throw(Error)
	{
		for (auto& point : src_points)
			this->DrawPoint(point, area_renderer_active);
	}
}

#endif
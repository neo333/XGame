#ifndef __SURFACE_XGAME__HPP
#define __SURFACE_XGAME__HPP

#include <XGame/Video/Config.hpp>
#include <XGame/Core/Error.hpp>
#include <XGame/Loader/MemoryPage.hpp>
#include <XGame/Video/Rect.hpp>
#include <XGame/Video/Color.hpp>
#include <XGame/Video/KeyPoints.hpp>
#include <SDL2/SDL.h>

namespace xgame{
	class XGAME_API_VIDEO Surface{
	public:
		//! Costruttore di default. Costruisce una surface vuota!
		Surface() = default;

		//! Costruisce una Surface di dimensioni designate con pixel di nero trasparente.
		Surface(const size_t w, const size_t h) throw(Error);

		//! Costruisce una Surface di dimensioni designate con pixel di un colore definito.
		Surface(const size_t w, const size_t h, const Color& pixels_color) throw(Error);

		//! Costruttore di copia.
		Surface(const Surface& oth) throw(Error);


		//! Costruttore di move.
		Surface(Surface&& oth) throw();


		//! Distruttore.
		inline virtual ~Surface() throw();


		//! Operatore di assegnazione.
		Surface& operator=(const Surface& oth) throw(Error);


		//!	Operatore di move.
		Surface& operator=(Surface&& oth) throw();


		//! Pulisce la surface deallocando le risorse.
		virtual void Clean() throw();


		/*!	Carica la surface da un'immagine memorizzata su una pagina di memoria.
			
			\param [in] page_input		Pagina di memoria contenente un'immagine (in formato png,jpg,tiff,...) da caricare
										sulla surface.

			\throw Error				In caso la pagina di memoria non sia valida, o qualche errore di caricamento interno.
		*/
		void LoadSurface_fromMemoryPage(const MemoryPage& page_input) throw(Error);


		//!	\return 'true' se la Surface è vuota.
		inline const bool Is_Void() const throw();
		
		//!	\return	La larghezza totale della surface.
		inline const size_t Get_W() const throw();

		//!	\return	L' altezza totale della surface.
		inline const size_t Get_H() const throw();

		/*! Setta l'alpha per tutti i pixel del colore di input presenti sulla surface.
			
			\param [in] color_select		Un colore che si vuole controllare della surface.
			\param [in] alpha_set			Il valore di alpha da settare per tutti i pixel della surface di quella surface.

			\note	Se la surface è vuota questo metodo non farà nulla.
			\note	La componente 'Alpha' del parametro di ingresso 'color_select' non è di interesse.
		*/
		void Set_ModAlpha_forThisColor(const Color& color_select,const Uint8 alpha_set);

		/*!	"Incolla" una surface di input su questa. Esegue un blit veloce in memoria centrale.
			
			\param [in] input_surface		La surface che deve essere incollata.
			\param [in] xy_blit				La posizione sulla surface destinazione (l'oggetto in questione) dove eseguire il blit.
			\param [in,out] area_cut		La porzione della surface di input da incollare.

			\note Questa funzione tiene in conto delle trasparenze della surface di input, ma non quelle della surface destinazione.
					Se la destione contiene delle aree trasparenti non verrà eseguito il blit su quelle aree.
			\note							Specificare dimensioni negative per il parametro 'area_cut' per indicare 'tutta la dimensione'.

			\throw Error					In caso di errori interni.
		*/
		inline void BlitSurfaceOnThis(const Surface& input_surface, const Point& xy_blit, Rect& area_cut) throw(Error);

		/*!	\see		Surface::BlitSurfaceOnThis(const Surface&, const Point&, Rect&)
			\note		Stessa funzionalità, ma con l'ultimo parametro di default!
							Rect(0,0,-1,-1).
		*/
		inline void BlitSurfaceOnThis(const Surface& input_surface, const Point& xy_blit) throw(Error);

		//! Operatore di conversione per SDL_Surface.
		inline explicit operator SDL_Surface*() throw();

		//! Operatore di conversione per const SDL_Surface.
		inline explicit operator const SDL_Surface*() const throw();

		/*! Operatore di assegnazione di una SDL_Surface a basso livello.
			\note	Verrà fatta una COPIA della SDL_Surface* di input.
		*/
		Surface& operator=(SDL_Surface* oth_surface) throw(Error);

		/*! Ricava i 'KeyPoints' (punti chiave) dalla surface.
			Questi insieme di punti solitamente coincidono approssimativamente col bordo del soggetto
			della surface (i pixel trasparenti [alpha==0] vengono omessi).

			\param [in] input_surface		Una surface di cui si vogliono calcolare i keypoints.
			\return							I keyPoints relativi alla surface di input.
		*/
		static KeyPoints GetKeyPoint_fromSurface(const Surface& input_surface) throw(Error);

	protected:
		SDL_Surface* m_surface = nullptr;
		friend class Texture;

		static const Uint32 rmask = 0xff000000;
		static const Uint32 gmask = 0x00ff0000;
		static const Uint32 bmask = 0x0000ff00;
		static const Uint32 amask = 0x000000ff;
	};

	inline Surface::~Surface() throw(){
		this->Clean();
	}

	inline Surface::operator SDL_Surface*() throw(){ return this->m_surface; }

	inline Surface::operator const SDL_Surface*() const throw(){ return this->m_surface; }

	inline const bool Surface::Is_Void() const throw(){ 
		if (m_surface == nullptr) return true;
		return false;
	}

	inline const size_t Surface::Get_W() const throw(){
		if (m_surface!=nullptr)
			return m_surface->w;
		return 0;
	}

	inline const size_t Surface::Get_H() const throw(){
		if (m_surface!=nullptr)
			return m_surface->h;
		return 0;
	}

	inline void Surface::BlitSurfaceOnThis(const Surface& input_surface, const Point& xy_blit, Rect& area_cut) throw(Error){
		if (input_surface.Is_Void()) return;
		if (area_cut.Get_Wcomponent() < 0) area_cut.Set_Wcomponent(input_surface.Get_W() - area_cut.Get_Xcomponent());
		if (area_cut.Get_Hcomponent() < 0) area_cut.Set_Hcomponent(input_surface.Get_H() - area_cut.Get_Ycomponent());

		if (SDL_BlitSurface(input_surface.m_surface, 
			static_cast<const SDL_Rect*>(area_cut), 
			m_surface,
			static_cast<SDL_Rect*>(Rect(xy_blit.Get_X_Component(), xy_blit.Get_Y_Component(), area_cut.Get_Wcomponent(), area_cut.Get_Hcomponent())))
			!= 0)
		{
			throw Error("Surface", "BlitSurfaceOnThis", "Impossibile eseguire il blit della surface\n%s", SDL_GetError());
		}
	}

	inline void Surface::BlitSurfaceOnThis(const Surface& input_surface, const Point& xy_blit) throw(Error){
		Rect default_area = Rect(0,0,-1,-1);
		this->BlitSurfaceOnThis(input_surface,xy_blit,default_area);
	}
}

#endif

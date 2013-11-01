#ifndef __TEXTURE_XGAME__HPP
#define __TEXTURE_XGAME__HPP

#include <XGame/Core/Config.hpp>
#include <XGame/Core/Error.hpp>
#include <XGame/Loader/MemoryPage.hpp>
#include <XGame/Video/Rect.hpp>
#include <SDL/SDL.h>

namespace xgame{
	class XGAME_API Texture{
	public:
		//! Distruttore
		virtual ~Texture();
		
		//! Svuota la texture
		void Clean() throw();

		/*! Carica l'oggetto texture con un'immagine (formato png,jpg,gif,ecc..) caricata su una pagina di memoria.
			Se il formato supporta le trasparenze la texture verrà caricata seguendo le trasparenze del formato.

			@param [in]	input_page		La pagina di memoria dove sono i conservati i dati relativi ad una determinata
										immagine memorizzata correttamente secondo un formato conosciuto.
			@throw Error				Lancia un'eccezione nel caso la lettura della pagina di memoria non vada a buon fine.
			@note						Questa funzione ripristina l'area di disegno della texture.
		*/
		void LoadTexture_fromMemoryPage(const MemoryPage& input_page) throw(...);

		//! @return	La dimensione W (rispetto all porzione di disegno) della texture (0 in caso la texture sia vuota!)
		inline int Get_WsizeDrawable() const throw();

		//! @return	La dimensione H (rispetto all porzione di disegno) della texture (0 in caso la texture sia vuota!)
		inline int Get_HsizeDrawable() const throw();


		//! Costruttore di copia
		Texture(const Texture& oth) throw(...);

		//! Operatore di assegnazione
		Texture& operator=(const Texture& oth) throw(...);

		//! Costruttore di move
		Texture(Texture&& oth);

		//! Operatore di move
		Texture& operator=(Texture&& oth) throw();

		//! @return 'true' se la texture è vuota
		inline const bool IsVoid() const throw();

		/*! Setta una porzione della texture.
			Quando la texture verrà disegnata su schermo, verrà vista come se fosse solo quella porzione.
			@note	Questo metodo va chiamato dopo aver caricato la texture! Ad ogni nuovo caricamento della texture
					la porzione disegnabile verrà ripristinata!
			@note	Utilizzare un rettangolo con dimensioni negative per indicare 'tutta la dimensione della texture'
		*/
		inline void Set_DrawnableArea(const Rect area) throw();

		//! @return		La porzione della texture che può essere disegnata sullo schermo.
		inline const Rect Get_DrawnableArea() const throw();


	private:
		SDL_Texture* m_texture;
		SDL_Renderer* m_render;
		int m_w_size;
		int m_h_size;
		Rect m_drawnable_area;

		//! Costruttore di inizializzazione con renderer
		friend class ScreenVideo;
		Texture(SDL_Renderer* render_subject) throw();

		/*Ritorna una COPIA della texture interna di una class.Texture
			NOTA: ritorna 'nullptr' in caso di sorgente vuota.
			Lancia un'eccezione in caso di accesso non riuscito della sorgente
		*/
		static SDL_Texture* CopyInternalTexture(const Texture& src) throw(...);
	};

	inline int Texture::Get_WsizeDrawable() const throw(){
		return this->m_drawnable_area.Get_Wcomponent();
	}

	inline int Texture::Get_HsizeDrawable() const throw(){
		return this->m_drawnable_area.Get_Hcomponent();
	}

	inline const bool Texture::IsVoid() const throw(){
		if(m_texture!=nullptr) return false;
		return true;
	}

	inline void Texture::Set_DrawnableArea(const Rect area) throw(){
		m_drawnable_area = area;
		if(m_drawnable_area.Get_Xcomponent() < 0) 
			m_drawnable_area.Set_Xcomponent(0);
		if(m_drawnable_area.Get_Ycomponent() < 0) 
			m_drawnable_area.Set_Ycomponent(0);

		if(m_drawnable_area.Get_Wcomponent() > m_w_size - m_drawnable_area.Get_Xcomponent()) 
			m_drawnable_area.Set_Wcomponent(m_w_size - m_drawnable_area.Get_Xcomponent());
		if(m_drawnable_area.Get_Hcomponent() > m_h_size - m_drawnable_area.Get_Ycomponent()) 
			m_drawnable_area.Set_Hcomponent(m_h_size - m_drawnable_area.Get_Ycomponent());
	}

	inline const Rect Texture::Get_DrawnableArea() const throw(){ return this->m_drawnable_area; }
}

#endif
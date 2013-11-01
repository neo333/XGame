#ifndef __TEXTURE_XGAME__HPP
#define __TEXTURE_XGAME__HPP

#include <XGame/Core/Config.hpp>
#include <XGame/Core/Error.hpp>
#include <XGame/Loader/MemoryPage.hpp>
#include <XGame/Video/Rect.hpp>
#include <SDL/SDL.h>

namespace xgame{

	class ScreenVideo;

	class XGAME_API Texture{
	public:
		//! Costruttore di default. Instanzia una Texture vuota.
		Texture() throw();

		//! Distruttore
		virtual ~Texture();
		
		//! Svuota la texture
		void Clean() throw();

		/*! Carica l'oggetto texture con un'immagine (formato png,jpg,gif,ecc..) caricata su una pagina di memoria.
			Se il formato supporta le trasparenze la texture verrà caricata seguendo le trasparenze del formato.

			@param [in]	input_page		La pagina di memoria dove sono i conservati i dati relativi ad una determinata
										immagine memorizzata correttamente secondo un formato conosciuto.
			@param [in] makerVideo		Uno ScreenVideo 'NECESSARIAMENTE APERTO' per il formato del driver video.
			@throw Error				Lancia un'eccezione nel caso la lettura della pagina di memoria non vada a buon fine.
										O in caso si tenti di caricare la texture con uno ScreenVideo non caricato.
			@note						Questa funzione ripristina l'area di disegno della texture.
		*/
		void LoadTexture_fromMemoryPage(const MemoryPage& input_page, const ScreenVideo& makerVideo) throw(...);

		/*! @return		La dimensione in pixel in larghezza della texture tenendo presente le trasformazioni operate
						su di essa (ridimensionamento, taglio dell'area di disegno, ecc..);
		*/
		inline int Get_WsizeDrawn() const throw();

		/*! @return		La dimensione in pixel in altezza della texture tenendo presente le trasformazioni operate
						su di essa (ridimensionamento, taglio dell'area di disegno, ecc..);
		*/
		inline int Get_HsizeDrawn() const throw();


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
			@note	Chiamando questa funzione vengono perse le modifiche fatte sullo ridimensionamento della texture!
		*/
		inline void Set_DrawnableArea(const Rect area) throw();

		//! @return		La porzione della texture che può essere disegnata sullo schermo.
		inline const Rect& Get_DrawnableArea() const throw();

		/*! Setta una nuova dimensione in pixel in larghezza.
			Al prossimo render la texture verrà scalata per rientrare nella nuova dimensione richiesta.
			@note Il metodo 'Texture::Set_DrawnableArea' annulla le trasformazioni fatte da questo metodo.
		*/
		inline void Resize_W_Texture(const size_t w_size) throw();

		/*! Setta una nuova dimensione in pixel in altezza.
			Al prossimo render la texture verrà scalata per rientrare nella nuova dimensione richiesta.
			@note Il metodo 'Texture::Set_DrawnableArea' annulla le trasformazioni fatte da questo metodo.
		*/
		inline void Resize_H_Texture(const size_t h_size) throw();

	private:
		SDL_Texture* m_texture;
		SDL_Renderer* m_render;
		int m_w_size;
		int m_h_size;
		Rect m_drawnable_area;
		int m_w_size_scaled;
		int m_h_size_scaled;

		friend class ScreenVideo;

		/*Ritorna una COPIA della texture interna di una class.Texture
			NOTA: ritorna 'nullptr' in caso di sorgente vuota.
			Lancia un'eccezione in caso di accesso non riuscito della sorgente
		*/
		static SDL_Texture* CopyInternalTexture(const Texture& src) throw(...);
	};

	inline int Texture::Get_WsizeDrawn() const throw(){
		return this->m_w_size_scaled;
	}

	inline int Texture::Get_HsizeDrawn() const throw(){
		return this->m_h_size_scaled;
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

		this->m_w_size_scaled = m_drawnable_area.Get_Wcomponent();
		this->m_h_size_scaled = m_drawnable_area.Get_Hcomponent();
	}

	inline const Rect& Texture::Get_DrawnableArea() const throw(){ return this->m_drawnable_area; }

	inline void Texture::Resize_W_Texture(const size_t w_size) throw(){
		m_w_size_scaled = w_size;
	}
	inline void Texture::Resize_H_Texture(const size_t h_size) throw(){
		m_h_size_scaled = h_size;
	}
}

#endif
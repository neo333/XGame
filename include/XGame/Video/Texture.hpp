#ifndef __TEXTURE_XGAME__HPP
#define __TEXTURE_XGAME__HPP

#include <XGame/Video/Config.hpp>
#include <XGame/Core/Error.hpp>
#include <XGame/Loader/MemoryPage.hpp>
#include <XGame/Video/Rect.hpp>
#include <XGame/Video/Point.hpp>
#include <XGame/Video/Color.hpp>
#include <XGame/Video/Surface.hpp>
#include <XGame/Video/Font.hpp>
#include <SDL2/SDL.h>

namespace xgame{

	class ScreenVideo;

	class XGAME_API_VIDEO Texture{
	public:
		//! Costruttore di default. Instanzia una Texture vuota.
		Texture() throw();

		/*! Costruisce una texture copiando una surface di input.
			\param [in]	input_surface		La surface da cui copiare il contenuto grafico.
			\param [in]	makerVideo			Uno ScreenVideo 'NECESSARIAMENTE APERTO' per il formato del driver video.

			\throw Error					In caso di errore grafico interno.
		*/
		Texture(const Surface& input_surface, const ScreenVideo& makerVideo) throw(Error);

		/*! Costruisce una texture non vuota! Con una determinata area.
			I pixel interni saranno tutti neri!
		*/
		Texture(const size_t w_size, const size_t h_size, const ScreenVideo& makerVideo) throw(Error);

		//! Distruttore.
		virtual ~Texture() throw();
		
		//! Svuota la texture. La texture diventerà vuota.
		void Clean() throw();


		/*! Carica l'oggetto texture con un'immagine (formato png,jpg,gif,ecc..) caricata su una pagina di memoria.
			Se il formato supporta le trasparenze la texture verrà caricata seguendo le trasparenze del formato.

			\param [in]	input_page		La pagina di memoria dove sono i conservati i dati relativi ad una determinata
										immagine memorizzata correttamente secondo un formato conosciuto.
			\param [in] makerVideo		Uno ScreenVideo 'NECESSARIAMENTE APERTO' per il formato del driver video.
			\throw Error				Lancia un'eccezione nel caso la lettura della pagina di memoria non vada a buon fine.
										O in caso si tenti di caricare la texture con uno ScreenVideo non caricato.
			\note						Questa funzione ripristina l'area di disegno della texture.
		*/
		void LoadTexture_fromMemoryPage(const MemoryPage& input_page, const ScreenVideo& makerVideo) throw(Error);

		/*! Carica l'oggetto texture copiandone i data pixels da una Surface.
			Spesso conviene utilizzare questa strategia per comporre un'immagine con più parti.
			Le Surface sono strutture dati memorizzate in RAM, ma permettono una più rapida e facile gestione: una volta
			composta la surface finale utilizzando questo metodo è possibile trasferirne il contenuto su una Texture.
			Le Texture sono memorizzate in VIDEO RAM ed ottimizzate dalla scheda video per il disegno.

			\param [in]	input_surface		La surface da cui copiare il contenuto grafico.
			\param [in]	makerVideo			Uno ScreenVideo 'NECESSARIAMENTE APERTO' per il formato del driver video.
			\param [in,out]	area_cut		La porzione della Surface che si vuole copiare.

			\note							Questa funzione ripristina l'area di disegno della texture.
			\note							Il parametro 'area_cut' è anche di output perché viene 'ridimensionato' dalla funzione
											nel caso le grandezze siano troppo grandi rispetto alle dimensioni della surface da copiare.

			\throw Error					In caso di errore grafico interno.
		*/
		void LoadTexture_fromSurface(const Surface& input_surface, const ScreenVideo& makerVideo, Rect& area_cut) throw(Error);

		/*!	\see 		Texture::LoadTexture_fromSurface(const Surface&, const ScreenVideo&, Rect&)
		  	\note		Questa funzione copia TUTTA la surface.
		*/
		inline void LoadTexture_fromSurface(const Surface& input_surface, const ScreenVideo& makerVideo) throw(Error);


		/*! Carica la texture renderizzando un testo secondo il font immesso come parametro di input.

			\param [in] input_str			La stringa di input da renderizzare.
			\param [in] input_font			Il font da usare per il rendering del testo.
			\param [in] str_color			Il colore con quale si vuole renderizzare il testo.
			\param [in] makerVideo			Uno ScreenVideo 'NECESSARIAMENTE APERTO' per il formato del driver video.

			\note							Questa metodo ripristina l'area di disegno della texture.
			\note							Questo metodo non fa altro che instanziare una nuova TextSurface ed associarla
											alla texture corrente con i parametri di default.
											Per un maggior controllo (più a basso livello) è possibile utilizzare la class xgame::TextSurface.
			\see							xgame::TextSurface.

			\throw Error					In caso di errore grafico interno.
		*/
		void LoadTexture_fromFont(const std::string& input_str, const Font& input_font, const Color& str_color, const ScreenVideo& makerVideo) throw(Error);


		/*! \return		La dimensione in pixel in larghezza della texture tenendo presente le trasformazioni operate
						su di essa (ridimensionamento, taglio dell'area di disegno, ecc..);
		*/
		inline int Get_WsizeDrawn() const throw();


		/*! \return		La dimensione in pixel in altezza della texture tenendo presente le trasformazioni operate
						su di essa (ridimensionamento, taglio dell'area di disegno, ecc..);
		*/
		inline int Get_HsizeDrawn() const throw();


		//! Costruttore di copia
		Texture(const Texture& oth) throw(Error);


		//! Operatore di assegnazione
		Texture& operator=(const Texture& oth) throw(Error);


		//! Costruttore di move
		Texture(Texture&& oth) throw();


		//! Operatore di move
		Texture& operator=(Texture&& oth) throw();


		//! \return 'true' se la texture è vuota
		inline const bool IsVoid() const throw();


		/*! Setta una porzione della texture.
			Quando la texture verrà disegnata su schermo, verrà vista come se fosse solo quella porzione.
			
			\note	Questo metodo va chiamato dopo aver caricato la texture! Ad ogni nuovo caricamento della texture
					la porzione disegnabile verrà ripristinata!
			\note	Utilizzare un rettangolo con dimensioni negative per indicare 'tutta la dimensione della texture'
			\note	Chiamando questa funzione vengono perse le modifiche fatte sullo ridimensionamento della texture!
		*/
		inline void Set_DrawnableArea(const Rect area) throw();


		//! \return		La porzione della texture che può essere disegnata sullo schermo.
		inline const Rect& Get_DrawnableArea() const throw();


		/*! Setta una nuova dimensione in pixel in larghezza.
			Al prossimo render la texture verrà scalata per rientrare nella nuova dimensione richiesta.
			\note Il metodo 'Texture::Set_DrawnableArea' annulla le trasformazioni fatte da questo metodo.
		*/
		inline void Resize_W_Texture(const size_t w_size) throw();


		/*! Setta una nuova dimensione in pixel in altezza.
			Al prossimo render la texture verrà scalata per rientrare nella nuova dimensione richiesta.
			\note Il metodo 'Texture::Set_DrawnableArea' annulla le trasformazioni fatte da questo metodo.
		*/
		inline void Resize_H_Texture(const size_t h_size) throw();


		/*! Setta la modifica per la trasparenza della texture.
			\note	255 è opaco, 0 è trasparente!
			\throw Error	In caso di errore grafico.
		*/
		inline void Set_AlphaMod(const Uint8 alpha_set) throw(Error);
		

		//! \return la modifica alpha per questa texture.
		inline Uint8 Get_AlphaMod() const throw();
	

		//! Converte l'oggetto texture in un puntatore a SDL_Texture
		inline explicit operator SDL_Texture*() throw();


		//! \see Texture::operator SDL_Texture*()
		inline explicit operator const SDL_Texture*() const throw();


	private:
		SDL_Texture* m_texture;
		SDL_Renderer* m_render;
		int m_w_size;
		int m_h_size;
		Rect m_drawnable_area;
		int m_w_size_scaled;
		int m_h_size_scaled;
		Uint8 m_alpha_mod = SDL_ALPHA_OPAQUE;

		friend class ScreenVideo;

		/*Ritorna una COPIA della texture interna di una class.Texture
			NOTA: ritorna 'nullptr' in caso di sorgente vuota.
			Lancia un'eccezione in caso di accesso non riuscito della sorgente
		*/
		static SDL_Texture* CopyInternalTexture(const Texture& src) throw(Error);
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

	inline void Texture::Set_AlphaMod(const Uint8 alpha_set) throw(Error){
		if (m_texture == nullptr) return;
		if (SDL_SetTextureAlphaMod(m_texture, alpha_set) != 0)
			throw Error("Texture", "Set_AplhaMod", "Impossibile applicare la trasformazione alpha!\n%s", SDL_GetError());
		m_alpha_mod = alpha_set;
	}

	inline Uint8 Texture::Get_AlphaMod() const throw(){
		return m_alpha_mod;
	}

	inline Texture::operator SDL_Texture*() throw(){
		return this->m_texture;
	}

	inline Texture::operator const SDL_Texture*() const throw(){
		return this->m_texture;
	}

	inline void Texture::LoadTexture_fromSurface(const Surface& input_surface, const ScreenVideo& makerVideo) throw(Error){
		Rect default_area = Rect(0,0,-1,-1);
		this->LoadTexture_fromSurface(input_surface,makerVideo,default_area);
	}
}

#endif

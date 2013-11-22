#ifndef __XPICTURE_XGAME__HPP
#define __XPICTURE_XGAME__HPP

#include <XGame/GUI/Config.hpp>
#include <XGame/Video/GraphicComponent.hpp>
#include <mutex>

namespace xgame{
	class XGAME_API_GUI XPicture: public GraphicComponent{
	public:
		//! Costruttore di default.
		XPicture() = default;

		//! Distruttore.
		~XPicture() = default;

		//! Costruttore di copia.
		XPicture(const XPicture&) = default;

		//! Operatore di assegnazione.
		XPicture& operator=(const XPicture&) = default;

		//! Costruttore di move.
		inline XPicture(XPicture&& oth) throw();

		//! Operatore di move. E' thread safe!
		inline XPicture& operator=(XPicture&& oth) throw();

		/*! Disegna il componente grafico su un determinato schermo
			\param [in,out] screen_out		Lo ScreenVideo dove verrà disegnato il componente grafico.
			\param [in] abs_xy				Le cordinate assolute di dove deve essere disegnato il componente sullo ScreenVideo.
			\param [in] area_clip			La porzione dello ScreenVideo abilitata al disegno del suddetto componente
											Verranno disegnati tutti i pixel del componente che si trovano in questa parte dello schermo.

			\note							Questa funzione è thread safe.
		*/
		virtual void DrawOnScreenVideo(ScreenVideo& screen_out, const Point& abs_xy, const Rect& area_clip = Rect(0, 0, -1, -1)) override;

		/*! Carica il componente grafico con un'immagine vuota!
			\param [in] screen_ref		Lo ScreenVideo di riferimento che serve per il caricamento di eventuali texture grafiche.
			\note						Questa funzione è thread safe.
		*/
		virtual void Load(const ScreenVideo& screen_ref) throw() override;

		/*! Carica il componente grafico con un'immagine memorizzata in una pagina di memoria.
			\param [in] screen_ref		Lo ScreenVideo di riferimento che serve per il caricamento di eventuali texture grafiche.
			\param [in] input_page		La pagina di memoria che contiene un'immagine (codificata in uno dei formati standard [png,jpg..])

			\throw Error				In caso di impossibilità nella lettura della pagina di memoria.

			\note						Questa funzione è thread safe.
		*/
		virtual void Load(const ScreenVideo& screen_ref, const MemoryPage& input_page) throw(Error);

		/*! Carica il componente grafico facendo una copia da una texture esterna di input.
			\param [in] screen_ref		Lo ScreenVideo di riferimento che serve per il caricamento di eventuali texture grafiche.
			\param [in] input_texture	Una texture da copiare di input.
			
			\throw Error				In caso di errore interno.

			\note						Questa funzione è thread safe.
		*/
		virtual void Load(const ScreenVideo& screen_ref, const Texture& input_texture) throw(Error);

		/*! Carica il componente grafico facendo una copia da una Surface (immagine RAM) di input.
			\param [in] screen_ref		Lo ScreenVideo di riferimento che serve per il caricamento di eventuali texture grafiche.
			\param [in] input_surface	La surface di input da cui effettuare una copia.
			\param [in,out] area_cut	La porzione della surface da copiare. 
										Specificare '-1' come parametro dimensionale per indicare 'tutta la dimensione'.
										Il parametro è anche di output perché in caso le dimensioni siano sbagliate vengono
										automaticamente corrette da questa funzione.

			\throw Error				In caso di errore interno.

			\note						Questa funzione è thread safe.
		*/
		virtual void Load(const ScreenVideo& screen_ref, const Surface& input_surface, Rect& area_cut = Rect(0,0,-1,-1)) throw(Error);

		//! Dealloca tutte le risorse del componente grafico. Questo metodo è thread safe.
		virtual void UnLoad() override;

		//! \return		'true' se il componente grafico è carico, 'false' altrimenti.  Questo metodo è thread safe.
		inline virtual const bool IsLoad() const override;

		//!	Setta una nuova dimensione (in pixel) in larghezza dell'immagine. Questo metodo è thread safe.
		inline void Resize_W(const size_t w_new_size) throw();

		//! Setta una nuova dimensione (in pixel) in altezza dell'immagine. Questo metodo è thread safe.
		inline void Resize_H(const size_t h_new_size) throw();

		//! Setta delle nuove dimensioni (in pixel) per l'altezza e la larghezza dell'immagine. Questo metodo è thread safe.
		inline void Resize(const size_t w_new_size, const size_t h_new_size) throw();

		//! \return La dimensione (in pixel) della larghezza dell'immagine. (Tenendo conto di eventuali Resize). Questo metodo è thread safe.
		inline int Get_W_SizeDrawn() const throw();

		//! \return La dimensione (in pixel) dell'altezza dell'immagine. (Tenendo conto di eventuali Resize). Questo metodo è thread safe.
		inline int Get_H_SizeDrawn() const throw();

		/*!	Setta la trasparenza (o opacità) dell'immagine.
			Il valore va espresso in percentuale (normalizzato).
			\param [in] alpha_percent	La percentuale (normalizzata nell'intervallo [0,1.0]) dell'opacità.
										1.0 = massima opacità
										0	= massima trasparenza.
			
			\throw	Error				In caso il parametro sia invalido (non contenuto nell'intervallo richiesto).

			\note						Questo metodo è thread safe.
		*/
		inline void Set_Alpha(const float alpha_percent = 1.0f) throw (Error);

		//!	\return		Il valore normalizzato dell'opacità. Questo metodo è thread safe.
		inline float Get_Alpha() const throw();

	private:
		Texture m_dataPicture;
		std::mutex m_dataPicture_mutex;
	};

	inline const bool XPicture::IsLoad() const{
		if (m_dataPicture.IsVoid()) return false;
		return true;
	}

	inline XPicture::XPicture(XPicture&& oth) throw() :
		m_dataPicture(std::move(oth.m_dataPicture))
	{

	}

	inline XPicture& XPicture::operator=(XPicture&& oth) throw()
	{
		if (this != &oth){
			std::unique_lock<std::mutex> lock_mutex_this(m_dataPicture_mutex,std::defer_lock);
			std::unique_lock<std::mutex> lock_mutex_oth(oth.m_dataPicture_mutex,std::defer_lock);
			std::lock(lock_mutex_this, lock_mutex_oth);
			this->m_dataPicture = std::move(oth.m_dataPicture);
		}
		return *this;
	}

	inline void XPicture::Resize_W(const size_t w_new_size) throw(){
		std::unique_lock<std::mutex> lock_mutex(m_dataPicture_mutex);
		m_dataPicture.Resize_W_Texture(w_new_size);
	}
	inline void XPicture::Resize_H(const size_t h_new_size) throw(){
		std::unique_lock<std::mutex> lock_mutex(m_dataPicture_mutex);
		m_dataPicture.Resize_H_Texture(h_new_size);
	}
	inline void XPicture::Resize(const size_t w_new_size, const size_t h_new_size) throw(){
		this->Resize_W(w_new_size);
		this->Resize_H(h_new_size);
	}

	inline int XPicture::Get_W_SizeDrawn() const throw(){
		return this->m_dataPicture.Get_WsizeDrawn();
	}

	inline int XPicture::Get_H_SizeDrawn() const throw(){
		return this->m_dataPicture.Get_HsizeDrawn();
	}

	inline void XPicture::Set_Alpha(const float alpha_percent) throw (Error){
		if (alpha_percent<0 || alpha_percent>1.f)
			throw Error("XPicture", "Set_Alpha", "Parametro alpha non valido: '%s'", std::to_string(alpha_percent));
		std::unique_lock<std::mutex> lock_mutex(m_dataPicture_mutex);
		this->m_dataPicture.Set_AlphaMod(static_cast<Uint8>(255.f*alpha_percent));
	}

	inline float XPicture::Get_Alpha() const throw(){
		return static_cast<float>(this->m_dataPicture.Get_AlphaMod())/255.f;
	}
}

#endif
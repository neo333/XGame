#ifndef __XPICTURE__XGAME__HPP
#define __XPICTURE__XGAME__HPP

#include <XGame/GUI/Config.hpp>
#include <XGame/Video/GraphicComponent.hpp>

namespace xgame{
	class XGAME_API_GUI XPicture :
		public GraphicComponent
	{
	public:
		//! Costruttore di default.
		XPicture() = default;

		//! Distruttore virtuale.
		virtual ~XPicture() = default;

		//! Costruttore di copia.
		XPicture(const XPicture&) = default;

		//! Operatore di assegnazione.
		XPicture& operator=(const XPicture&) = default;

		//! Costruttore di move.
		inline XPicture(XPicture&&) throw();

		//! Operatore di move.
		inline XPicture& operator=(XPicture&&) throw();

		/*! Carica l'immagine da una pagina di memoria contenente un formato immagine supportato.
			\param [in] input_page			La pagina di memoria che contiene l'immagine nel formato supportato da caricare.
			\param [in] outvideo_ref		Un riferimento ad uno ScreenVideo per l'ottimizzazione dell'immagine.

			\see							Texture::LoadTexture_fromMemoryPage

			\note							Questo metodo cancella tutte le modifiche fatte sull'immagine:
												-Resize delle dimensioni.
												-Alpha channel.
		*/
		virtual void LoadPicture_fromMemoryPage(const MemoryPage& input_page, const ScreenVideo& outvideo_ref) throw(Error);
		
		/*!	Disegna il componente grafico su un determinato schermo
			\param[in, out] screen_out		Lo ScreenVideo dove verrà disegnato il componente grafico.
			\param[in] abs_xy				Le cordinate assolute di dove deve essere disegnato il componente sullo ScreenVideo.
			\param[in] area_clip			La porzione dello ScreenVideo abilitata al disegno del suddetto componente
											Verranno disegnati tutti i pixel del componente che si trovano in questa parte dello schermo.
		*/
		virtual void DrawOnScreenVideo(ScreenVideo& screen_out, const Point& abs_xy, const Rect& area_clip = Rect(0, 0, -1, -1)) override;

		/*! Aggiorna l'oggetto interattivo passandogli gli eventi verificatosi.
			\see	ObjectInteractive::NotificationEvents
			\note	Generalmente un'oggetto interattivo cancella l'evento che lo ha interessato dalla lista.
		*/
		virtual void NotificationEvents(xgame::ListEvents& events) throw();

		//! Ridimensiona la larghezza dell'immagine secondo il parametro (in pixel) di input.
		virtual void Resize_W(const size_t new_w) throw();

		//! Ridimensiona l'altezza dell'immagine secondo il parametro (in pixel) di input.
		virtual void Resize_H(const size_t new_h) throw();

		//! Ridimensiona la larghezza e l'altezza dell'immagine secondo i parametri (in pixel) di input.
		inline virtual void Resize_W_H(const size_t new_w, const size_t new_h) throw();

	private:
		Texture m_data_image;
	};

	inline XPicture::XPicture(XPicture&& oth) throw() :
		GraphicComponent(std::move(oth)),
		m_data_image(std::move(oth.m_data_image))
	{

	}

	inline XPicture& XPicture::operator=(XPicture&& oth) throw()
	{
		if (this != &oth){
			GraphicComponent::operator=(std::move(oth));
			this->m_data_image = std::move(oth.m_data_image);
		}
		return *this;
	}

	inline void XPicture::Resize_W_H(const size_t new_w, const size_t new_h) throw()
	{
		this->Resize_W(new_w);
		this->Resize_H(new_h);
	}
}

#endif
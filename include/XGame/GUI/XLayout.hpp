#ifndef __XLAYOUT__XGAME__HPP
#define __XLAYOUT__XGAME__HPP

#include <XGame/GUI/Config.hpp>
#include <XGame/Video/GraphicContainer.hpp>

namespace xgame{
	class XGAME_API_GUI XLayout :
		public GraphicContainer
	{
	public:
		//! Costruttore di default.
		XLayout(const size_t w_layout =0, const size_t h_layout =0);

		//! Distruttore virtuale.
		virtual ~XLayout() = default;

		//! Costruttore di copia.
		XLayout(const XLayout&) = default;

		//! Operatore di assegnazione.
		XLayout& operator=(const XLayout&) = default;

		//! Costruttore di move.
		inline XLayout(XLayout&&) throw();

		//! Operatore di move.
		inline XLayout& operator=(XLayout&&) throw();

		/*!	Disegna tutti i componenti grafici contenuti in questo contenitore.
			\param [in,out] screen_out		Lo ScreenVideo dove verrà disegnato il contenitore grafico.
			\param [in] abs_xy				Le cordinate assolute di dove deve essere disegnato il contenitore sullo ScreenVideo.
			\param [in] area_clip			La porzione dello ScreenVideo abilitata al disegno del suddetto contenitore
											Verranno disegnati tutti i pixel del contenitore che si trovano in questa parte dello schermo.
		*/
		virtual void DrawOnScreenVideo(ScreenVideo& screen_out, const Point& abs_xy, const Rect& area_clip = Rect(0, 0, -1, -1)) override;

		/*!	Notifica tutti i componenti grafici contenuti in questo contenitori con una lista di eventi.
			\note			La notifica verrà gestita con politica LIFO.
							Questo per dare priorità ai componenti che vengono disegnati graficamente sopra gli altri.
		*/
		virtual void NotificationEvents(xgame::ListEvents& events);

		//! \return la larghezza (in pixel) del layout.
		inline const size_t Get_W_Size() const throw();

		//!	\return l'altezza (in pixel) del layout.
		inline const size_t Get_H_Size() const throw();
	};

	inline XLayout::XLayout(XLayout&& oth) throw():
		GraphicContainer(std::move(oth))
	{
	}

	inline XLayout& XLayout::operator=(XLayout&& oth) throw(){
		if (this != &oth){
			GraphicContainer::operator=(std::move(oth));
		}
		return *this;
	}

	inline const size_t XLayout::Get_W_Size() const throw(){
		return this->m_w_size_container;
	}

	inline const size_t XLayout::Get_H_Size() const throw(){
		return this->m_h_size_container;
	}
}

#endif
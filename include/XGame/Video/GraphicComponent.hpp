#ifndef __GRAPHIC_COMPONENT_XGAME__HPP
#define __GRAPHIC_COMPONENT_XGAME__HPP

#include <XGame/Video/Config.hpp>
#include <XGame/Video/Point.hpp>
#include <XGame/Video/ScreenVideo.hpp>

namespace xgame{
	class XGAME_API_VIDEO GraphicComponent{
	public:
		/*! Setta la posizione relativa dell'oggetto sullo schermo.
			\note La posizione è realtiva perché l'oggetto grafico potrebbe trovarsi in un contenitore posto in qualche punto
					dello schermo. Dunque la posizione assoluta dell'oggetto non è definibile a priori.
		*/
		inline void SetPosition(const Point& xy_relative) throw();

		//! \return la posizione relativa dell' oggetto sullo schermo.
		inline const Point& GetPosition() const throw();

		/*! Disegna il componente grafico su un determinato schermo
			\param [in,out] screen_out		Lo ScreenVideo dove verrà disegnato il componente grafico.
			\param [in] abs_xy				Le cordinate assolute di dove deve essere disegnato il componente sullo ScreenVideo.
			\param [in] area_clip			La porzione dello ScreenVideo abilitata al disegno del suddetto componente
											Verranno disegnati tutti i pixel del componente che si trovano in questa parte dello schermo.
		*/
		virtual void DrawOnScreenVideo(ScreenVideo& screen_out, const Point& abs_xy, const Rect& area_clip =Rect(0,0,-1,-1));

		/*! Carica il componente grafico. (Es. letture di immagini dal disco rigido).
			\param [in] screen_ref		Lo ScreenVideo di riferimento che serve per il caricamento di eventuali texture grafiche.
		*/
		virtual void Load(const ScreenVideo& screen_ref);

		//! Dealloca tutte le risorse del componente grafico.
		virtual void UnLoad() =0;

		//! \return		'true' se il componente grafico è carico, 'false' altrimenti.
		virtual const bool IsLoad() const =0;

		//! Costruttore di default.
		GraphicComponent() = default;

		//! Distruttore.
		virtual ~GraphicComponent();

	protected:
		Point m_xy_relative;
		const ScreenVideo* m_screen_maker_realative = nullptr;

		//! \return l'area o porzione del renderer abilitata per l'ultimo DRAWN chiamato designata per l'oggetto in questione.
		inline const Rect& GetRendererAreaClip() const throw();

		//! \return La posizione ASSOLUTA di disegno sullo ScreenVideo riferita all'ultima chiamata di DRAWN.
		inline const Point& GetXYabs() const throw();

	private:
		Rect pri_area_renderer_visible;
		Point m_xy_abs;
	};

	inline void GraphicComponent::SetPosition(const Point& xy_relative) throw(){ this->m_xy_relative = xy_relative; }
	inline const Point& GraphicComponent::GetPosition() const throw(){ return this->m_xy_relative; }
	inline const Rect& GraphicComponent::GetRendererAreaClip() const throw(){ return this->pri_area_renderer_visible; }

	inline const Point& GraphicComponent::GetXYabs() const throw(){ return this->m_xy_abs; }
}

#endif
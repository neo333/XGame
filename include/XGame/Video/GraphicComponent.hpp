#ifndef __GRAPHIC_COMPONENT_XGAME__HPP
#define __GRAPHIC_COMPONENT_XGAME__HPP

#include <XGame/Video/Config.hpp>
#include <XGame/Video/Point.hpp>
#include <XGame/Video/ScreenVideo.hpp>
#include <XGame/Event/ObjectInteractive.hpp>

namespace xgame{
	class XGAME_API_VIDEO GraphicComponent:
		public ObjectInteractive
	{
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

		/*! Aggiorna l'oggetto interattivo passandogli gli eventi verificatosi.
			\see	ObjectInteractive::NotificationEvents
			\note	Generalmente un'oggetto interattivo cancella l'evento che lo ha interessato dalla lista.
		*/
		virtual void NotificationEvents(xgame::ListEvents& events) = 0;

		//! Costruttore di default.
		GraphicComponent() = default;

		//! Distruttore.
		virtual ~GraphicComponent() = default;

		//! Costruttore di copia.
		GraphicComponent(const GraphicComponent&) = default;

		//! Operatore di assegnazione.
		GraphicComponent& operator=(const GraphicComponent&) = default;

		//! Costruttore di move.
		inline GraphicComponent(GraphicComponent&& oth) throw();

		//! Operatore di move.
		inline GraphicComponent& operator=(GraphicComponent&& oth) throw();

	protected:
		Point m_xy_relative;

		//! \return l'area o porzione del renderer abilitata per l'ultimo DRAWN chiamato designata per l'oggetto in questione.
		inline const Rect& GetRendererAreaClip() const throw();

		//! \return La posizione ASSOLUTA di disegno sullo ScreenVideo riferita all'ultima chiamata di DRAWN.
		inline const Point& GetXYabs() const throw();

	private:
		Rect pri_area_renderer_visible;
		Point m_xy_abs;
		friend class GraphicContainer;
	};

	inline void GraphicComponent::SetPosition(const Point& xy_relative) throw()
	{
		this->m_xy_relative = xy_relative; 
	}

	inline const Point& GraphicComponent::GetPosition() const throw()
	{ 
		return this->m_xy_relative; 
	}

	inline const Rect& GraphicComponent::GetRendererAreaClip() const throw()
	{ 
		return this->pri_area_renderer_visible; 
	}

	inline const Point& GraphicComponent::GetXYabs() const throw()
	{
		return this->m_xy_abs; 
	}

	inline GraphicComponent::GraphicComponent(GraphicComponent&& oth) throw() :
		ObjectInteractive(std::move(oth)),
		m_xy_relative(oth.m_xy_relative),
		pri_area_renderer_visible(oth.pri_area_renderer_visible),
		m_xy_abs(oth.m_xy_abs)
	{
		pri_area_renderer_visible.Set_AllComponent(0, 0, 0, 0);
		m_xy_abs.Set_X_Component(0);
		m_xy_abs.Set_Y_Component(0);
	}

	inline GraphicComponent& GraphicComponent::operator=(GraphicComponent&& oth) throw()
	{
		if (this != &oth){
			ObjectInteractive::operator=(std::move(oth));
			this->m_xy_relative = oth.m_xy_relative;
			this->pri_area_renderer_visible = oth.pri_area_renderer_visible;
			this->m_xy_abs = oth.m_xy_abs;

			oth.pri_area_renderer_visible.Set_AllComponent(0, 0, 0, 0);
			oth.m_xy_abs.Set_X_Component(0);
			oth.m_xy_abs.Set_Y_Component(0);
		}
		return *this;
	}
}

#endif

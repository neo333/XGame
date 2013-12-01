#ifndef __GRAPHIC_CONTAINER_XGAME__HPP
#define __GRAPHIC_CONTAINER_XGAME__HPP

#include <XGame/Video/Config.hpp>
#include <XGame/Video/GraphicComponent.hpp>
#include <list>
#include <map>

namespace xgame{	
	class XGAME_API_VIDEO GraphicContainer: 
		public GraphicComponent
	{
	public:
		//! Costruttore di default.
		GraphicContainer() = default;

		//! Distruttore virtuale.
		virtual ~GraphicContainer() = default;

		//! Costruttore di copia.
		GraphicContainer(const GraphicContainer&) = default;

		//! Operatore di assegnazione.
		GraphicContainer& operator=(const GraphicContainer&) = default;

		//! Costruttore di move.
		inline GraphicContainer(GraphicContainer&& oth) throw();

		//! Operatore di move.
		inline GraphicContainer& operator=(GraphicContainer&& oth) throw();

		/*! Inserisce un componente grafico all'interno del contenitore.
			Il contenitore diventa responsabile del componente grafico inserito.
			
			\param [in]	adder_obj		Un puntatore SMART al componente da inserire all'interno del contenitore.
			
			\return						Un puntatore SMART al componente inserito all'interno del contenitore.
		*/
		template<class T> inline std::shared_ptr<T> InsertGraphicComponent(const std::shared_ptr<T>& adder_obj);


	protected:
		size_t m_w_size_container =0;
		size_t m_h_size_container =0;

		typedef std::shared_ptr<GraphicComponent> ptrGraphicComponent;
		typedef std::list<ptrGraphicComponent> ListGraphicComponent;
		ListGraphicComponent m_internComponent;

		/*! Disegna tutti i componenti interni (associati a questo contenitore) con polita FIFO.
			[Il primo inserito è il primo ad essere disegnato sullo ScreenVideo]

			\param [in,out] screen_out		Lo ScreenVideo dove eseguire il disegno di tutti i componenti grafici.

			\note							Gli oggetti verranno disegnati solamente nell'area del contenitore.
											L'area definita dalle variabili membro 'm_w_size_container' & 'm_h_size_container'.
		*/
		virtual void DrawnAllInternalComponents_InSizeArea(ScreenVideo& screen_out) const throw(Error);

		/*! Notifica tutti i componenti interni (associati a questo contenitore) con una lista di eventi che si sono scatenati.
			La notifica viene gestita con polita LIFO.
			[L'ultimo elemento inserito è il primo ad essere notificato]

			\param [in,out] events		La lista di eventi da notificare a tutti i componenti.
			
			\note						Il parametro lista è un parametro di output poiché generalmente i componenti grafici
										eliminano un evento dalla lista che li ha interessati per non attivare
										altri componenti meno prioritari.
		*/
		virtual void NotificationAllInternalComponents(ListEvents& events);
	};

	inline GraphicContainer::GraphicContainer(GraphicContainer&& oth) throw() :
		GraphicComponent(std::move(oth)),
		m_w_size_container(std::move(oth.m_w_size_container)), m_h_size_container(std::move(oth.m_h_size_container)),
		m_internComponent(std::move(oth.m_internComponent))
	{

	}

	inline GraphicContainer& GraphicContainer::operator=(GraphicContainer&& oth) throw()
	{
		if (this != &oth){
			GraphicComponent::operator=(std::move(oth));
			this->m_w_size_container = oth.m_w_size_container;
			this->m_h_size_container = oth.m_h_size_container;
			this->m_internComponent = std::move(oth.m_internComponent);
			oth.m_w_size_container = 0;
			oth.m_h_size_container = 0;
		}
		return *this;
	}

	template<class T> inline std::shared_ptr<T> GraphicContainer::InsertGraphicComponent(const std::shared_ptr<T>& adder_obj)
	{
		m_internComponent.push_back(adder_obj);
		return adder_obj;
	}
}

#endif

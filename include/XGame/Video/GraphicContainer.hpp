#ifndef __GRAPHIC_CONTAINER_XGAME__HPP
#define __GRAPHIC_CONTAINER_XGAME__HPP

#include <XGame/Video/Config.hpp>
#include <XGame/Video/GraphicComponent.hpp>
#include <list>

namespace xgame{
	class XGAME_API_VIDEO GraphicContainer : public GraphicComponent{
	public:
		virtual void DrawOnScreenVideo(ScreenVideo& screen_out) = 0;
		virtual void Load(const ScreenVideo& screen_ref) =0;
		virtual void UnLoad() =0;

		void InsertGraphicComponent(GraphicComponent* adder);
	protected:
		std::list<GraphicComponent*> m_internComponent;
	};
}

#endif
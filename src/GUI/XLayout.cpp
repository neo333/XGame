#include <XGame/GUI/XLayout.hpp>

namespace xgame{
	XLayout::XLayout(const size_t w_layout, const size_t h_layout) :
		GraphicContainer()
	{
		this->m_w_size_container = w_layout;
		this->m_h_size_container = h_layout;
	}

	void XLayout::DrawOnScreenVideo(ScreenVideo& screen_out, const Point& abs_xy, const Rect& area_clip){
		GraphicContainer::DrawOnScreenVideo(screen_out, abs_xy, area_clip);
		this->DrawnAllInternalComponents_InSizeArea(screen_out);
	}

	void XLayout::NotificationEvents(xgame::ListEvents& events){
		this->NotificationAllInternalComponents(events);
	}
}
#include <XGame/Video/GraphicComponent.hpp>

namespace xgame{
	GraphicComponent::~GraphicComponent() throw(){  }

	void GraphicComponent::Load(const ScreenVideo& screen_ref){
		this->m_screen_maker_relative = &screen_ref;
	}

	void GraphicComponent::DrawOnScreenVideo(ScreenVideo& screen_out, const Point& abs_xy, const Rect& area_clip){
		this->pri_area_renderer_visible = area_clip;
		if (pri_area_renderer_visible.Get_Wcomponent() < 0)
			pri_area_renderer_visible.Set_Wcomponent(screen_out.Get_WSizeRenderer() - pri_area_renderer_visible.Get_Xcomponent());
		if (pri_area_renderer_visible.Get_Hcomponent() < 0)
			pri_area_renderer_visible.Set_Hcomponent(screen_out.Get_HSizeRenderer() - pri_area_renderer_visible.Get_Ycomponent());
		this->m_xy_abs = abs_xy;
	}
}

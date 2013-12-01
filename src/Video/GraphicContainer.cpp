#include <XGame/Video/GraphicContainer.hpp>

namespace xgame{
	void GraphicContainer::DrawnAllInternalComponents_InSizeArea(ScreenVideo& screen_out) const throw(Error){
		Rect scissor_area;
		for (auto& obj : m_internComponent)
		{
			if (Rect::Rects_Intersection(
					Rect(this->GetXYabs().Get_X_Component(), this->GetXYabs().Get_Y_Component(), this->m_w_size_container, this->m_h_size_container),
					this->GetRendererAreaClip(),
					scissor_area))
			{
				obj->DrawOnScreenVideo(screen_out,this->GetXYabs() + obj->m_xy_relative,scissor_area);
			}
		}
	}

	void GraphicContainer::NotificationAllInternalComponents(ListEvents& events){
		for (auto obj = m_internComponent.rbegin(); obj != m_internComponent.rend(); obj++)
			(*obj)->NotificationEvents(events);
	}
}
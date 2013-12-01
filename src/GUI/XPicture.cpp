#include <XGame/GUI/XPicture.hpp>
#include <XGame/Loader/ModuleLoader.hpp>

namespace xgame{
	void XPicture::LoadPicture_fromMemoryPage(const MemoryPage& input_page, const ScreenVideo& outvideo_ref) throw(Error)
	{
		m_data_image.LoadTexture_fromMemoryPage(input_page, outvideo_ref);
	}

	void XPicture::DrawOnScreenVideo(ScreenVideo& screen_out, const Point& abs_xy, const Rect& area_clip)
	{
		GraphicComponent::DrawOnScreenVideo(screen_out, abs_xy, area_clip);
		screen_out.DrawTexture(m_data_image, abs_xy, area_clip);
	}

	void XPicture::NotificationEvents(xgame::ListEvents& events) throw()
	{
		//NOP
	}

	void XPicture::Resize_W(const size_t new_w) throw()
	{
		m_data_image.Resize_W_Texture(new_w);
	}

	void XPicture::Resize_H(const size_t new_h) throw()
	{
		m_data_image.Resize_H_Texture(new_h);
	}
}
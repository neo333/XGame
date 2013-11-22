#include <XGame/GUI/XPicture.hpp>

namespace xgame{
	void XPicture::Load(const ScreenVideo& screen_ref) throw(){
		GraphicComponent::Load(screen_ref);
		std::unique_lock<std::mutex> lock_mutex(m_dataPicture_mutex);
		m_dataPicture.Clean();
	}

	void XPicture::UnLoad(){
		std::unique_lock<std::mutex> lock_mutex(m_dataPicture_mutex);
		this->m_dataPicture.Clean();
	}

	void XPicture::DrawOnScreenVideo(ScreenVideo& screen_out, const Point& abs_xy, const Rect& area_clip){
		GraphicComponent::DrawOnScreenVideo(screen_out, abs_xy, area_clip);
		std::unique_lock<std::mutex> lock_mutex(m_dataPicture_mutex);
		screen_out.DrawTexture(m_dataPicture, abs_xy, area_clip);
	}

	void XPicture::Load(const ScreenVideo& screen_ref, const MemoryPage& input_page) throw(Error){
		GraphicComponent::Load(screen_ref);
		std::unique_lock<std::mutex> lock_mutex(m_dataPicture_mutex);
		this->m_dataPicture.LoadTexture_fromMemoryPage(input_page, screen_ref);
	}

	void XPicture::Load(const ScreenVideo& screen_ref, const Texture& input_texture) throw(Error){
		GraphicComponent::Load(screen_ref);
		std::unique_lock<std::mutex> lock_mutex(m_dataPicture_mutex);
		this->m_dataPicture = input_texture;
	}

	void XPicture::Load(const ScreenVideo& screen_ref, const Surface& input_surface, Rect& area_cut) throw(Error){
		GraphicComponent::Load(screen_ref);
		std::unique_lock<std::mutex> lock_mutex(m_dataPicture_mutex);
		this->m_dataPicture.LoadTexture_fromSurface(input_surface, screen_ref, area_cut);
	}
}
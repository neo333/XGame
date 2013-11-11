#ifndef __XBUTTON_XGAME__HPP
#define __XBUTTON_XGAME__HPP

#include <XGame/GUI/Config.hpp>
#include <XGame/Video/GraphicComponent.hpp>
#include <XGame/Event/ObjectInteractive.hpp>
#include <functional>

namespace xgame{
	class XGAME_API_GUI XButton : public GraphicComponent, public ObjectInteractive{
	public:

		virtual void Load(const ScreenVideo& screen_ref) throw(...) override;
		inline virtual const bool IsLoad() const throw() override;
		virtual void UnLoad() throw() override;

		virtual void DrawOnScreenVideo(ScreenVideo& screen_out, const Point& abs_xy, const Rect& area_clip = Rect(0, 0, -1, -1)) throw(...) override;

		inline void ResizeW(const size_t w_set) throw(...);
		inline void ResizeH(const size_t h_set) throw(...);

		inline void SetAction_onClit(const std::function<void(void)>& action) throw();
	private:
		size_t m_wSize_but = 400;
		size_t m_hSize_but = 50;
		bool m_button_on = false;
		Texture m_texture_button_on_formatted;
		Texture m_texture_button_off_formatted;
		std::function<void(void)> m_action_on_clic;

		static Surface surface_button_on;
		static Surface surface_button_off;

		static const Rect sCutDimensione;

	protected:
		virtual void NotificationEvent(const SDL_Event& event) override;
	};

	inline const bool XButton::IsLoad() const throw(){
		if (m_texture_button_on_formatted.IsVoid() == false && m_texture_button_off_formatted.IsVoid() == false)
			return true;
		return false;
	}

	inline void XButton::ResizeW(const size_t w_set) throw(...){
		this->m_wSize_but = w_set;
		if (this->IsLoad())
			this->Load(*this->m_screen_maker_realative);
	}

	inline void XButton::ResizeH(const size_t h_set) throw(...){
		this->m_hSize_but = h_set;
		if (this->IsLoad())
			this->Load(*this->m_screen_maker_realative);
	}

	inline void XButton::SetAction_onClit(const std::function<void(void)>& action) throw(){
		m_action_on_clic = action;
	}
}

#endif
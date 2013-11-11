#include <XGame/GUI/XButton.hpp>
#include <XGame/GUI/Resource/button_off_res_gif.h>
#include <XGame/GUI/Resource/button_on_res_gif.h>
#include <XGame/Loader/ModuleLoader.hpp>

namespace xgame{
	Surface XButton::surface_button_on;
	Surface XButton::surface_button_off;

	const Rect XButton::sCutDimensione = Rect(0, 0, 6, 6);

	void XButton::Load(const ScreenVideo& screen_ref) throw(...) {
		GraphicComponent::Load(screen_ref);

		if (XButton::surface_button_off.Is_Void()){
			MemoryPage temp_page = ModuleLoader::GetGlobalInstance().MakeMemoryPage_fromMemory(button_off_res_gif, button_off_res_gif_size);
			XButton::surface_button_off.LoadSurface_fromMemoryPage(temp_page);
		}
		if (XButton::surface_button_on.Is_Void()){
			MemoryPage temp_page = ModuleLoader::GetGlobalInstance().MakeMemoryPage_fromMemory(button_on_res_gif, button_on_res_gif_size);
			XButton::surface_button_on.LoadSurface_fromMemoryPage(temp_page);
		}

		m_texture_button_off_formatted = Texture(m_wSize_but, m_hSize_but, screen_ref);
		m_texture_button_on_formatted = Texture(m_wSize_but, m_hSize_but, screen_ref);
		Surface temp_surface_but_on_formatted(m_wSize_but, m_hSize_but);
		Surface temp_surface_but_off_formatted(m_wSize_but, m_hSize_but);


		size_t w_middle = m_wSize_but - (sCutDimensione.Get_Wcomponent() * 2);
		size_t h_middle = m_hSize_but - (sCutDimensione.Get_Hcomponent() * 2);
		Point xy_paste;
		Rect cut_paste;

		for (size_t k = 0; k < h_middle + 2; k++){
			for (size_t i = 0; i < w_middle + 2; i++){
				if (k == 0){
					if (i == 0){
						//SS
						xy_paste.Set_X_Component(0);
						xy_paste.Set_Y_Component(0);
						cut_paste.Set_AllComponent(0, 0, sCutDimensione.Get_Wcomponent(), sCutDimensione.Get_Hcomponent());
					}
					else{
						if (i == w_middle + 1){
							//SD
							xy_paste.Set_X_Component(sCutDimensione.Get_Wcomponent() + (i - 1));
							xy_paste.Set_Y_Component(0);
							cut_paste.Set_AllComponent(sCutDimensione.Get_Wcomponent() + 1, 0, sCutDimensione.Get_Wcomponent(), sCutDimensione.Get_Hcomponent());
						}
						else{
							//SC
							xy_paste.Set_X_Component(sCutDimensione.Get_Wcomponent() + (i - 1));
							xy_paste.Set_Y_Component(0);
							cut_paste.Set_AllComponent(sCutDimensione.Get_Wcomponent(), 0, 1, sCutDimensione.Get_Hcomponent());
						}
					}
				}
				else{
					if (k == h_middle + 1){
						if (i == 0){
							//GS
							xy_paste.Set_X_Component(0);
							xy_paste.Set_Y_Component(sCutDimensione.Get_Hcomponent() + (k - 1));
							cut_paste.Set_AllComponent(0, sCutDimensione.Get_Hcomponent() + 1, sCutDimensione.Get_Wcomponent(), sCutDimensione.Get_Hcomponent());
						}
						else{
							if (i == w_middle + 1){
								//GD
								xy_paste.Set_X_Component(sCutDimensione.Get_Wcomponent() + (i - 1));
								xy_paste.Set_Y_Component(sCutDimensione.Get_Hcomponent() + (k - 1));
								cut_paste.Set_AllComponent(sCutDimensione.Get_Wcomponent() + 1, sCutDimensione.Get_Hcomponent() + 1, sCutDimensione.Get_Wcomponent(), sCutDimensione.Get_Hcomponent());
							}
							else{
								//GC
								xy_paste.Set_X_Component(sCutDimensione.Get_Wcomponent() + (i - 1));
								xy_paste.Set_Y_Component(sCutDimensione.Get_Hcomponent() + (k - 1));
								cut_paste.Set_AllComponent(sCutDimensione.Get_Wcomponent(), sCutDimensione.Get_Hcomponent() + 1, 1, sCutDimensione.Get_Hcomponent());
							}
						}
					}
					else{
						if (i == 0){
							//CS
							xy_paste.Set_X_Component(0);
							xy_paste.Set_Y_Component(sCutDimensione.Get_Hcomponent() + (k - 1));
							cut_paste.Set_AllComponent(0, sCutDimensione.Get_Hcomponent(), sCutDimensione.Get_Wcomponent(), 1);
						}
						else{
							if (i == w_middle + 1){
								//CD
								xy_paste.Set_X_Component(sCutDimensione.Get_Wcomponent() + (i - 1));
								xy_paste.Set_Y_Component(sCutDimensione.Get_Hcomponent() + (k - 1));
								cut_paste.Set_AllComponent(sCutDimensione.Get_Wcomponent() + 1, sCutDimensione.Get_Hcomponent(), sCutDimensione.Get_Wcomponent(), 1);
							}
							else{
								//CC
								xy_paste.Set_X_Component(sCutDimensione.Get_Wcomponent() + (i - 1));
								xy_paste.Set_Y_Component(sCutDimensione.Get_Hcomponent() + (k - 1));
								cut_paste.Set_AllComponent(sCutDimensione.Get_Wcomponent(), sCutDimensione.Get_Hcomponent(), 1, 1);
							}
						}
					}
				}
				temp_surface_but_on_formatted.BlitSurfaceOnThis(XButton::surface_button_on, xy_paste,cut_paste);
				temp_surface_but_off_formatted.BlitSurfaceOnThis(XButton::surface_button_off, xy_paste, cut_paste);
			}
		}
		m_texture_button_on_formatted.LoadTexture_fromSurface(temp_surface_but_on_formatted, screen_ref);
		m_texture_button_off_formatted.LoadTexture_fromSurface(temp_surface_but_off_formatted, screen_ref);
	}

	void XButton::UnLoad() throw(){
		m_texture_button_off_formatted.Clean();
		m_texture_button_on_formatted.Clean();
	}

	void XButton::DrawOnScreenVideo(ScreenVideo& screen_out, const Point& abs_xy, const Rect& area_clip) throw(...){
		GraphicComponent::DrawOnScreenVideo(screen_out, abs_xy,area_clip);

		if (m_button_on)
			screen_out.DrawTexture(m_texture_button_on_formatted, abs_xy, area_clip);
		else
			screen_out.DrawTexture(m_texture_button_off_formatted, abs_xy, area_clip);
	}

	void XButton::NotificationEvent(const SDL_Event& event){
		if (event.type == SDL_MOUSEMOTION){
			Rect region_interactive_button;
			if (Rect::Rects_Intersection(Rect(this->GetXYabs().Get_X_Component(), this->GetXYabs().Get_Y_Component(), m_wSize_but, m_hSize_but),
				this->GetRendererAreaClip(), region_interactive_button))
			{
				if (region_interactive_button.Point_is_In(Point(event.motion.x, event.motion.y))){
					m_button_on = true;
				}
				else{
					m_button_on = false;
				}
			}	
		}
		else{
			if (event.type == SDL_MOUSEBUTTONDOWN){
				if (m_button_on){
					m_button_on = false;
					if (static_cast<const bool>(this->m_action_on_clic) == true)
						this->m_action_on_clic();
				}
			}
		}
	}
}
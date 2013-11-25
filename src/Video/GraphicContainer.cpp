#include <XGame/Video/GraphicContainer.hpp>

namespace xgame{
	void GraphicContainer::DrawnAllInternalComponents_InSizeArea(ScreenVideo& screen_out) const throw(Error){
		for (auto& obj : m_internComponent)
			obj->DrawOnScreenVideo(screen_out,
				this->GetXYabs() + obj->m_xy_relative,
				Rect(this->GetXYabs().Get_X_Component(), this->GetXYabs().Get_Y_Component(), this->m_w_size_container, this->m_h_size_container));
	}

	const bool GraphicContainer::AllInternalComponents_IsLoaded() const throw(){
		for (auto& obj : m_internComponent)
		if (obj->IsLoad() == false)
			return false;
		return true;
	}

	void GraphicContainer::LoadAllInternalComponents(const ScreenVideo& screen_ref){
		for (auto& obj : m_internComponent)
			obj->Load(screen_ref);
	}

	void GraphicContainer::UnLoadAllInternalComponents() throw(){
		for (auto& obj : m_internComponent)
			obj->UnLoad();
	}

	void GraphicContainer::NotificationAllInternalComponents(ListEvents& events){
		for (auto obj = m_internComponent.rbegin(); obj != m_internComponent.rend(); obj++)
			(*obj)->NotificationEvents(events);
	}

	template<class T> std::shared_ptr<T> GraphicContainer::InsertGraphicComponent(const std::shared_ptr<T>& adder_obj)
	{
		m_internComponent.push_back(adder_obj);
		return adder_obj;
	}
}
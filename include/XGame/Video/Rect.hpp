#ifndef __RECT_XGAME__HPP
#define __RECT_XGAME__HPP

#include <XGame/Core/Config.hpp>
#include <SDL/SDL.h>

namespace xgame{
	class XGAME_API Rect{
	public:
		//! Costruttore di default
		inline Rect(int x=0, int y=0, int w=0, int h=0);

		//! Costruttore da SDL_Rect
		inline Rect(const SDL_Rect& oth);

		//! Metodi 'setting' delle componenti.
		inline void Set_Xcomponent(const int x_set) throw();
		inline void Set_Ycomponent(const int y_set) throw();
		inline void Set_Wcomponent(const int w_size_set) throw();
		inline void Set_Hcomponent(const int h_size_set) throw();
		inline void Set_AllComponent(const int x, const int y, const int w, const int h);

		//! Metodi 'getting' delle componenti.
		inline int Get_Xcomponent() const throw();
		inline int Get_Ycomponent() const throw();
		inline int Get_Wcomponent() const throw();
		inline int Get_Hcomponent() const throw();

		//! Operatore di assegnazione con SDL_Rect.
		inline Rect& operator=(const SDL_Rect& oth);

		//! Operatori di casting
		inline operator SDL_Rect&();
		inline operator const SDL_Rect&() const;
		inline operator SDL_Rect*();
		inline operator const SDL_Rect*() const;

		//! @return		'true' se due l'intersezione di due rettangoli in ingresso � non nulla!
		inline static const bool Rects_HasIntersection(const Rect& rect_1, const Rect& rect_2) throw();

		//! @return		Il rettangolo risultante dall'intersezione di due rettangoli.
		inline static const bool Rects_Intersection(const Rect& rect_1, const Rect& rect_2, Rect& rect_out) throw();

	private:
		SDL_Rect m_data;
	};

	inline Rect::Rect(int x, int y, int w, int h){
		m_data.x = x;
		m_data.y = y;
		m_data.w = w;
		m_data.h = h;
	}
	
	inline Rect::Rect(const SDL_Rect& oth){
		m_data = oth;
	}

	inline Rect& Rect::operator=(const SDL_Rect& oth){
		this->m_data=oth;
		return *this;
	}

	

	inline Rect::operator SDL_Rect&(){ return this->m_data; }
	inline Rect::operator const SDL_Rect&() const { return this->m_data; }
	inline Rect::operator SDL_Rect*(){ return &this->m_data;}
	inline Rect::operator const SDL_Rect*() const{return &this->m_data;}

	inline const bool Rect::Rects_HasIntersection(const Rect& rect_1, const Rect& rect_2) throw(){
		if(SDL_HasIntersection(rect_1,rect_2)==SDL_TRUE) return true;
		return false;
	}

	inline const bool Rect::Rects_Intersection(const Rect& rect_1, const Rect& rect_2, Rect& rect_out) throw(){
		if(SDL_IntersectRect(rect_1,rect_2,rect_out)==SDL_TRUE) return true;
		return false;
	}

	inline void Rect::Set_Xcomponent(const int x_set) throw(){ this->m_data.x = x_set; }
	inline void Rect::Set_Ycomponent(const int y_set) throw(){ this->m_data.y = y_set; }
	inline void Rect::Set_Wcomponent(const int w_size_set) throw(){ this->m_data.w = w_size_set; }
	inline void Rect::Set_Hcomponent(const int h_size_set) throw(){ this->m_data.h = h_size_set; }
	inline void Rect::Set_AllComponent(const int x, const int y, const int w, const int h){
		this->Set_Xcomponent(x); this->Set_Ycomponent(y);
		this->Set_Wcomponent(w); this->Set_Hcomponent(h);
	}

	inline int Rect::Get_Xcomponent() const throw(){ return this->m_data.x; }
	inline int Rect::Get_Ycomponent() const throw(){ return this->m_data.y; }
	inline int Rect::Get_Wcomponent() const throw(){ return this->m_data.w; }
	inline int Rect::Get_Hcomponent() const throw(){ return this->m_data.h; }
}

#endif
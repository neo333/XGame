#ifndef __COLOR_XGAME__HPP
#define __COLOR_XGAME__HPP

#include <XGame/Core/Config.hpp>
#include <SDL/SDL.h>

namespace xgame{
	class Color{
	public:
		//Costruttore di default.
		inline Color(const Uint8 red =0, const Uint8 green =0, const Uint8 blue =0, const Uint8 alpha =255);

		//! Metodi 'getting' per le componenti.
		inline Uint8 Get_RedComponent() const throw();
		inline Uint8 Get_GreenComponent() const throw();
		inline Uint8 Get_BlueComponent() const throw();
		inline Uint8 Get_AlphaComponent() const throw();

		//! Metodi 'setting' per le componenti.
		inline void Set_RedComponent(const Uint8 red_set) throw();
		inline void Set_GreenComponent(const Uint8 green_set) throw();
		inline void Set_BlueComponent(const Uint8 blue_set) throw();
		inline void Set_AlphaComponent(const Uint8 alpha_set) throw();

		//! Operatori di casting per SDL_Color.
		inline operator SDL_Color&() throw();
		inline operator const SDL_Color&() const throw();
		inline operator SDL_Color*() throw();
		inline operator const SDL_Color*() const throw();


	private:
		SDL_Color m_dataColor;
	};

	inline Color::Color(const Uint8 red, const Uint8 green, const Uint8 blue, const Uint8 alpha){
		m_dataColor.r=red;
		m_dataColor.g=green;
		m_dataColor.b=blue;
		m_dataColor.a=alpha;
	}

	inline Uint8 Color::Get_RedComponent() const throw(){ return m_dataColor.r; }
	inline Uint8 Color::Get_GreenComponent() const throw(){ return m_dataColor.g; }
	inline Uint8 Color::Get_BlueComponent() const throw(){ return m_dataColor.b; }
	inline Uint8 Color::Get_AlphaComponent() const throw(){ return m_dataColor.a; }

	inline Color::operator SDL_Color&() throw(){ return m_dataColor; }
	inline Color::operator const SDL_Color&() const throw(){ return m_dataColor; }
	inline Color::operator SDL_Color*() throw(){ return &m_dataColor; }
	inline Color::operator const SDL_Color*() const throw(){ return &m_dataColor; }

	inline void Color::Set_RedComponent(const Uint8 red_set) throw(){ m_dataColor.r = red_set; }
	inline void Color::Set_GreenComponent(const Uint8 green_set) throw(){ m_dataColor.g = green_set; }
	inline void Color::Set_BlueComponent(const Uint8 blue_set) throw(){ m_dataColor.b = blue_set; }
	inline void Color::Set_AlphaComponent(const Uint8 alpha_set) throw(){ m_dataColor.a = alpha_set; }
}

#endif
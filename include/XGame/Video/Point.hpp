#ifndef __POINT_XGAME__HPP
#define __POINT_XGAME__HPP

#include <XGame/Core/Config.hpp>
#include <cmath>
#include <SDL/SDL.h>

namespace xgame{
	class XGAME_API Point{
	public:
		//! Costruttore di default
		inline Point(const int x =0, const int y =0);
		
		//! @return		La componente X del punto.
		inline int Get_X_Component() const;

		//! @return		La componente Y del punto.
		inline int Get_Y_Component() const;

		//! @param [in] x	Setta la componente X del punto.
		inline void Set_X_Component(const int x);

		//! @param [in] y	Setta la componente Y del punto.
		inline void Set_Y_Component(const int y);

		//! @return		La somma di due punti
		inline Point operator+(const Point& oth) const;

		//! Aggiunge un punto a questo punto
		inline Point& operator+=(const Point& oth);

		//! @return		La differenze di due punti
		inline Point operator-(const Point& oth) const;

		//! Sottrae un punto a questo punto
		inline Point& operator-=(const Point& oth);

		//! @return		Il modulo del punto
		inline double Get_Module() const;

		//! Casting in SDL_point
		inline operator SDL_Point&();
		inline operator const SDL_Point&() const;
		inline operator SDL_Point*();
		inline operator const SDL_Point*() const;

	private:
		SDL_Point m_data;
	};


	inline Point::Point(const int x, const int y){
		m_data.x=x;
		m_data.y=y;
	}

	inline int Point::Get_X_Component() const{ return this->m_data.x; }

	inline int Point::Get_Y_Component() const{ return this->m_data.y; }

	inline void Point::Set_X_Component(const int x){ this->m_data.x = x; }

	inline void Point::Set_Y_Component(const int y){ this->m_data.y = y; }

	inline Point Point::operator+(const Point& oth) const{ return Point(this->m_data.x + oth.m_data.x, this->m_data.y + oth.m_data.y); }

	inline Point& Point::operator+=(const Point& oth){
		this->m_data.x+=oth.m_data.x;
		this->m_data.y+=oth.m_data.y;
		return *this;
	}

	inline Point Point::operator-(const Point& oth) const{ return Point(this->m_data.x - oth.m_data.x, this->m_data.y - oth.m_data.y); }

	inline Point& Point::operator-=(const Point& oth){
		this->m_data.x-=oth.m_data.x;
		this->m_data.y-=oth.m_data.y;
		return *this;
	}

	inline double Point::Get_Module() const{ return sqrt(static_cast<double>(m_data.x*m_data.x) + static_cast<double>(m_data.y*m_data.y)); }

	inline Point::operator SDL_Point&(){
		return m_data;
	}

	inline Point::operator const SDL_Point&() const{
		return m_data;
	}

	inline Point::operator SDL_Point*(){ return &this->m_data; }
	inline Point::operator const SDL_Point*() const{ return &this->m_data; }
}

#endif
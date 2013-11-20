#ifndef __POINT_XGAME__HPP
#define __POINT_XGAME__HPP

#include <XGame/Video/Config.hpp>
#include <cmath>
#include <SDL2/SDL.h>

namespace xgame{
	class XGAME_API_VIDEO Point{
	public:
		//! Costruttore di default
		inline Point(const int x =0, const int y =0) throw();
		
		//! \return		La componente X del punto.
		inline int Get_X_Component() const throw();

		//! \return		La componente Y del punto.
		inline int Get_Y_Component() const throw();

		//! \param [in] x	Setta la componente X del punto.
		inline void Set_X_Component(const int x) throw();

		//! \param [in] y	Setta la componente Y del punto.
		inline void Set_Y_Component(const int y) throw();

		//! \return		La somma di due punti
		inline Point operator+(const Point& oth) const throw();

		//! Aggiunge un punto a questo punto
		inline Point& operator+=(const Point& oth) throw();

		//! \return		La differenze di due punti
		inline Point operator-(const Point& oth) const throw();

		//! Sottrae un punto a questo punto
		inline Point& operator-=(const Point& oth) throw();

		//! \return		Il modulo del punto
		inline double Get_Module() const throw();

		//! Casting in SDL_point
		inline operator SDL_Point&();
		inline operator const SDL_Point&() const;
		inline operator SDL_Point*();
		inline operator const SDL_Point*() const;

	private:
		SDL_Point m_data;
	};


	inline Point::Point(const int x, const int y) throw(){
		m_data.x=x;
		m_data.y=y;
	}

	inline int Point::Get_X_Component() const throw(){ return this->m_data.x; }

	inline int Point::Get_Y_Component() const throw(){ return this->m_data.y; }

	inline void Point::Set_X_Component(const int x)throw(){ this->m_data.x = x; }

	inline void Point::Set_Y_Component(const int y)throw(){ this->m_data.y = y; }

	inline Point Point::operator+(const Point& oth) const throw(){ return Point(this->m_data.x + oth.m_data.x, this->m_data.y + oth.m_data.y); }

	inline Point& Point::operator+=(const Point& oth) throw(){
		this->m_data.x+=oth.m_data.x;
		this->m_data.y+=oth.m_data.y;
		return *this;
	}

	inline Point Point::operator-(const Point& oth) const throw(){ return Point(this->m_data.x - oth.m_data.x, this->m_data.y - oth.m_data.y); }

	inline Point& Point::operator-=(const Point& oth) throw(){
		this->m_data.x-=oth.m_data.x;
		this->m_data.y-=oth.m_data.y;
		return *this;
	}

	inline double Point::Get_Module() const throw(){ return sqrt(static_cast<double>(m_data.x*m_data.x) + static_cast<double>(m_data.y*m_data.y)); }

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
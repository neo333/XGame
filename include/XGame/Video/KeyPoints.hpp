#ifndef __KEY_POINTS__XGAME__HPP
#define __KEY_POINTS__XGAME__HPP

#include <XGame/Video/Config.hpp>
#include <XGame/Video/Point.hpp>
#include <vector>

#ifdef XGAME_MSVC 
#pragma warning(disable:4251)
#endif

namespace xgame{
	class XGAME_API_VIDEO KeyPoints
	{
	public:
		//! Costruttore di default.
		KeyPoints() = default;

		//! Distruttore.
		~KeyPoints() = default;

		//! Costruttore di copia.
		KeyPoints(const KeyPoints&) = default;

		//! Operatore di assegnazione.
		KeyPoints& operator=(const KeyPoints&) = default;

		//! Costruttore di move.
		inline KeyPoints(KeyPoints&&) throw();

		//! Operatore di move.
		inline KeyPoints& operator=(KeyPoints&&) throw();

		//! Operatore di casting esplicito a vettore di punti!
		inline explicit operator const std::vector<Point>&() const throw();
	private:
		friend class Surface;
		std::vector<Point> m_key;
	};

	inline KeyPoints::KeyPoints(KeyPoints&& oth) throw():
		m_key(std::move(oth.m_key))
	{

	}
	inline KeyPoints& KeyPoints::operator = (KeyPoints&& oth) throw()
	{
		if (this != &oth){
			this->m_key = std::move(oth.m_key);
		}
		return *this;
	}

	inline KeyPoints::operator const std::vector<Point>&() const throw()
	{
		return m_key;
	}
}

#endif
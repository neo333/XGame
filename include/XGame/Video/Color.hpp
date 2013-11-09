#ifndef __COLOR_XGAME__HPP
#define __COLOR_XGAME__HPP

#include <XGame/Video/Config.hpp>
#include <SDL/SDL.h>
#include <boost/lexical_cast.hpp>

namespace xgame{
	class XGAME_API_VIDEO Color{
	public:
		//! Costruttore di default.
		inline Color(const Uint8 red, const Uint8 green, const Uint8 blue, const Uint8 alpha =255);

		//! Costruttore esplicito con notazione compatta.
		inline explicit Color(const Uint32 rgba) throw();

		//! Operatore di conversione escplicita a notazione compatta.
		inline explicit operator Uint32() const throw();

		//! \return La componente RED.
		inline Uint8 Get_RedComponent() const throw();

		//! \return La componente GREEN.
		inline Uint8 Get_GreenComponent() const throw();

		//! \return La componente BLUE.
		inline Uint8 Get_BlueComponent() const throw();

		//! \return La componente ALPHA.
		inline Uint8 Get_AlphaComponent() const throw();

		//! \return La componente ALPHA NORMALIZZATA. (Un valore percentile compreso tra 0 e 1).
		inline float Get_AlphaComponentNormalized() const throw();

		//! Setta la componente RED.
		inline void Set_RedComponent(const Uint8 red_set) throw();

		//! Setta la componente GREEN.
		inline void Set_GreenComponent(const Uint8 green_set) throw();

		//! Setta la componente BLUE.
		inline void Set_BlueComponent(const Uint8 blue_set) throw();

		//! Setta la componente ALPHA.
		inline void Set_AlphaComponent(const Uint8 alpha_set) throw();

		/*! Setta la componente alpha secondo un valore NORMALIZZATO.
			\param [in] alpha_set_norm			Un valore di alpha (percentile).
												Compreso tra 0 e 1.
			\throw Error						In caso il valore di input non sia compreso nell'intervallo [0,1].
		*/
		inline void Set_AlphaComponentNormalized(const float alpha_set_norm) throw(...);

		//! Converte l'oggetto Color in una struttura SDL_Color.
		inline operator SDL_Color&() throw();

		//! \see Color::operator SDL_Color&()
		inline operator const SDL_Color&() const throw();

		//! Converte l'oggetto Color in un puntatore ad una struttura SDL_Color.
		inline operator SDL_Color*() throw();

		//! \see Color::operator SDL_Color*()
		inline operator const SDL_Color*() const throw();

		/*! Sottrazione di due colori. Corrisponde alla pura sottrazione delle componente tra i due.
			\note	Non viene eseguito alcun controllo sulle grandezze in questione: 
						Esempio:
							Se ho colorRed1 = 10 e colorRed2 = 20
							Allora il risultato è colorRed_tot = 10-20 = -10 ----> Uint8(-10) = 255-10;
		*/
		inline Color operator-(const Color oth) const throw();

		//! \see Color::operator-(const Color oth);
		inline void operator-=(const Color oth) throw();




	private:
		SDL_Color m_dataColor;
	};

	inline Color::Color(const Uint8 red, const Uint8 green, const Uint8 blue, const Uint8 alpha){
		m_dataColor.r=red;
		m_dataColor.g=green;
		m_dataColor.b=blue;
		m_dataColor.a=alpha;
	}

	inline Color::Color(const Uint32 rgba) throw(){
		Uint32 temp = rgba & 0xff000000;
		m_dataColor.r = temp >> 24;

		temp = rgba & 0x00ff0000;
		m_dataColor.g = temp >> 16;

		temp = rgba & 0x0000ff00;
		m_dataColor.b = rgba >> 8;

		m_dataColor.a = rgba & 0x000000ff;
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

	inline Color::operator Uint32() const throw(){
		Uint32 rts = m_dataColor.r;
		rts<<= 8;
		rts |= m_dataColor.g;
		rts<<= 8;
		rts |= m_dataColor.b;
		rts<<= 8;
		rts |= m_dataColor.a;
		return rts;
	}

	inline float Color::Get_AlphaComponentNormalized() const throw(){
		return static_cast<float>(m_dataColor.a) / 255.f;
	}

	inline void Color::Set_AlphaComponentNormalized(const float alpha_set_norm) throw(...){
		if (alpha_set_norm < 0 && alpha_set_norm > 1)
			throw Error("Color", "Set_AlphaComponentNormalized", "Alpha setting non normalizzato: '%s'", 
															std::to_string(alpha_set_norm).c_str());
		m_dataColor.a = static_cast <Uint8>(255.f * alpha_set_norm);
	}

	inline Color Color::operator-(const Color oth) const throw(){
		return Color(this->m_dataColor.r - oth.m_dataColor.r,
			this->m_dataColor.g - oth.m_dataColor.g,
			this->m_dataColor.b - oth.m_dataColor.b,
			this->m_dataColor.a - oth.m_dataColor.a);
	}

	inline void Color::operator-=(const Color oth) throw(){
		this->m_dataColor.r -= oth.m_dataColor.r;
		this->m_dataColor.g -= oth.m_dataColor.g;
		this->m_dataColor.b -= oth.m_dataColor.b;
		this->m_dataColor.a -= oth.m_dataColor.a;
	}
}

#endif
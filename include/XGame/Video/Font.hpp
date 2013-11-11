#ifndef __FONT_XGAME__HPP
#define __FONT_XGAME__HPP

#include <XGame/Video/Config.hpp>
#include <XGame/Loader/MemoryPage.hpp>
#include <XGame/Core/Error.hpp>
#include <SDL/SDL_ttf.h>

namespace xgame{
	class XGAME_API_VIDEO Font{
	public:
		//! Costruttore di default.
		Font() = default;

		//!	DELETE Costruttore di copia. Questo oggetto non può essere copiato.
		Font(const Font& oth) = delete;

		//!	DELETE Operatore di assegnazione. Questo oggetto non può essere copiato.
		Font& operator=(const Font& oth) = delete;

		//!	Costruttore di move.
		Font(Font&& oth) throw();

		//! Operatore di move.
		Font& operator=(Font&& oth) throw();

		//!	Distruttore.
		~Font();

		//!	Pulisce il font. Dealloca tutte le risorse grafiche associate. Il font diventa vuoto!
		void Clean() throw();

		//!	\return		'true' se il font è vuoto (non caricato).
		inline const bool IsVoid() const throw();

		//!	Operatore di casting a puntatore TTF_Font.
		inline explicit operator TTF_Font*() throw();

		//!	Operatore di casting a puntatore const TTF_Font.
		inline explicit operator const TTF_Font*() const throw();

		/*!	Carica un font tramite una pagina di memoria.
			La pagina di memoria deve contenere interamente un tipo di dato TTF o FON file.

			\param [in] input_page		La pagina di memoria contenente il dato TTF o FON.
			\param [in] ptsize_font		Le dimensioni del font da caricare (basate su 72DPI).

			\throw	Error				In caso di inleggibilità del dato.
		*/
		void LoadFont_fromMemoryPage(const MemoryPage& input_page, const size_t ptsize_font) throw(...);
	
	
	private:
		TTF_Font* m_font = nullptr;
		size_t m_size_font = 0;

		friend class TextSurface;
	};

	inline const bool Font::IsVoid() const throw(){
		if (m_font == nullptr) return true;
		return false;
	}

	inline Font::operator TTF_Font*() throw(){
		return m_font;
	}
	inline Font::operator const TTF_Font*() const throw(){
		return m_font;
	}
}

#endif
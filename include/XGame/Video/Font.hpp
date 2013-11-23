#ifndef __FONT_XGAME__HPP
#define __FONT_XGAME__HPP

#include <XGame/Video/Config.hpp>
#include <XGame/Loader/MemoryPage.hpp>
#include <XGame/Core/Error.hpp>
#include <XGame/Video/Rect.hpp>
#include <SDL2/SDL_ttf.h>

#ifdef XGAME_WINDOWS_OS
#pragma warning(disable:4290)
#endif

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
		virtual ~Font() throw();

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
		void LoadFont_fromMemoryPage(const MemoryPage& input_page, const size_t ptsize_font) throw(Error);

		/*!	Carica un font tramite una pagina di memoria spostandone il contenuto.
			Questo metodo è un caricamento velocizzato poiché non effettua una copia della pagina di memoria
			di ingresso ma uno spostamento interno.

			\param [in] input_page		La pagina di memoria contenente il dato TTF o FON.
										Dopo la chiamata di questo metodo la pagina di memoria in ingresso 
										sarà vuota poiché tutto il suo contenuto verrà spostanto nei dati interni
										di della classe.
			\param [in] ptsize_font		Le dimensioni del font da caricare (basate su 72DPI).

			\throw	Error				In caso di inleggibilità del dato.
		*/
		void LoadFont_fromMemoryPage(MemoryPage&& input_page, const size_t ptsize_font) throw(Error);
	

		//! Wrapper degli stili dei font.
		enum STYLE_FONT{
			STYLE_NORMAL = TTF_STYLE_NORMAL,
			STYLE_BOLD = TTF_STYLE_BOLD,
			STYLE_ITALIC = TTF_STYLE_ITALIC,
			STYLE_UNDERLINE = TTF_STYLE_UNDERLINE
		};

		/*!	Setta lo stile del font.
			
			\param [in] style_set		Lo stile da settare. Non sono esclusivi!
										(E' possibile effettuare una concatenazione di più stili).
			
			\see Font::STYLE_FONT
			
			\note						Se il font viene ricaricato gli stili vengono resettati (verrà reimpostato lo stile di default).

			\throw	Error				In caso di errore grafico interno.
		*/
		void SetStyle(const STYLE_FONT style_set) throw(Error);

		//!	\return Gli stili applicati correntemente al font.
		inline const STYLE_FONT GetStyle() const throw();


		/*!	Calcola la dimensione occupata da un'eventuale surface ottenuta renderizzando la stringa di ingresso.
			Questa funzione non effettua nessun rendering o modifca sul font, ma può essere utilizzata per effettuare
			un pre-calcolo sulle dimensioni di un eventuale rendering di testo.

			\param [in] str_input		La stringa che verrà eventualmente renderizzata.
			
			\return Rect				Sulle componenti W e H la dimensione di un'eventuale surface renderizzata dalla stringa.

			\throw Error				In caso di errore interno.
		*/
		const Rect CalculateSizeText_withThisFont(const std::string& str_input) const;

		//!	\return		La dimensione (in pixel) che dovrebbe intercorrere tra una riga e l'altra per il font.
		inline const int Get_H_LineSkyp() const throw();

	private:
		TTF_Font* m_font = nullptr;
		MemoryPage m_font_data;
		size_t m_size_font = 0;
		int m_style_font = TTF_STYLE_NORMAL;
		int m_h_lineskyp = 0;

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
	inline auto Font::GetStyle() const throw() -> const STYLE_FONT{
		return static_cast<STYLE_FONT>(m_style_font);
	}

	inline const int Font::Get_H_LineSkyp() const throw(){
		return m_h_lineskyp;
	}
}

#endif
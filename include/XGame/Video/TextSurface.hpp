#ifndef __TEXT_SURFACE_XGAME__HPP
#define __TEXT_SURFACE_XGAME__HPP

#include <XGame/Video/Config.hpp>
#include <XGame/Video/Surface.hpp>
#include <XGame/Video/Font.hpp>

#ifdef XGAME_MSVC 
#pragma warning(disable:4251)
#endif

namespace xgame{
	class XGAME_API_VIDEO TextSurface: public Surface{
	public:

		//! Pulisce la surface deallocando le risorse.
		virtual void Clean() throw() override;

		//!	Enumerazione per la qualità del render del testo.
		enum class QUALITY_RENDER_FONT{
			LOW_QUALITY, HIGH_QUALITY
		};

		//! Enumerazione per la codifica di input per il render del testo.
		enum class CODEC_RENDER_FONT{
			INPUT_LATIN1, INPUT_UTF8
		};

		/*! Carica la surface renderizzando un testo tramite uno specifico font.
			
			\param [in] input_font			Il font da usare per il rendering del testo.
			\param [in] str_input			La stringa di input da renderizzare.
			\param [in] color_text			Il colore con quale si vuole renderizzare il testo.
			\param [in] quality				La qualità dell'operazione di rendering.
			\param [in] codec				La codifica della stringa di ingresso.
			
			\see TextSurface::QUALITY_RENDER_FONT
			\see TextSurface::CODEC_RENDER_FONT
			
			\note							A bassa qualità il caricamento è un pò più veloce, ma le scritte non supportano
											le sfumature.
			\note							Questo metodo supporta il carattere '\n'. Creando una Surface con più linee.
		*/
		void LoadSurface_fromFont(const Font& input_font, const std::string& str_input, const Color& color_text,
			const QUALITY_RENDER_FONT quality = QUALITY_RENDER_FONT::HIGH_QUALITY,
			const CODEC_RENDER_FONT codec = CODEC_RENDER_FONT::INPUT_LATIN1);

		//!	\return		La stringa utilizzata per il render. Una stringa vuota in caso la surface sia vuota.
		inline const std::string& Get_String() const throw();

		//! Costruttore di default.
		TextSurface() = default;

		//! Costruttore di copia di default.
		TextSurface(const TextSurface& oth) = default;

		//! Operatore di assegnazione di default.
		TextSurface& operator=(const TextSurface& oth) = default;

		//! Costruttore di move.
		inline TextSurface(TextSurface&& oth) throw();

		//! Operatore di move.
		inline TextSurface& operator=(TextSurface&& oth) throw();


	private:
		std::string m_this_text;
	};

	inline TextSurface& TextSurface::operator=(TextSurface&& oth) throw(){
		if (this != &oth){
			Surface::operator=(std::move(oth));
			m_this_text = std::move(oth.m_this_text);
		}
		return *this;
	}

	inline TextSurface::TextSurface(TextSurface&& oth) throw():Surface(std::move(oth)),m_this_text(std::move(oth.m_this_text)){ }

	inline const std::string& TextSurface::Get_String() const throw(){ return this->m_this_text; }
}

#endif
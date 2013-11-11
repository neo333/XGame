#ifndef __TEXT_SURFACE_XGAME__HPP
#define __TEXT_SURFACE_XGAME__HPP

#include <XGame/Video/Config.hpp>
#include <XGame/Video/Surface.hpp>
#include <XGame/Video/Font.hpp>

namespace xgame{
	class XGAME_API_VIDEO TextSurface: public Surface{
	public:

		//!	Enumerazione per la qualità del render del testo.
		enum class QUALITY_RENDER_FONT{
			LOW_QUALITY, MEDIUM_QUALITY, HIGH_QUALITY
		};

		//! Enumerazione per la codifica di input per il render del testo.
		enum class CODEC_RENDER_FONT{
			INPUT_LATIN1, INPUT_UTF8, INPUT_UNICODE
		};

		/*!
		*/
		void LoadSurface_fromFont(const Font& input_font, const std::string& str_input, const Color& color_text, const Color& color_background =Color(0,0,0),
			const QUALITY_RENDER_FONT quality = QUALITY_RENDER_FONT::LOW_QUALITY,
			const CODEC_RENDER_FONT codec = CODEC_RENDER_FONT::INPUT_LATIN1) throw(...);
	};
}

#endif
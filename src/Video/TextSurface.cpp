#include <XGame/Video/TextSurface.hpp>
#include <codecvt>

namespace xgame{
	void TextSurface::LoadSurface_fromFont(const Font& input_font, const std::string& str_input, const Color& color_text, const Color& color_background,
		const QUALITY_RENDER_FONT quality, const CODEC_RENDER_FONT codec) throw(...)
	{
		this->Clean();
		if (input_font.IsVoid()) return;
		if (str_input.size() == 0) return;
		std::function < SDL_Surface*(TTF_Font*, const char*, SDL_Color) > render_process;
		std::function < SDL_Surface*(TTF_Font*, const char*, SDL_Color,SDL_Color) > render_process_shaded;
		std::function < SDL_Surface*(TTF_Font*, const Uint16*, SDL_Color) > render_process_unicode;
		std::function < SDL_Surface*(TTF_Font*, const Uint16*, SDL_Color,SDL_Color) > render_process_unicode_shaded;

		if (quality == QUALITY_RENDER_FONT::LOW_QUALITY){
			if (codec == CODEC_RENDER_FONT::INPUT_LATIN1)
				render_process = TTF_RenderText_Solid;
			else if (codec == CODEC_RENDER_FONT::INPUT_UNICODE)
				render_process_unicode = TTF_RenderUNICODE_Solid;
			else if (codec == CODEC_RENDER_FONT::INPUT_UTF8)
				render_process = TTF_RenderUTF8_Solid;
		}
		else{
			if (quality == QUALITY_RENDER_FONT::MEDIUM_QUALITY){
				if (codec == CODEC_RENDER_FONT::INPUT_LATIN1)
					render_process_shaded = TTF_RenderText_Shaded;
				else if (codec == CODEC_RENDER_FONT::INPUT_UNICODE)
					render_process_unicode_shaded = TTF_RenderUNICODE_Shaded;
				else if (codec == CODEC_RENDER_FONT::INPUT_UTF8)
					render_process_shaded = TTF_RenderUTF8_Shaded;
			}
			else{
				if (quality == QUALITY_RENDER_FONT::HIGH_QUALITY){
					if (codec == CODEC_RENDER_FONT::INPUT_LATIN1)
						render_process = TTF_RenderText_Blended;
					else if (codec == CODEC_RENDER_FONT::INPUT_UNICODE)
						render_process_unicode = TTF_RenderUNICODE_Blended;
					else if (codec == CODEC_RENDER_FONT::INPUT_UTF8)
						render_process = TTF_RenderUTF8_Blended;
				}
			}
		}

		SDL_Surface* data_surface_text = nullptr;
		if (render_process){
			data_surface_text = render_process(input_font.m_font, str_input.c_str(), color_text);
		}
		else if (render_process_shaded){
			data_surface_text = render_process_shaded(input_font.m_font, str_input.c_str(), color_text, color_background);
		}
		else if (render_process_unicode){
			throw Error("TextSurface", "LoadSurface_fromFont", "TODO: Metodo ancora non in progettazione!");
		}
		else if (render_process_unicode_shaded){
			throw Error("TextSurface", "LoadSurface_fromFont", "TODO: Metodo ancora non in progettazione!");
		}

		if (data_surface_text == nullptr)
			throw Error("TextSurface", "LoadSurface_fromFont", "Impossibile eseguire un render del testo!\n%s",TTF_GetError());

		m_surface = SDL_ConvertSurfaceFormat(data_surface_text, SDL_PIXELFORMAT_RGBA8888, 0);
		SDL_FreeSurface(data_surface_text);
		data_surface_text = nullptr;
		if (m_surface==nullptr)
			throw Error("TextSurface", "LoadSurface_fromFont", "Impossibile ottimizzare la surface!\n%s", TTF_GetError());

		SDL_SetSurfaceRLE(m_surface, 1);
		SDL_SetSurfaceBlendMode(m_surface, SDL_BLENDMODE_BLEND);

		m_this_text = str_input;
	}

	void TextSurface::Clean() throw(){
		Surface::Clean();
		m_this_text.clear();
	}
}
#include <XGame/Video/TextSurface.hpp>
#include <functional>

namespace xgame{
	void TextSurface::LoadSurface_fromFont(const Font& input_font, const std::string& str_input, const Color& color_text,
		const QUALITY_RENDER_FONT quality, const CODEC_RENDER_FONT codec)
	{
		this->Clean();
		if (input_font.IsVoid()) return;
		if (str_input.size() == 0) return;
		std::function < SDL_Surface*(TTF_Font*, const char*, SDL_Color) > render_process;

		if (quality == QUALITY_RENDER_FONT::LOW_QUALITY){
			if (codec == CODEC_RENDER_FONT::INPUT_LATIN1)
				render_process = TTF_RenderText_Solid;
			else if (codec == CODEC_RENDER_FONT::INPUT_UTF8)
				render_process = TTF_RenderUTF8_Solid;
		}
		else{
			if (quality == QUALITY_RENDER_FONT::HIGH_QUALITY){
				if (codec == CODEC_RENDER_FONT::INPUT_LATIN1)
					render_process = TTF_RenderText_Blended;
				else if (codec == CODEC_RENDER_FONT::INPUT_UTF8)
					render_process = TTF_RenderUTF8_Blended;
			}
		}

		std::vector<size_t> offset_lines;
		offset_lines.push_back(0);
		std::vector<size_t> len_lines;
		size_t finder;
		size_t last_offset = 0;
		while ((finder = str_input.find('\n', last_offset)) != std::string::npos){
			len_lines.push_back(finder - offset_lines[offset_lines.size() - 1]);
			offset_lines.push_back(finder+1);
			last_offset = finder + 1;
		}
		len_lines.push_back(str_input.size() - offset_lines[offset_lines.size() - 1]);

		const size_t& N_LINES = offset_lines.size();
		std::vector<Surface> opt_lines_surface;
		opt_lines_surface.resize(N_LINES);

		SDL_Surface* data_surface_text = nullptr;
		if (!render_process)
			throw Error("TextSurface", "LoadSurface_fromFont", "Funzione di render text non specificata!");

		std::string str_line;
		for (size_t i = 0; i < N_LINES; i++){
			str_line = std::move(str_input.substr(offset_lines[i], len_lines[i]));
			if (str_line.size() == 0)
				str_line = " ";
			data_surface_text = render_process(input_font.m_font, str_line.c_str(), color_text);
				
			if (data_surface_text == nullptr)
				throw Error("TextSurface", "LoadSurface_fromFont", "Impossibile eseguire un render del testo!\n%s", TTF_GetError());

			opt_lines_surface[i] = data_surface_text;
			SDL_FreeSurface(data_surface_text);
			data_surface_text = nullptr;
			if (opt_lines_surface[i].Is_Void())
				throw Error("TextSurface", "LoadSurface_fromFont", "Impossibile ottimizzare la surface!\n%s", TTF_GetError());
		}

		size_t w_max=0;
		for (auto& surf_lines : opt_lines_surface){
			if (w_max < surf_lines.Get_W())
				w_max = surf_lines.Get_W();
		}
		
		(Surface&)(*this) = Surface(w_max, input_font.Get_H_LineSkyp()*N_LINES);

		for (size_t i = 0; i < N_LINES; i++){
			this->BlitSurfaceOnThis(opt_lines_surface[i], Point(0, input_font.Get_H_LineSkyp()*i));
		}

		m_this_text = str_input;
	}

	

	void TextSurface::Clean() throw(){
		Surface::Clean();
		m_this_text.clear();
	}
}
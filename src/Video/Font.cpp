#include <XGame/Video/Font.hpp>

namespace xgame{
	Font::~Font() throw(){
		this->Clean();
	}

	void Font::Clean() throw(){
		if (m_font != nullptr){
			TTF_CloseFont(m_font);
			m_font = nullptr;
			m_size_font = 0;
			m_style_font = TTF_STYLE_NORMAL;
			m_h_lineskyp = 0;
		}
	}

	Font::Font(Font&& oth) throw():m_font(oth.m_font),m_size_font(oth.m_size_font),m_style_font(oth.m_style_font),
									m_h_lineskyp(oth.m_h_lineskyp)
	{
		oth.m_font = nullptr;
		oth.m_size_font = 0;
		oth.m_style_font = TTF_STYLE_NORMAL;
		oth.m_h_lineskyp = 0;
	}

	Font& Font::operator=(Font&& oth) throw(){
		if (this != &oth){
			this->Clean();
			this->m_font = oth.m_font;
			this->m_size_font = oth.m_size_font;
			this->m_style_font = oth.m_style_font;
			this->m_h_lineskyp = oth.m_h_lineskyp;
			oth.m_font = nullptr;
			oth.m_size_font = 0;
			oth.m_style_font = TTF_STYLE_NORMAL;
			oth.m_h_lineskyp = 0;
		}
		return *this;
	}

	void Font::LoadFont_fromMemoryPage(const MemoryPage& input_page, const size_t ptsize_font) throw(Error){
		this->Clean();
		if (input_page.GetSize() == 0) return;
		SDL_RWops* data_access = SDL_RWFromConstMem(input_page.Get_PtrMemory(), input_page.GetSize());
		if (data_access == nullptr)
			throw Error("Font", "LoadFont_fromMemoryPage", "Pagina di memoria corrotta!\n%s", SDL_GetError());
		m_font = TTF_OpenFontRW(data_access, 1, static_cast<int>(ptsize_font));
		if (m_font==nullptr)
			throw Error("Font", "LoadFont_fromMemoryPage", "Impossibile caricare il font richiesto!\n%s", SDL_GetError());
		m_size_font = ptsize_font;
		m_h_lineskyp = TTF_FontLineSkip(m_font);
	}

	void Font::SetStyle(const STYLE_FONT style_set) throw(Error){
		if (this->IsVoid() || style_set==m_style_font) return;
		try{
			TTF_SetFontStyle(m_font, style_set);
			m_style_font = style_set;
		}
		catch (const std::exception& err){
			throw Error("Font", "SetStyle", err.what());
		}
	}

	const Rect Font::CalculateSizeText_withThisFont(const std::string& str_input) const{
		if (m_font == nullptr) return Rect(0,0,0,0);
		int _w, _h;
		if (TTF_SizeText(m_font, str_input.c_str(), &_w, &_h) != 0)
			throw Error("Font", "CalculateSizeText_withThisFont", "Impossibile determinare la dimensione renderizzata del testo!\n%s", TTF_GetError());
		return Rect(0, 0, _w, _h);
	}
}
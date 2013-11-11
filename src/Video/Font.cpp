#include <XGame/Video/Font.hpp>

namespace xgame{
	Font::~Font(){
		this->Clean();
	}

	void Font::Clean() throw(){
		if (m_font != nullptr){
			TTF_CloseFont(m_font);
			m_font = nullptr;
			m_size_font = 0;
		}
	}

	Font::Font(Font&& oth) throw():m_font(oth.m_font),m_size_font(oth.m_size_font){
		oth.m_font = nullptr;
		oth.m_size_font = 0;
	}

	Font& Font::operator=(Font&& oth) throw(){
		if (this != &oth){
			this->Clean();
			this->m_font = oth.m_font;
			this->m_size_font = oth.m_size_font;
			oth.m_font = nullptr;
			oth.m_size_font = 0;
		}
		return *this;
	}

	void Font::LoadFont_fromMemoryPage(const MemoryPage& input_page, const size_t ptsize_font) throw(...){
		this->Clean();
		if (input_page.GetSize() == 0) return;
		SDL_RWops* data_access = SDL_RWFromConstMem(input_page.Get_PtrMemory(), input_page.GetSize());
		if (data_access == nullptr)
			throw Error("Font", "LoadFont_fromMemoryPage", "Pagina di memoria corrotta!\n%s", SDL_GetError());
		m_font = TTF_OpenFontRW(data_access, 1, static_cast<int>(ptsize_font));
		if (m_font==nullptr)
			throw Error("Font", "LoadFont_fromMemoryPage", "Impossibile caricare il font richiesto!\n%s", SDL_GetError());
		m_size_font = ptsize_font;
	}
}
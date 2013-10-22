#include <XGame/Loader/PackMemoryPage.hpp>

namespace xgame{
	PackMemoryPage::PackMemoryPage(){ }

	bool PackMemoryPage::InsertMemoryPage(MemoryPage&& input_page, const std::string& page_name){
		if(page_name.size()==0) throw Error("PackMemoryPage","InsertMemoryPage","Parametro di ingresso nullo!");
		std::unordered_map<std::string,size_t>::iterator find_page = m_index_pages.find(page_name);
		if(find_page!=m_index_pages.end()) 
			return false;
		this->m_pack_pages.push_back(std::move(input_page));
		this->m_index_pages.insert(std::pair<std::string,size_t>(page_name,m_pack_pages.size()-1));
		return true;
	}

	void PackMemoryPage::Clear(){
		this->m_index_pages.clear();
		this->m_pack_pages.clear();
	}
}
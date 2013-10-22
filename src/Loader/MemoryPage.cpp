#include <XGame/Loader/MemoryPage.hpp>
#include <cstring>

namespace xgame{

	MemoryPage::MemoryPage():prtMemory(nullptr),sizePage(0){

	}

	MemoryPage::~MemoryPage(){
		this->Delete();
	}

	MemoryPage::MemoryPage(const MemoryPage& oth):sizePage(oth.sizePage),prtMemory(new uint8_t[oth.sizePage]){
		std::memcpy(static_cast<void*>(this->prtMemory),static_cast<void*>(oth.prtMemory),oth.sizePage);
	}

	MemoryPage::MemoryPage(MemoryPage&& oth){
		this->prtMemory = oth.prtMemory;
		this->sizePage = oth.sizePage;
		oth.prtMemory = nullptr;
		oth.sizePage = 0;
	}

	MemoryPage& MemoryPage::operator=(const MemoryPage& oth){
		if(this!=&oth){
			delete[] this->prtMemory;
			this->prtMemory = new uint8_t[oth.sizePage];
			this->sizePage = oth.sizePage;
			std::memcpy(static_cast<void*>(this->prtMemory),static_cast<void*>(oth.prtMemory),oth.sizePage);
		}
		return *this;
	}

	MemoryPage& MemoryPage::operator=(MemoryPage&& oth){
		if(this!=&oth){
			delete[] this->prtMemory;
			this->prtMemory = oth.prtMemory;
			this->sizePage = oth.sizePage;
			oth.prtMemory = nullptr;
			oth.sizePage = 0;
		}
		return *this;
	}

}
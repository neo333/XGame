#ifndef __MEMORY_PAGE__HPP
#define __MEMORY_PAGE__HPP

#include <cstdint>
#include <XGame/Core/Config.hpp>

namespace xgame{
class XGAME_API MemoryPage{
	MemoryPage();
	MemoryPage(const MemoryPage&);
	MemoryPage(MemoryPage&&);
	MemoryPage& operator=(const MemoryPage&);
	MemoryPage& operator=(MemoryPage&&);
	~MemoryPage();

	inline void Delete();
	inline const size_t GetSize() const;

private:
	uint8_t* prtMemory;
	size_t sizePage;
};


inline const size_t MemoryPage::GetSize() const{
	return this->sizePage;
}

inline void MemoryPage::Delete(){
	if(this->prtMemory!=nullptr){
		delete[] this->prtMemory;
		this->prtMemory=nullptr;
		this->sizePage=0;
	}
}

}

#endif
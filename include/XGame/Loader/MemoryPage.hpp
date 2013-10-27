#ifndef __MEMORY_PAGE__HPP
#define __MEMORY_PAGE__HPP

#include <cstdint>
#include <XGame/Core/Config.hpp>
#include <memory>

namespace xgame{

	class XGAME_API MemoryPage{
	public:
		//! Costruttore di default
		MemoryPage();

		//! Costruttore di copia
		MemoryPage(const MemoryPage&);

		//! Costruttore di move
		MemoryPage(MemoryPage&&);

		//! Operatore di assegnazione
		MemoryPage& operator=(const MemoryPage&);

		//! Operatore di move
		MemoryPage& operator=(MemoryPage&&);

		//! Distruttore
		~MemoryPage();

		//! Dealloca la pagina di memoria rilasciando tutte le risorse ad essa associate.
		inline void Delete();

		//! @return la dimensione (in bytes) della pagina di memoria.
		inline const size_t GetSize() const;

		//! @return Il puntatore all'area di memoria associata alla memory page.
		inline const void* Get_PtrMemory() const;



				/*---------------PRIVATE DATA-----------------*/
	private:	
		uint8_t* prtMemory;
		size_t sizePage;
		friend class ModuleCryp;
		friend class ModuleLoader;
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

	inline const void* MemoryPage::Get_PtrMemory() const{
		return static_cast<const void*>(this->prtMemory);
	}
}

#endif
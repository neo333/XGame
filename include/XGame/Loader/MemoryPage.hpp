#ifndef __MEMORY_PAGE__HPP
#define __MEMORY_PAGE__HPP

#include <XGame/Loader/Config.hpp>
#include <cstdint>
#include <memory>

namespace xgame{

	class XGAME_API_LOADER MemoryPage{
	public:
		//! Costruttore di default
		MemoryPage() throw();

		//! Costruttore di copia
		MemoryPage(const MemoryPage&);

		//! Costruttore di move
		MemoryPage(MemoryPage&&) throw();

		//! Operatore di assegnazione
		MemoryPage& operator=(const MemoryPage&);

		//! Operatore di move
		MemoryPage& operator=(MemoryPage&&) throw();

		//! Distruttore
		~MemoryPage() throw();

		//! Dealloca la pagina di memoria rilasciando tutte le risorse ad essa associate.
		inline void Delete() throw();

		//! @return la dimensione (in bytes) della pagina di memoria.
		inline const size_t GetSize() const throw();

		//! @return Il puntatore all'area di memoria associata alla memory page.
		inline const void* Get_PtrMemory() const throw();



				/*---------------PRIVATE DATA-----------------*/
	private:	
		uint8_t* prtMemory;
		size_t sizePage;
		friend class ModuleCryp;
		friend class ModuleLoader;
	};


	inline const size_t MemoryPage::GetSize() const throw(){
		return this->sizePage;
	}

	inline void MemoryPage::Delete() throw(){
		if(this->prtMemory!=nullptr){
			delete[] this->prtMemory;
			this->prtMemory=nullptr;
			this->sizePage=0;
		}
	}

	inline const void* MemoryPage::Get_PtrMemory() const throw(){
		return static_cast<const void*>(this->prtMemory);
	}
}

#endif
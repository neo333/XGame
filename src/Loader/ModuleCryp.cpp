#include <XGame/Loader/ModuleCryp.hpp>
#include <XGame/Loader/hashlib2plus/hashlibpp.h>
#include <limits>

namespace xgame{
	const size_t ModuleCryp::EndPage = std::numeric_limits<size_t>::max();

	ModuleCryp::ModuleCryp(){ 
		this->m_keyCryp.reserve(40);
	}

	void ModuleCryp::Set_Passphrase(const std::string& passphrase){
		if(passphrase.size()==0){
			m_keyCryp.clear();
		}else{
			try{
				m_keyCryp.clear();
				hashwrapper* engine_cryp = new sha1wrapper();
				m_keyCryp = engine_cryp->getHashFromString(passphrase);
				delete engine_cryp;
				engine_cryp=nullptr;
			}catch(...){
				throw Error("ModuleCryp","Set_Passphrase","Impossibile settare la 'passphare' del modulo!");
			}
		}
	}

	void ModuleCryp::CrypMemoryPage(MemoryPage* io_page, size_t offset, size_t len) const{
		if(this->m_keyCryp.size()==0 || io_page->GetSize()==0) return;
		const size_t KEY_DIM=m_keyCryp.size();
		if(len==ModuleCryp::EndPage) len=io_page->sizePage - offset;
		if(offset + len > io_page->sizePage) len=io_page->sizePage - offset;
		if(offset >= io_page->sizePage)  throw Error("ModuleCryp","CrypMemoryPage","Parametro di offset non corretto!");

		for(register size_t i=offset; i<len+offset; i++){
			io_page->prtMemory[i]+=static_cast<uint8_t>(m_keyCryp[i % KEY_DIM]);
		}
	}

	void ModuleCryp::DeCrypMemoryPage(MemoryPage* io_page,size_t offset, size_t len) const{
		if(this->m_keyCryp.size()==0 || io_page->GetSize()==0) return;
		const size_t KEY_DIM=m_keyCryp.size();
		if(len==ModuleCryp::EndPage) len=io_page->sizePage - offset;
		if(offset + len > io_page->sizePage) len=io_page->sizePage - offset;
		if(offset >= io_page->sizePage)  throw Error("ModuleCryp","DeCrypMemoryPage","Parametro di offset non corretto!");
		for(register size_t i=offset; i<len+offset; i++){
			io_page->prtMemory[i]-=static_cast<uint8_t>(m_keyCryp[i % KEY_DIM]);
		}

	}
}
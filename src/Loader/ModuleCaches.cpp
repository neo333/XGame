#define _CRT_SECURE_NO_WARNINGS
#include <XGame/Loader/ModuleCaches.hpp>
#include <XGame/Loader/ModuleLoader.hpp>
#include <XGame/Loader/hashlib2plus/hashlibpp.h>

namespace xgame{
	ModuleCaches::ModuleCaches():m_max_dimCache(ModuleCaches::sc_MAXDIMCACHES),dimCaches(0){ }
	
	void ModuleCaches::_hashStrKey(std::string& str_key){
		hashwrapper* engine_cryp = new sha1wrapper();
		str_key = engine_cryp->getHashFromString(str_key);
		delete engine_cryp;
		engine_cryp=nullptr;
	}

	bool ModuleCaches::InsertMemoryPage_intoCaches(const MemoryPage& input_page, const std::string& ref_filename) throw(Error){
		if(ref_filename.size()==0) throw Error("ModuleCaches","InsertMemoryPage_intoCaches","Parametro di input non valido!");
		if(input_page.GetSize() > m_max_dimCache) return false;
		time_t time_last_mod_file = ModuleLoader::GetLastMod_file(ref_filename);
		const std::string str_time_last_mod_file = ctime(&time_last_mod_file);
		std::string key_total = ref_filename;
		key_total+=str_time_last_mod_file;
		ModuleCaches::_hashStrKey(key_total);

		if(m_index.find(key_total)!=m_index.end()) return false;

		this->CleanOldCaches_toRemain(input_page.GetSize());

		m_index.insert(std::pair<std::string,puntatoreLista>(key_total,this->m_caches_store.insert(this->m_caches_store.begin(),Cache(input_page,key_total))));
		dimCaches+=input_page.GetSize();
		return true;
	}

	bool ModuleCaches::Find_and_Give_Caches(const std::string& ref_filename, MemoryPage& output_page) throw(Error){
		if(ref_filename.size()==0) throw Error("ModuleCaches","Find_and_Give_Caches","Parametro di input non valido!");
		time_t time_last_mod_file = ModuleLoader::GetLastMod_file(ref_filename);
		const std::string str_time_last_mod_file = ctime(&time_last_mod_file);
		std::string key_total = ref_filename;
		key_total+=str_time_last_mod_file;
		ModuleCaches::_hashStrKey(key_total);

		MappaIndici::iterator index_cache_find = m_index.find(key_total);
		if(index_cache_find == m_index.end()) return false;

		output_page.Delete();
		output_page = (*(*index_cache_find).second).page_data;

		this->UpdateKey(index_cache_find);

		return true;
	}

	void ModuleCaches::CleanOldCaches_toRemain(const size_t space_avail){
		while(m_max_dimCache - dimCaches > space_avail && m_caches_store.empty()==false){
			this->CleanOldCache();
		}
	}

	bool ModuleCaches::CleanOldCache(){
		if(m_caches_store.empty()){
			return false;
		}else{
			m_index.erase(m_caches_store.back().key_page);
			dimCaches-=m_caches_store.back().page_data.GetSize();
			m_caches_store.pop_back();
			return true;
		}
	}

	void ModuleCaches::CleanAllCaches(){
		this->m_caches_store.clear();
		this->m_index.clear();
		this->dimCaches=0;
	}
	
	void ModuleCaches::UpdateKey(MappaIndici::iterator& key_to_update){
		if(this->m_caches_store.size()==1) return;
		puntatoreLista old_key = (*key_to_update).second;
		(*key_to_update).second = m_caches_store.insert(m_caches_store.begin(),std::move(*old_key));
		m_caches_store.erase(old_key);
	}

	void ModuleCaches::Set_MaxSizeCaches(const size_t max_dim){
		this->m_max_dimCache=max_dim;
		while(this->dimCaches > this->m_max_dimCache){
			this->CleanOldCache();
		}
	}
}

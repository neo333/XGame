#define _POSIX_C_SOURCE 200809L
#include <XGame/Loader/ModuleLoader.hpp>
#include <XGame/Loader/hashlib2plus/hashlibpp.h>
#include <sys/stat.h>
#include <thread>
#include <vector>
#include <limits>
#include <future>
#include <stdexcept>

namespace xgame{
	ModuleLoader* ModuleLoader::s_prtInstance = nullptr;

	ModuleLoader::ModuleLoader():m_modulecryp_active(false),m_modulecaches_active(true){ }

	const time_t ModuleLoader::GetLastMod_file(const std::string& file_name) throw (...){
		if(file_name.size()==0) throw Error("ModuleLoader","GetLastMod_file","Parametro nullo!");
		struct stat info_file;
		int rts = stat(file_name.c_str(),&info_file);
		if(rts!=0)
			throw Error("ModuleLoader","GetLastMod_file","Impossibile accedere al file '%s'!",file_name.c_str());
		return info_file.st_mtime;
	}

	const std::string ModuleLoader::GetSignature_file(const std::string& file_name) throw(...){
		if(file_name.size()==0) throw Error("ModuleLoader","GetSignature_file","Parametro nullo!");
		try{
			hashwrapper* engine_cryp = new sha1wrapper();
			std::string rts = engine_cryp->getHashFromFile(file_name);
			delete engine_cryp;
			return rts;
		}catch(...){
			throw Error("ModuleLoader","GetSignature_file","Impossibile accedere al file '%s'!",file_name.c_str());
		}
	}

	void ModuleLoader::LoadFile_onMemoryPage(const std::string& namefile, MemoryPage& outpage) throw(...){
		if(namefile.size()==0) throw Error("ModuleLoader","LoadFile_onMemoryPage","Parametro di ingresso del file nullo!");
		try{
			bool finded_in_caches = false;
			if(this->m_modulecaches_active){
				std::unique_lock<std::mutex> cache_lock(m_mutex_modulecaches);
				finded_in_caches = m_modulecaches.Find_and_Give_Caches(namefile,outpage);
				cache_lock.unlock();
				if(finded_in_caches) return;
			}

			std::ifstream file;
			file.open(namefile.c_str(),std::ios_base::in | std::ios_base::binary);
			if(file.fail()) throw Error("ModuleLoader","LoadFile_onMemoryPage","Impossibile accedere in lettura al file '%s'",namefile.c_str());

			outpage.Delete();

			outpage.sizePage = ModuleLoader::GetSize_file(namefile);
			outpage.prtMemory = new uint8_t[outpage.sizePage];
			size_t curs_writer = 0;
			size_t curs_last = 0;

			std::vector<std::thread> threads_cryp;
			if(m_modulecryp_active)
				threads_cryp.reserve(static_cast<int>(outpage.sizePage / ModuleLoader::DIM_buffer)+1);

			while(file.eof()==false){
				curs_last = curs_writer;
				file.read(reinterpret_cast<char*>(outpage.prtMemory+curs_writer),static_cast<std::streamsize>(ModuleLoader::DIM_buffer));
				curs_writer+=static_cast<size_t>(file.gcount());
				if(m_modulecryp_active && file.gcount()>0){
					threads_cryp.push_back(std::thread(&ModuleCryp::DeCrypMemoryPage,std::ref(m_modulecryp),&outpage,curs_last,curs_writer-curs_last));
				}
			}
			file.close();

			for(auto& th: threads_cryp) th.join();

			if(finded_in_caches==false && m_modulecaches_active==true){
				std::unique_lock<std::mutex> cache_lock(this->m_mutex_modulecaches);
				this->m_modulecaches.InsertMemoryPage_intoCaches(outpage,namefile);
				cache_lock.unlock();
			}
		}catch(std::exception& err){
			throw Error("ModuleLoader","LoadFile_onMemoryPage",err.what());
		}
	}

	void ModuleLoader::WriteMemoryPage_onFile(const MemoryPage& inputpage, const std::string& namefile){
		if(namefile.size()==0) throw Error("ModuleLoader","WriteMemoryPage_onFile","Parametro di ingresso del file nullo!");
		
		std::ofstream file;
		file.open(namefile.c_str(), std::ios_base::out | std::ios_base::binary);
		if(file.fail()) throw Error("ModuleLoader","WriteMemoryPage_onFile","Impossibile accedere in scrittura al file '%s'",namefile.c_str());

		MemoryPage* page_cryp = nullptr;
		if(m_modulecryp_active){
			page_cryp = new MemoryPage(inputpage);
			m_modulecryp.CrypMemoryPage(page_cryp);
		}
		const MemoryPage* page_to_write = nullptr;
		if(page_cryp){
			page_to_write = page_cryp;
		}else{
			page_to_write = &inputpage;
		}

		size_t bytes_written = 0;
		size_t byte_to_write = ModuleLoader::DIM_buffer;
		while(bytes_written < page_to_write->GetSize()){
			if(byte_to_write > page_to_write->GetSize() - bytes_written) 
				byte_to_write = page_to_write->GetSize() - bytes_written;
			file.write(reinterpret_cast<const char*>(page_to_write->prtMemory + bytes_written),static_cast<std::streamsize>(byte_to_write));
			bytes_written+=byte_to_write;
		}

		file.close();

		if(page_cryp!=nullptr){
			delete page_cryp;
			page_cryp=nullptr;
		}
	}

	void ModuleLoader::ActiveModuleCryp(const std::string& passhrase) throw(...){
		m_modulecryp.Set_Passphrase(passhrase);
		m_modulecryp_active=true;
	}

	void ModuleLoader::DisableModuleCryp() throw(...){
		m_modulecryp.Set_Passphrase(std::string());
		m_modulecryp_active=false;
	}

	void ModuleLoader::ActiveModuleCaching(const size_t bytes_reserved){
		std::lock_guard<std::mutex> lock(m_mutex_modulecaches);
		this->m_modulecaches_active = true;
		this->m_modulecaches.Set_MaxSizeCaches(bytes_reserved);
	}

	void ModuleLoader::DisableModuleCaching(){
		std::unique_lock<std::mutex> lock(m_mutex_modulecaches);
		this->m_modulecaches_active=false;
		this->m_modulecaches.Set_MaxSizeCaches(0);
	}

	ModuleLoader::Header_packFile::Header_packFile(const PackMemoryPage& pack_input):n_pages(pack_input.GetSize()){
		this->len_name_page.resize(this->n_pages);
		this->name_page.resize(this->n_pages);
		this->len_page.resize(this->n_pages);

		for(size_t i=0; i<this->n_pages; i++){
			this->name_page[i] = pack_input.GetNamePage(i);
			this->len_name_page[i] = this->name_page[i].size();
			this->len_page[i] = pack_input[i].GetSize();
		}
	}

	const ModuleLoader::Header_packFile& ModuleLoader::Header_packFile::operator>>(std::ofstream& os) const{
		os.write(reinterpret_cast<const char*>(&this->n_pages),sizeof(size_t));
		for(size_t i=0; i<this->n_pages; i++){
			os.write(reinterpret_cast<const char*>(&(this->len_name_page[i])),sizeof(size_t));
			os.write(this->name_page[i].c_str(),this->name_page[i].size());
			os.write(reinterpret_cast<const char*>(&(this->len_page[i])),sizeof(size_t));
		}
		return *this;
	}

	const  ModuleLoader::Header_packFile&  ModuleLoader::Header_packFile::operator<<(std::ifstream& is){
		this->len_name_page.clear();
		this->name_page.clear();
		this->len_page.clear();
		is.read(reinterpret_cast<char*>(&this->n_pages),sizeof(size_t));
		if(is.gcount()!=sizeof(size_t)) 
			throw std::runtime_error("File corrotto!");

		this->len_name_page.resize(this->n_pages);
		this->name_page.resize(this->n_pages);
		this->len_page.resize(this->n_pages);

		for(size_t i=0; i<this->n_pages; i++){
			is.read(reinterpret_cast<char*>(&(this->len_name_page[i])),sizeof(size_t));
			if(is.gcount()!=sizeof(size_t)) 
				throw std::runtime_error("File corrotto!");

			char* buffer_name = new char[this->len_name_page[i]];
			is.read(buffer_name,this->len_name_page[i]);
			if(is.gcount()!=this->len_name_page[i]) 
				throw std::runtime_error("File corrotto!");
			this->name_page[i].assign(buffer_name,this->len_name_page[i]);
			delete[] buffer_name;

			is.read(reinterpret_cast<char*>(&(this->len_page[i])),sizeof(size_t));
			if(is.gcount()!=sizeof(size_t)) 
				throw std::runtime_error("File corrotto!");
		}

		return *this;
	}

	bool ModuleLoader::WritePackMemoryPage_onFile(const PackMemoryPage& inputPack, const std::string& namefile){
		if(namefile.size()==0) throw Error("ModuleLoader","WritePackMemoryPage_onFile","Parametro di ingresso del file nullo!");

		std::ofstream file;
		file.open(namefile.c_str(), std::ios_base::out | std::ios_base::binary);
		if(file.fail()) throw Error("ModuleLoader","WriteMemoryPage_onFile","Impossibile accedere in scrittura al file '%s'",namefile.c_str());

		PackMemoryPage* pack_cryp = nullptr;
		if(m_modulecryp_active)
			pack_cryp = new PackMemoryPage(inputPack);

		const size_t PACK_SIZE = inputPack.GetSize();

		std::vector<std::thread> th_cryp_pages;
		
		if(m_modulecryp_active){
			th_cryp_pages.reserve(PACK_SIZE);
			for(size_t i=0; i<PACK_SIZE; i++){
				MemoryPage* page_to_cryp = &(*pack_cryp)[i];
				th_cryp_pages.push_back(std::thread(&ModuleCryp::CrypMemoryPage,std::ref(m_modulecryp),page_to_cryp,0,ModuleCryp::EndPage));
			}
		}

		ModuleLoader::Header_packFile header(inputPack);
		header >> file;

		const PackMemoryPage* pack_to_write = nullptr;
		if(m_modulecryp_active){
			pack_to_write = pack_cryp;
		}else{
			pack_to_write = &inputPack;
		}

		for(size_t i=0; i<PACK_SIZE; i++){
			if(m_modulecryp_active) th_cryp_pages[i].join();
			file.write(reinterpret_cast<const char*>((*pack_to_write)[i].prtMemory),(*pack_to_write)[i].sizePage);
		}
		file.close();

		if(pack_cryp!=nullptr){
			delete pack_cryp;
			pack_cryp=nullptr;
		}
		return true;
	}

	bool ModuleLoader::LoadPackMemoryPage_fromFile(const std::string& namefile, PackMemoryPage& outpack){
		if(namefile.size()==0) throw Error("ModuleLoader","WritePackMemoryPage_onFile","Parametro di ingresso del file nullo!");

		std::ifstream file;
		file.open(namefile.c_str(), std::ios_base::in | std::ios_base::binary);
		if(file.fail()) throw Error("ModuleLoader","LoadPackMemoryPage_fromFile","Impossibile accedere in lettura al file '%s'",namefile.c_str());

		Header_packFile header;
		header << file;

		std::vector<std::thread> th_decryp;
		if(this->m_modulecryp_active)
			th_decryp.reserve(header.n_pages);

		outpack.Clear();
		for(size_t i=0; i<header.n_pages; i++){
			MemoryPage temp_mem;
			temp_mem.sizePage = header.len_page[i];
			temp_mem.prtMemory = new uint8_t[temp_mem.sizePage];
			file.read(reinterpret_cast<char*>(temp_mem.prtMemory),temp_mem.sizePage);
			if(file.gcount()!=temp_mem.sizePage)
				throw Error("ModuleLoader","LoadPackMemoryPage_fromFile","Impossibile leggere il file '%s'. Potrebbe essere corrotto!",namefile.c_str());
			outpack.InsertMemoryPage(std::move(temp_mem),header.name_page[i]);
			if(this->m_modulecryp_active){
				th_decryp.push_back(std::thread(&ModuleCryp::DeCrypMemoryPage,std::ref(m_modulecryp),&(outpack[i]),0,ModuleCryp::EndPage));
			}
		}
		file.close();

		for(auto& th: th_decryp) th.join();

		return true;
	}

	void ModuleLoader::MakeMemoryPage_fromMemory(const void* memory_input, const size_t size_input, MemoryPage& page_out) const throw(...){
		page_out.Delete();
		if (memory_input == nullptr || size_input==0) return;
		page_out.prtMemory = new uint8_t[size_input];
		page_out.sizePage = size_input;
		memcpy(static_cast<void*>(page_out.prtMemory), memory_input, size_input);
	}

}
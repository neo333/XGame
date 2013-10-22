#define _POSIX_C_SOURCE 200809L
#include <XGame/Loader/ModuleLoader.hpp>
#include <XGame/Loader/hashlib2plus/hashlibpp.h>
#include <sys/stat.h>
#include <thread>
#include <vector>
#include <limits>

namespace xgame{
	const size_t ModuleLoader::EndStream = std::numeric_limits<size_t>::max();
	ModuleLoader* ModuleLoader::s_prtInstance = nullptr;

	ModuleLoader::~ModuleLoader(){
		delete ModuleLoader::s_prtInstance;
	}

	ModuleLoader::ModuleLoader():m_modulecryp_active(false){ }

	const time_t ModuleLoader::GetLastMod_file(const std::string& file_name) throw (Error){
		if(file_name.size()==0) throw Error("ModuleLoader","GetLastMod_file","Parametro nullo!");
		struct stat info_file;
		int rts = stat(file_name.c_str(),&info_file);
		if(rts!=0)
			throw Error("ModuleLoader","GetLastMod_file","Impossibile accedere al file '%s'!",file_name.c_str());
		return info_file.st_mtime;
	}

	const std::string ModuleLoader::GetSignature_file(const std::string& file_name) throw(Error){
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

	void ModuleLoader::ActiveModuleCryp(const std::string& passhrase) throw(Error){
		m_modulecryp.Set_Passphrase(passhrase);
		m_modulecryp_active=true;
	}

	void ModuleLoader::DisableModuleCryp() throw(Error){
		m_modulecryp.Set_Passphrase(std::string());
		m_modulecryp_active=false;
	}

	void ModuleLoader::_loadDiskIntoPage(const std::string& file_name, const int dim_buffer, MemoryPage& out_page,
											const size_t offset =0, const size_t len =ModuleLoader::EndStream) const throw(Error){
		if(file_name.size()==0) throw Error("ModuleLoader","_loadDiskIntoPage","Parametro nullo!");
		std::ifstream file;
		file.open(file_name.c_str(),std::ios_base::in | std::ios_base::binary);
		if(file.fail())
			throw Error("ModuleLoader","_loadDiskIntoPage","Impossibile caricare il file '%s' dal disco!\n"
														   "Verificare che il file esista e che si abbiano i permessi di lettura necessari!",
														   file_name.c_str());
		out_page.Delete();
		out_page.sizePage = ModuleLoader::GetSize_file(file_name);
		out_page.prtMemory = new uint8_t[out_page.sizePage];

		char* buffer = new char[dim_buffer]; 
		size_t i_page=0;

		std::vector<std::thread> cryp_threads;

		while(file.eof()==false){
			file.read(buffer,dim_buffer);
			size_t i_page_mem=i_page;
			memcpy(static_cast<void*>(out_page.prtMemory+i_page),static_cast<void*>(buffer),static_cast<size_t>(file.gcount()));
			i_page+=static_cast<size_t>(file.gcount());
			if(this->m_modulecryp_active){
				cryp_threads.push_back(std::thread(&ModuleCryp::DeCrypMemoryPage,std::ref(m_modulecryp),
												out_page,i_page_mem,file.gcount()));

			}
		}

		delete[] buffer;
		buffer = nullptr;
		file.close();

		for(auto& th: cryp_threads) th.join();
	}

	void ModuleLoader::_storePageIntoDisk(const MemoryPage& input_page, const int dim_buffer, const std::string& out_filename, 
											const size_t offset =0, const size_t len =ModuleLoader::EndStream) const throw(Error){
		if(out_filename.size()==0) throw Error("ModuleLoader","GetLastMod_file","Parametro nullo!");

		std::ofstream file;
		file.open(out_filename,std::ios_base::out | std::ios_base::binary);
		if(file.fail()) 
			throw Error("ModuleLoader","_storePageIntoDisk","Impossibile creare il file '%s' sul disco!\n"
														   "Verificareche si abbiano i permessi di scrittura necessari!",
														   out_filename.c_str());
		MemoryPage page_mod = input_page;
		if(m_modulecryp_active)
			m_modulecryp.CrypMemoryPage(page_mod);
		for(register size_t i_page=0; i_page < input_page.sizePage; i_page+=dim_buffer){
			file.write(reinterpret_cast<const char*>(page_mod.prtMemory),dim_buffer);
		}

		file.close();
	}
}
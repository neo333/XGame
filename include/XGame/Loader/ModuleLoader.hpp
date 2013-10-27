#ifndef __MODULE_LOADER__HPP
#define __MODULE_LOADER__HPP

#include <XGame/Core/Config.hpp>
#include <XGame/Core/Error.hpp>
#include <XGame/Loader/MemoryPage.hpp>
#include <XGame/Loader/ModuleCryp.hpp>
#include <XGame/Loader/ModuleCaches.hpp>
#include <XGame/Loader/PackMemoryPage.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include <ctime>
#include <fstream>
#include <mutex>
#include <vector>

namespace xgame{
	class XGAME_API ModuleLoader: private boost::noncopyable{
	public:
		/*! Fornisce l'accesso all'instanza globale di questo componente. (Pattern: Singleton)
			@return L'instanza globale di questa classe.
		*/
		inline static ModuleLoader& GetGlobalInstance();


		/*! Ottiene il tempo dell'ultima modifica in scrittura avvenuta sul file specificato
			@param [in]	file_name	Il nome del file di cui si vuole conoscere la data dell'ultima modifica
			@return const time_t	Il time (data/ora) dell'ultima modifica apportata al file specificato nel parametro di ingresso.
			@throw Error	In caso il parametro non sia corretto o il file non esiste o � inaccessibile.
		*/
		static const time_t GetLastMod_file(const std::string& file_name) throw (Error);


		/*! Ottiene una firma riguardo il conenuto del file specificato sul parametro. Ogni variazione sul contenuto
			si avr� una variazione sulla firma.
			@param [in] file_name	Il nome del file di cui si vuole calcolare la firma sul contenuto
			@return const std::string	La stringa rappresentate la firma del file (in esadecimale).
			@throw Error	In caso il parametro non sia corretto o il file non esiste o � inaccessibile.
		*/
		static const std::string GetSignature_file(const std::string& file_name) throw(Error);


		/*! Ricava la dimensione in bytes del file specificato nel parametro di ingresso.
			@param [in] file_name	Il nome del file di cui si vuole sapere la dimensione in bytes.
			@return const size_t	La dimensione in bytes del file.
			@throw Error	In caso il parametro non sia corretto o il file non esiste o � inaccessibile.
		*/
		inline static const size_t GetSize_file(const std::string& file_name) throw(Error);


		/*! Attiva il modulo di crittografia interno!
			Se questo modulo viene attivato tutte le operazioni di 'loading' (lettura dal disco) verranno seguite automaticamente
			da una decrittografia con la chiave immessa!
			@param [in]	passhrase	La 'passhrase' con la quale sono state crittografati i file sul disco eventualmente.
			@throw Error			In caso di errore del modulo.
		*/
		void ActiveModuleCryp(const std::string& passhrase) throw(Error);


		//! Disattiva il modulo di crittografia interno! Le pagine verranno caricate dal disco senza operazioni di decrittografia!
		void DisableModuleCryp() throw(Error);


		/*! Attiva il modulo di caching interno.
			NOTA BENE: Il modulo di caching � di default abilitato!

			@param [in] bytes_reserved		Il numero di bytes riservati in memoria centrale per l'archiviazione delle caches.
		*/
		void ActiveModuleCaching(const size_t bytes_reserved);


		//! @return Il modulo caching interno al loader!
		inline const ModuleCaches& Get_InfoModuleCaching() const;
		

		//! Disattiva il modulo caching.
		void DisableModuleCaching();


		/*! Carica un file dalla memoria di massa e riporta tutto il suo contenuto sulla memoryPage di output.
			All'operazione di caricamento seguir�, se il modulo cryp � abilitato, un'operazione di decifratura automatica sul conenuto
			letto.

			@param [in] namefile		Il nome del file che si vuole caricare in memoria.
			@param [out]outpage			La pagina di memoria dove verr� ricopiato tutto il conenuto del file.
		*/
		void LoadFile_onMemoryPage(const std::string& namefile, MemoryPage& outpage) throw(Error);


		/*! Scrive il contenuto di una pagina di memoria su un file (sulla memoria di massa).
			L'operazione di scrittura verr� preceduta, se il modulo cryp � abilitato, da un'operazione di cifratura automatica
			sul contenuto prima di essere scritto.

			@param [in] inputpage	La pagina di memoria che deve essere scritta.
			@param [in] namefile	Il nome del file che si desidera scrivere.
									Attenzione: se il file esiste il conenuto precedente del file verr� completamente cancellato!
		*/
		void WriteMemoryPage_onFile(const MemoryPage& inputpage, const std::string& namefile);


		/*! Scrive il contenuto di un pacchetto di pagine di memoria su un file (sulla memoria di massa).
			L'operazione di scrittura verr� preceduta, se il modulo cryp � abilitato, da un'operzione di cifratura automatica
			sul contenuto prima di essere scritto.

			@param [in] inputPack	Il pacchetto di pagine di memoria che deve essere scritto su file.
			@param [in]	namefile	Il nome del file che si desidera scrivere.
									Attenzione: se il file esiste verr� sovrascritto!
		*/
		bool WritePackMemoryPage_onFile(const PackMemoryPage& inputPack, const std::string& namefile);


		/*! Carica un pacchetto di pagine in memoria da un file che era stato precedentemente scritto con l'apposito metodo.
			L'operazione di lettura potrebbe essere pi� lenta rispetto alla lettura di una singola pagina di memoria,
			sia per il numero maggiore di bytes coinvolti, sia perch� il modulo caching � attualmente disabilitato
			per i pacchetti di memoria.

			@param [in] namefile		Il nome del file che si desidera leggere
			@param [out] outpack		Il pacchetto di pagine di memoria che verr� scritto col contenuto del file.
		*/
		bool LoadPackMemoryPage_fromFile(const std::string& namefile, PackMemoryPage& outpack);



	private:
		static ModuleLoader* s_prtInstance;
		ModuleLoader();
		ModuleCryp m_modulecryp;
		bool m_modulecryp_active;
		ModuleCaches m_modulecaches;
		bool m_modulecaches_active;
		std::mutex m_mutex_modulecaches;

		static const size_t DIM_buffer = 1024;	//TODO: portare a 1024!

		struct Header_packFile{
			size_t n_pages;
			std::vector<std::string> name_page;
			std::vector<size_t> len_name_page;
			std::vector<size_t> len_page;

			Header_packFile();
			Header_packFile(const PackMemoryPage& pack_input);

			const Header_packFile& operator>>(std::ofstream& os) const;
			const Header_packFile& operator<<(std::ifstream& is);
		};
	};






	inline ModuleLoader& ModuleLoader::GetGlobalInstance(){
		if(ModuleLoader::s_prtInstance==nullptr){
			ModuleLoader::s_prtInstance = new ModuleLoader();
		}
		return *ModuleLoader::s_prtInstance;
	}

	inline const size_t ModuleLoader::GetSize_file(const std::string& file_name) throw(Error){
		std::ifstream file;
		file.open(file_name,std::ios_base::binary);
		if(file.fail())
			throw Error("ModuleLoader","GetSize_file","Impossibile aprire il file '%s'!", file_name.c_str());
		file.seekg(0,file.end);
		size_t rts = static_cast<size_t>(file.tellg());
		file.close();
		return rts;
	}

	inline const ModuleCaches& ModuleLoader::Get_InfoModuleCaching() const{
		return m_modulecaches;
	}

#define Loader xgame::ModuleLoader::GetGlobalInstance()
}

#endif
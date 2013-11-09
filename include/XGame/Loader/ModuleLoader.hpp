#ifndef __MODULE_LOADER__HPP
#define __MODULE_LOADER__HPP

#include <XGame/Loader/Config.hpp>
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

#ifdef WIN32 
#pragma warning(disable:4251)
#pragma warning(disable:4275)
#endif

namespace xgame{
	class XGAME_API_LOADER ModuleLoader : private boost::noncopyable{
	public:
		/*! Fornisce l'accesso all'instanza globale di questo componente. (Pattern: Singleton)
			\return L'instanza globale di questa classe.
		*/
		inline static ModuleLoader& GetGlobalInstance();


		/*! Ottiene il tempo dell'ultima modifica in scrittura avvenuta sul file specificato.

			\param [in]	file_name	Il nome del file di cui si vuole conoscere la data dell'ultima modifica
			\return const time_t	Il time (data/ora) dell'ultima modifica apportata al file specificato nel parametro di ingresso.
			\throw Error	In caso il parametro non sia corretto o il file non esiste o è inaccessibile.
		*/
		static const time_t GetLastMod_file(const std::string& file_name) throw (...);


		/*! Ottiene una firma riguardo il conenuto del file specificato sul parametro. Ogni variazione sul contenuto
			si avrà una variazione sulla firma.

			\param [in] file_name	Il nome del file di cui si vuole calcolare la firma sul contenuto
			\return const std::string	La stringa rappresentate la firma del file (in esadecimale).
			\throw Error	In caso il parametro non sia corretto o il file non esiste o è inaccessibile.
		*/
		static const std::string GetSignature_file(const std::string& file_name) throw(...);


		/*! Ricava la dimensione in bytes del file specificato nel parametro di ingresso.

			\param [in] file_name	Il nome del file di cui si vuole sapere la dimensione in bytes.
			\return const size_t	La dimensione in bytes del file.
			\throw Error			In caso il parametro non sia corretto o il file non esiste o è inaccessibile.
		*/
		inline static const size_t GetSize_file(const std::string& file_name) throw(...);


		/*! Attiva il modulo di crittografia interno!
			Se questo modulo viene attivato tutte le operazioni di 'loading' (lettura dal disco) verranno seguite automaticamente
			da una decrittografia con la chiave immessa!

			\param [in]	passhrase	La 'passhrase' con la quale sono state crittografati i file sul disco eventualmente.
			\throw Error			In caso di errore del modulo.
		*/
		void ActiveModuleCryp(const std::string& passhrase) throw(...);


		//! Disattiva il modulo di crittografia interno! Le pagine verranno caricate dal disco senza operazioni di decrittografia!
		void DisableModuleCryp() throw(...);


		/*! Attiva il modulo di caching interno.

			\param [in] bytes_reserved		Il numero di bytes riservati in memoria centrale per l'archiviazione delle caches.
			\note	Il modulo di caching è di default abilitato!
		*/
		void ActiveModuleCaching(const size_t bytes_reserved);


		//! \return Il modulo caching interno al loader!
		inline const ModuleCaches& Get_InfoModuleCaching() const;
		

		//! Disattiva il modulo caching.
		void DisableModuleCaching();


		/*! Carica un file dalla memoria di massa e riporta tutto il suo contenuto sulla memoryPage di output.
			All'operazione di caricamento seguirà, se il modulo cryp è abilitato, un'operazione di decifratura automatica sul conenuto
			letto.

			\param [in] namefile		Il nome del file che si vuole caricare in memoria.
			\param [out]outpage			La pagina di memoria dove verrà ricopiato tutto il conenuto del file.
		*/
		void LoadFile_onMemoryPage(const std::string& namefile, MemoryPage& outpage) throw(...);

		/*! Carica un file dalla memoria di massa e riporta tutto il suo contenuto sulla memoryPage di output.
			All'operazione di caricamento seguirà, se il modulo cryp è abilitato, un'operazione di decifratura automatica sul conenuto
			letto.

			\param [in] namefile		Il nome del file che si vuole caricare in memoria.
			\return						La pagina di memoria dove verrà ricopiato tutto il conenuto del file.
			\note						C++11, ora, utilizza il costruttore di MOVE per i 'return' delle funzioni.
										la classe \see MemoryPage supporta completamente lo standard C++11
		*/
		inline MemoryPage LoadFile_onMemoryPage(const std::string& namefile) throw(...);


		/*! Scrive il contenuto di una pagina di memoria su un file (sulla memoria di massa).
			L'operazione di scrittura verrà preceduta, se il modulo cryp è abilitato, da un'operazione di cifratura automatica
			sul contenuto prima di essere scritto.

			\param [in] inputpage	La pagina di memoria che deve essere scritta.
			\param [in] namefile	Il nome del file che si desidera scrivere.
									Attenzione: se il file esiste il conenuto precedente del file verrà completamente cancellato!
		*/
		void WriteMemoryPage_onFile(const MemoryPage& inputpage, const std::string& namefile);


		/*! Scrive il contenuto di un pacchetto di pagine di memoria su un file (sulla memoria di massa).
			L'operazione di scrittura verrà preceduta, se il modulo cryp è abilitato, da un'operzione di cifratura automatica
			sul contenuto prima di essere scritto.

			\param [in] inputPack	Il pacchetto di pagine di memoria che deve essere scritto su file.
			\param [in]	namefile	Il nome del file che si desidera scrivere.
									Attenzione: se il file esiste verrà sovrascritto!
		*/
		bool WritePackMemoryPage_onFile(const PackMemoryPage& inputPack, const std::string& namefile);


		/*! Carica un pacchetto di pagine in memoria da un file che era stato precedentemente scritto con l'apposito metodo.
			L'operazione di lettura potrebbe essere più lenta rispetto alla lettura di una singola pagina di memoria,
			sia per il numero maggiore di bytes coinvolti, sia perché il modulo caching è attualmente disabilitato
			per i pacchetti di memoria.

			\param [in] namefile		Il nome del file che si desidera leggere
			\param [out] outpack		Il pacchetto di pagine di memoria che verrà scritto col contenuto del file.
		*/
		bool LoadPackMemoryPage_fromFile(const std::string& namefile, PackMemoryPage& outpack);

		/*! Carica una Pagina di Memoria come COPIA di un'area di memoria già inizializzata in RAM.
			
			\param [in] memory_input	Il puntatore all'area di memoria da copiare.
			\param [in] size_input		La dimensione (espressa in byte) dell'area di memoria da copiare.
			\param [out] page_out		La pagina di memoria che custodirà la COPIA all'area di memoria in ingresso.

			\note						Questa tipologia di caricamento non utilizza tecniche di caching o crittografia!
			\note						La chiamata di questa metodo non avrà alcune effetto sulla memoria di input.
		*/
		void MakeMemoryPage_fromMemory(const void* memory_input, const size_t size_input, MemoryPage& page_out) const throw(...);
		inline MemoryPage MakeMemoryPage_fromMemory(const void* memory_input, const size_t size_input) const throw(...);





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

	inline const size_t ModuleLoader::GetSize_file(const std::string& file_name) throw(...){
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

	inline MemoryPage ModuleLoader::LoadFile_onMemoryPage(const std::string& namefile) throw(...){
		MemoryPage rts;
		this->LoadFile_onMemoryPage(namefile,rts);
		return rts;
	}

	inline MemoryPage ModuleLoader::MakeMemoryPage_fromMemory(const void* memory_input, const size_t size_input) const throw(...){
		MemoryPage rts;
		this->MakeMemoryPage_fromMemory(memory_input, size_input, rts);
		return rts;
	}
}

#define Loader xgame::ModuleLoader::GetGlobalInstance()

#endif
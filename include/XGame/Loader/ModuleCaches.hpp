#ifndef __MODULE_CACHES__HPP
#define __MODULE_CACHES__HPP

#include <XGame/Core/Config.hpp>
#include <XGame/Core/Error.hpp>
#include <XGame/Loader/MemoryPage.hpp>
#include <boost/noncopyable.hpp>
#include <list>
#include <unordered_map>

#ifdef WIN32 
#pragma warning(disable:4251)
#pragma warning(disable:4275)
#endif

namespace xgame{
	class XGAME_API ModuleCaches: private boost::noncopyable{
	public:
		//! Costruttore di default
		ModuleCaches();

		/*! Inserisce una pagina di memoria all'interno del modulo di caching.
			Verrà creata una COPIA della pagina di input e memorizzata nelle caches.
			Questa operazione può risultare utile quando la pagina di memoria, che è stata precedentemente caricata dall'hard disk,
			potrebbe essere necessaria in futuro. Infatti, tramite l'apposito metodo del modulo, è possibile riottenere la stessa pagina
			in un secondo momento.
			NOTA BENE: Per funzionare in maniera corretta il meccanismo di caching mappa una determinata pagina di memoria con un
				relativo file! Per inserire una pagina di memoria all'interno di questo modulo è necessario specificare, inoltre,
				il file dal quale quella pagina di memoria è stata caricata. Il file deve necessariamente ESISTERE e avere
				in questo momento lo stesso contenuto della pagina di input.

			@param [in] input_page		Pagina di memoria appena caricata da un file che si vuole salvare per un riutilizzo futuro.
			@param [in] ref_filename	Il nome del file dalla quale si ha APPENA caricata la pagina di memoria!
										Il contenuto del file deve essere (all'atto della chiamata di questo metodo) lo stesso della
										pagina di memoria!

			@return						'true' se la pagina di memoria viene inserita correttamente nelle caches
										'false' se la pagina di memoria è troppo grande per la dimensione massima delle caches
											o già esiste all'interno del modulo (in quel caso non verrà inserita!)

			@throw Error				In caso il file non esista!
		*/
		bool InsertMemoryPage_intoCaches(const MemoryPage& input_page, const std::string& ref_filename) throw(...);


		//! Svuota tutte le caches!
		void CleanAllCaches();

		
		/*! Prova una ricerca all'interno delle caches.
			Questa funzione serve quando si vuole caricare un file in memoria centrale. Se quel file è stato già caricato e la 
			relativa pagina memorizzata nelle caches, non sarà necessario effettura una lettura del file completo, ma sarà
			possibile ricavare il contenuto velocemente tramite questo apposito metodo.
			Se la cache viene trovata la pagina di memoria di output sarà scritta, altrimenti non verrà toccata!

			@param [in]	ref_filename	Il nome del file che si vorrebbe caricare in memoria centrale.
			@param [out]output_page		La pagina di memoria dove, in caso sia stata memorizzata in precedenza la cache relativa del
										file di ingresso, verrà salvato il contenuto del file direttamente.

			@return						'true' se la cache viene trovata e scritta sulla pagina di memoria di output
										'false' se la cache non viene trovata (cache-miss) e la pagina di memoria di output non verrà toccata!

			@throw Error				In caso il file di riferimento non esista!
		*/
		bool Find_and_Give_Caches(const std::string& ref_filename, MemoryPage& output_page) throw(...);


		//! @return		Il numero di bytes totali occupati attualmente dalle caches.
		inline const size_t Get_SizeCurrentCaches() const;


		//! @return		Il numero di bytes massimi riservati per il modulo di caching.
		inline const size_t Get_MaxSizeCaches() const;

		/*! Setta il numero massimo in bytes occupabili dal modulo di caching.
			@param [in] max_dim		La dimensione in bytes massima riservata al modulo di caching.
		*/
		void Set_MaxSizeCaches(const size_t max_dim);

	private:
		struct Cache{
			MemoryPage page_data;
			std::string key_page;

			Cache(const MemoryPage& input_page, const std::string& input_key):page_data(input_page),key_page(input_key){ }
			Cache& operator=(Cache&& oth){
				if(this!=&oth){
					this->key_page = std::move(oth.key_page);
					this->page_data = std::move(oth.page_data);
				}
				return *this;
			}
			Cache(Cache&& oth):key_page(std::move(oth.key_page)),page_data(std::move(oth.page_data)){ }
		};
		typedef std::list<Cache> ListaPagine;
		typedef ListaPagine::iterator puntatoreLista;
		typedef std::unordered_map<std::string,puntatoreLista> MappaIndici;

		ListaPagine m_caches_store;
		MappaIndici m_index;
		size_t m_max_dimCache;
		size_t dimCaches;
		static const size_t sc_MAXDIMCACHES = 10485760;

		static void _hashStrKey(std::string& str_key);
		void CleanOldCaches_toRemain(const size_t space_avail);
		bool CleanOldCache();
		void UpdateKey(MappaIndici::iterator& key_to_update);
	};


	inline const size_t ModuleCaches::Get_SizeCurrentCaches() const{
		return this->dimCaches;
	}
}

#endif
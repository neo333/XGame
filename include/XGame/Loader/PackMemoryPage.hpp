#ifndef __PACK_MEMORYPAGE__HPP
#define __PACK_MEMORYPAGE__HPP

#include <XGame/Loader/Config.hpp>
#include <XGame/Core/Error.hpp>
#include <XGame/Loader/MemoryPage.hpp>
#include <unordered_map>
#include <vector>

#ifdef XGAME_WINDOWS_OS 
#pragma warning(disable:4251)
#endif

namespace xgame{
	class XGAME_API_LOADER PackMemoryPage{
	public:
		//! Costruttore di default.
		PackMemoryPage() throw();


		/*! Inserisce una MemoryPage all'interno del pacchetto di pagine.
			La pagina verrà inserita con una stringa rappresentate il "nome della pagina" all'interno del pacchetto
			(due pagine all'interno del pacchetto non possono avere nome uguali -> la chiave è UNICA).
			La chiave può servire in futuro per recuperare la pagina di memoria dal pacchetto.

			\param [in] input_page		La pagina di memoria che si vuole SPOSTARE all'interno del pacchetto.
										Il parametro di input sarà INVALIDATO dopo la chiamata di questa funzione.
										(Utilizzare 'std::move(page)')
			\param [in] page_name		Un nome simbolico identificativo che la pagina prendere all'interno del pacchetto.
										Questa stringa funziona da chiave per l'estrazione della pagina dal pacchetto
										in un secondo momento.

			\return						'true' se la pagina viene correttamente spostata all'interno del pacchetto
											(invalidando il parametro di ingresso)
										'false'	se la pagina non viene spostata all'interno del pacchetto. Perché già presente
											una pagina con lo stesso nome (non invalida il parametro di ingresso).

			\note						la pagina di memoria di input verrà SPOSTATA all'interno del pacchetto.
										In poche parole non verrà fatta una copia dell'ingresso, ma la pagina di memoria 
										di ingresso verrà invalidata e spostata all'interno del pacchetto di pagine.
		*/
		bool InsertMemoryPage(MemoryPage&& input_page, const std::string& page_name);
		

		//! Accede all'n-esima pagina di memoria all'interno del pacchetto.
		inline MemoryPage& operator[](const size_t index);
		inline const MemoryPage& operator[](const size_t index) const;
		
		/*! Accede alla pagina di memoria all'interno del pacchetto con un determinato nome di riferimento.
			\note	 Se non esiste alcuna pagina di memoria con quel nome, verrà lanciata un'eccezione!
		*/
		inline MemoryPage& operator[](const std::string& page_name);
		inline const MemoryPage& operator[](const std::string& page_name) const;

		//! \return Il numero di pagine di memoria attualmente contenuto all'interno di questo pacchetto di pagine
		inline const size_t GetSize() const throw();

		//! Cancella tutte le pagine contenuto all'interno del pacchetto 'resettandolo'.
		void Clear() throw();

		/*! \param [in] index	L'indice di riferimento della pagina all'interno del pacchetto.
			\return				Il nome di riferimento della pagina all'interno del pacchetto.
		*/
		inline const std::string& GetNamePage(const size_t index) const;




	private:
		std::unordered_map<std::string,size_t> m_index_pages;
		std::vector<MemoryPage> m_pack_pages;
		friend class ModuleLoader;
	};

	inline const size_t PackMemoryPage::GetSize() const throw(){
		return this->m_pack_pages.size();
	}

	inline MemoryPage& PackMemoryPage::operator[](const size_t index){
		return this->m_pack_pages[index];
	}
	inline const MemoryPage& PackMemoryPage::operator[](const size_t index) const{
		return this->m_pack_pages[index];
	}
	inline MemoryPage& PackMemoryPage::operator[](const std::string& page_name){
		std::unordered_map<std::string,size_t>::iterator finder = m_index_pages.find(page_name);
		if(finder == m_index_pages.end()) 
			throw Error("PackMemoryPage","operator[]","Impossibile accedere alla pagina di memoria nominata '%s'",page_name.c_str());
		return m_pack_pages[(*finder).second];
	}
	inline const MemoryPage& PackMemoryPage::operator[](const std::string& page_name) const{
		std::unordered_map<std::string,size_t>::const_iterator finder = m_index_pages.find(page_name);
		if(finder == m_index_pages.end()) 
			throw Error("PackMemoryPage","operator[]","Impossibile accedere alla pagina di memoria nominata '%s'",page_name.c_str());
		return m_pack_pages[(*finder).second];
	}

	inline const std::string& PackMemoryPage::GetNamePage(const size_t index) const{
		//TODO: ricerca sequenziale.. è possibile migliorare le prestazioni
		std::unordered_map<std::string,size_t>::const_iterator it;
		for(it=this->m_index_pages.cbegin(); it!=this->m_index_pages.cend(); it++){
			if((*it).second==index) return (*it).first;
		}
		throw Error("PackMemoryPage","GetNamePage","Indice di riferimento non valido all'interno della struttura dati");
	}
};

#endif
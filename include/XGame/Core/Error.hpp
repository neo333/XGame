#ifndef __ERROR__HPP
#define __ERROR__HPP

#include <exception>
#include <XGame/Core/Config.hpp>
#include <string>
#include <vector>
#include <cstdarg>

namespace xgame{
	class Error: public std::exception{
	public:
		/** Costruttore
			@param [in] name_class		Il nome della classe che ha originato l'errore. E' una stringa identificativa.
			@param [in] name_method		Il nome del metodo/funzione dove si è originato l'errore. E' una stringa identificativa.
			@param [in] what			Una descrizione dell'errore!
										E' possibile utilizzare la notazione %s per indicare una sostituizione con una stringa.
			@param [in]	...				Tutte le possibili stringhe che vanno a rimpiazzare i %s nella descrizione
		*/
		inline Error(const char* name_class, const char* name_method, const char* what, ...);

		/** @return La descrizione formattata dell'errore!*/
		inline virtual const char* what() const throw();





	private:
		std::string m_name_class;
		std::string m_name_method;
		std::string m_what;
		std::string m_formatted;
	};

	inline Error::Error(const char* name_class, const char* name_method, const char* what, ...):
				m_name_class(name_class),m_name_method(name_method),m_what(what),exception(what){
		va_list args;
		va_start(args,what);
		size_t off_set=0;
		std::vector<size_t> calling_place;
		size_t finder;
		while((finder = m_what.find("%s",off_set))!=m_what.npos){
			calling_place.push_back(finder);
			off_set=finder+1;
		}
		for(size_t i=0; i<calling_place.size(); i++){
			std::string arg_temp(va_arg(args,char*));
			m_what = m_what.replace(calling_place[i],2,arg_temp);
		}
		va_end(args);

		m_formatted = "@";
		m_formatted+=this->m_name_class;
		m_formatted+="::";
		m_formatted+=this->m_name_method;
		m_formatted+=">> ";
		m_formatted+=this->m_what;
	}

	inline const char* Error::what() const throw(){
		return m_formatted.c_str();
	}
}



#endif
#include <XGame/Script/Config.hpp>
#include <XGame/Script/lua/lua.hpp>
#include <XGame/Core/Error.hpp>
#include <XGame/Loader/MemoryPage.hpp>
#include <string>

#ifdef XGAME_MSVC
#pragma warning(disable:4290)
#pragma warning(disable:4251)
#endif

#include <XGame/Script/BinderClass.hpp>


namespace xgame{
	class XGAME_API_SCRIPT Script{
	public:
		//! Costruttore di default.
		Script() = default;

		//! Distruttore virtuale.
		virtual ~Script();

		//! Costruttore di copia eliminato
		Script(const Script&) = delete;

		//! Operatore di assegnazione eliminato
		Script& operator=(const Script&) = delete;

		//! Costruttore di move eliminato
		Script(Script&&) = delete;

		//! Operatore di move eliminato.
		Script& operator=(Script&&) = delete;

		//! Dealloca tutte le risorse assegnate a questo script.
		void Clean() throw();

		/*! Carica uno script LUA con le informazioni contenute in una Pagina di Memoria.
			\param [in] name_script			Un nome simbolico (di riferimento) per lo script in questione.
			\param [in,out] input_page		Una pagina di memoria contenente lo script in stringa.

			\note							La pagina di memoria in ingresso verrà spostata
											all'interno dello script.
			
			\throw Error					In caso di errore di sintassi dello script o di allocazione della memoria.
		*/
		void LoadScript(const std::string& name_script, MemoryPage&& input_page) throw(Error);

		//! \return 'true' se lo script è stato caricato precedentemente correttamente, 'false' altrimenti.
		inline const bool Is_Load() const throw();

		/*! Esegue lo script se 'caricato'. Altrimenti non fa nulla.

			\throw Error	In caso di errore nell'esecuzione o l'interpretazione dello script.
		*/
		void Run() throw(Error);

		/*! Registra una funzione C,C++ allo script.
			Una volta agganciata la funzione con questo metodo sarà possibile richiamare la stessa
			dallo script.

			\note	Questo metodo va chiamato PRIMA di eseguire lo script. Prima del metodo 'Script::Run'.
		*/
		template<class Fn>void RegisterFunction(const std::string& funct_name,Fn&& function);

		/*! Registra una class C++ allo script.
			Una volta agganciata la classe con questo metodo sarà possibile richiamare la stessa dallo script,
			o usare oggetti di quella classe come parametri.

			\note	Questo metodo va chiamato PRIMA di eseguire lo script. Prima del metodo 'Script::Run'.
		*/
		template<class Type>void RegisterClass(const BinderClass<Type>& bind_class);

		/*! Chiama ed esegue una funzione dello script.
			
			\note	Questo metodo va chiamato DOPO aver eseguito lo script (in modo che sia stato interpretato).
					Quindi prima del metodo 'Script::Run'.

			\note	Utilizzare 'boost::ref' per passare un argomento per riferimento.
		*/
		template<class RetValue, class... Arg>RetValue CallFunction(const std::string& funct_name, Arg&&... args) throw(Error);

		//! Operatore di casting esplicito a puntatore lua_State.
		inline explicit operator lua_State*() throw();

		//! Operatore di casting esplicito a puntatore cost lua_State.
		inline explicit operator const lua_State*() const throw();
	private:
		lua_State* m_state_script = nullptr;
		MemoryPage m_ref_script;
		std::string m_name_script;
		bool m_runned = false;

		static int handle_error(lua_State*);
	};

	inline const bool Script::Is_Load() const throw(){
		if (m_state_script) return true;
		return false;
	}

	template<class Fn>void Script::RegisterFunction(const std::string& funct_name, Fn&& function){
		luabind::module(m_state_script)
			[
				luabind::def(funct_name.c_str(), function)
			];
	}

	template<class RetValue, class... Arg>RetValue Script::CallFunction(const std::string& funct_name, Arg&&... args) throw(Error){
		try{
			return luabind::call_function<RetValue>(m_state_script, funct_name.c_str(), args...);
		}
		catch (const std::exception& err){
			std::string _what = err.what();
			_what += '\n';
			const char* info_debug = lua_tostring(m_state_script, 1);
			if (info_debug)
				_what += info_debug;
			throw Error("Script", "CallFunction", _what.c_str());
		}
	}

	template<class Type>void Script::RegisterClass(const BinderClass<Type>& bind_class){
		luabind::module(m_state_script)
			[
				bind_class.m_ref_class
			];
	}

	inline Script::operator lua_State*() throw(){
		return m_state_script;
	}

	inline Script::operator const lua_State*() const throw(){
		return m_state_script;
	}
}
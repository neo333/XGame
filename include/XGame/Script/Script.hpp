#include <XGame/Script/Config.hpp>
#include <XGame/Script/lua/lua.hpp>
#include <XGame/Core/Error.hpp>
#include <XGame/Loader/MemoryPage.hpp>
#include <string>

#ifdef XGAME_MSVC
#pragma warning(disable:4290)
#pragma warning(disable:4251)
#endif

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

		inline const bool Is_Load() const throw();

		void Run();
	private:
		lua_State* m_state_script = nullptr;
		MemoryPage m_ref_script;
		std::string m_name_script;
	};

	inline const bool Script::Is_Load() const throw(){
		if (m_state_script) return true;
		return false;
	}
}
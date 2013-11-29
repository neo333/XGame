#include <XGame/Script/Script.hpp>
#include <sstream>

namespace xgame{
	Script::~Script(){
		this->Clean();
	}

	void Script::Clean(){
		if (this->m_state_script){
			lua_close(this->m_state_script);
			this->m_state_script = nullptr;
		}
		this->m_ref_script.Delete();
		this->m_name_script.clear();
		this->m_runned = false;
	}

	void Script::LoadScript(const std::string& name_script, MemoryPage&& input_page) throw(Error){
		this->Clean();
		if (input_page.GetSize() == 0) return;
		m_name_script = name_script;
		m_state_script = luaL_newstate();
		if (m_state_script == nullptr)
			throw Error("Script", "LoadScript", "Impossibile instanziare uno stato coerente per lo script!");
		luaL_openlibs(m_state_script);
		this->m_ref_script = std::move(input_page);
		int result = luaL_loadbuffer(
			m_state_script, 
			static_cast<const char*>(m_ref_script.Get_PtrMemory()), 
			m_ref_script.GetSize(), 
			m_name_script.c_str()
			);
		if (result == LUA_ERRSYNTAX)
			throw Error("Script", "LoadScript", "Errore di sintassi nello script '%s'",m_name_script.c_str());
		else if (result == LUA_ERRMEM)
			throw Error("Script", "LoadScript", "Errore nell'allocazione di memoria nello script '%s'", m_name_script.c_str());
		else if (result == LUA_ERRGCMM)
			throw Error("Script", "LoadScript", "Errore di interpretazione dello script '%s'", m_name_script.c_str());

		try{
			luabind::open(m_state_script);
			luabind::set_pcall_callback(Script::handle_error);
		}
		catch (const std::exception& err){
			throw Error("Script", "LoadScript", err.what());
		}
	}

	void Script::Run() throw(Error){
		if (this->Is_Load()){
			int result = lua_pcall(m_state_script, 0, LUA_MULTRET, 0);
			if (result == LUA_ERRRUN)
				throw Error("Script", "Run", "Errore a run time nello script '%s'!", m_name_script.c_str());
			else if (result == LUA_ERRMEM)
				throw Error("Script", "Run", "Errore allocazione della memoria nello script '%s'!", m_name_script.c_str());
			else if (result == LUA_ERRERR)
				throw Error("Script", "Run", "Errore messaggio nello script '%s'", m_name_script.c_str());
			else if (result == LUA_ERRGCMM)
				throw Error("Script", "LoadScript", "Errore di interpretazione dello script '%s'", m_name_script.c_str());
			if (result != LUA_OK)
				throw Error("Script", "LoadScript", "Errore sconosiuto dello script '%s'", m_name_script.c_str());
			this->m_runned = true;
		}
	}

	int Script::handle_error(lua_State* L){
		lua_Debug d;
		lua_getstack(L, 1, &d);
		lua_getinfo(L, "Sln", &d);
		std::string err = lua_tostring(L, -1);
		lua_pop(L, 1);
		std::stringstream msg;
		msg << d.short_src << ":" << d.currentline;

		if (d.name != 0)
		{
			msg << "(" << d.namewhat << " " << d.name << ")";
		}
		msg << " " << err;
		lua_pushstring(L, msg.str().c_str());
		return 1;
	}
}
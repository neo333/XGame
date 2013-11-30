#ifndef __BINDER_CLASS_XGAME__HPP
#define __BINDER_CLASS_XGAME__HPP

#include <XGame/Script/luabind/luabind.hpp>

namespace xgame{
	template<class Class>
	class BinderClass{
	public:
		//! Costruttore. Specificare il nome di riferimento all'interno dello script per la classe.
		inline BinderClass(const char* name_class);

		//! Registra un costruttore con 1 parametro di ingresso.
		template<class TypePar> inline
			void RegisterCostructor();

		/*! Registra una funzione (funzione membro della classe).
			\param [in] name_funct			Nome di riferimento per la funzione.
			\param [in] ref_function		La funzione/metodo da registrare.
		*/
		template<class Fn> inline void RegisterFunction(const char* name_funct, Fn&& ref_function);

	private:
		luabind::class_<Class> m_ref_class;

		friend class Script;
	};

	template<class TypeClass> BinderClass<TypeClass>::BinderClass(const char* name_class)
		:m_ref_class(name_class)
	{ }

	template<class TypeClass> template<class Fn> void  BinderClass<TypeClass>::RegisterFunction(const char* name_funct, Fn&& ref_function){
		m_ref_class.def(name_funct, ref_function);
	}

	template<class TypeClass> template<class TypePar> void BinderClass<TypeClass>::RegisterCostructor(){
		m_ref_class.def(luabind::constructor<TypePar>());
	}
}

#endif
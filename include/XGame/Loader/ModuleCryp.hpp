#ifndef __MODULE_CRYP__HPP
#define __MODULE_CRYP__HPP

#include <XGame/Core/Config.hpp>
#include <boost/noncopyable.hpp>
#include <XGame/Core/Error.hpp>
#include <XGame/Loader/MemoryPage.hpp>
#include <string>

namespace xgame{
	class XGAME_API ModuleCryp: private boost::noncopyable{
	public:
		//! Costruttore di default
		ModuleCryp();


		/*! Setta la 'password' di cifratura/decifratura. Per decifrare una pagina occerrerà utilizzare la stessa 'passphrase'
			utilizzata per la cifratura. 
			N.B. Non è possibile recuperare la passpharase utilizzata! (Non esiste una funzione 'Get')
			@param [in]	passphrase		Una stringa qualsiasi che il modulo utilizzerà da ora in avanti per cifrare o decifrare
										le pagine.
										Immettere una stringa VUOTA per evitare operazioni di cifratura o decifratura!
		*/
		void Set_Passphrase(const std::string& passphrase) throw(Error);


		/*! Critta la pagina di memoria tramite la passphrase del modulo (immessa tramite l'apposito metodo).
			E' possibile inoltre crittografare anche solo una porzione di un'intera pagina di memoria tramite i parametri
			di 'offset' e 'len'.
			N.B. Se la passphrase immessa è una stringa vuota, allora questo metodo non farà niente!

			@param [in,out] io_page		La pagina che deve essere crittografata!
			@param [in]		offset		L'offset del byte da dove iniziare la crittografia (per una crittografia partizionata).
			@param [in]		len			Il numero di byte (partendo da 'off_set') che devono essere crittografati della pagina.
										Utilizzare il valore 'ModuleCryp::EndFile' per indicare 'fino alla fine della pagina'.
		*/
		void CrypMemoryPage(MemoryPage* io_page, size_t offset =0, size_t len =ModuleCryp::EndPage) const;


		/*! DeCritta la pagina di memoria tramite la passphrase del modulo (immessa tramite l'apposito metodo).
			E' possibile inoltre decrittografare anche solo una porzione di un'intera pagina di memoria tramite i parametri
			di 'offset' e 'len'.
			N.B. Se la passphrase immessa è una stringa vuota, allora questo metodo non farà niente!
			NOTA BENE: E' possibile decrittografare una pagina di memoria con lo STESSO contenuto col quale è stata crittografata!
						Questo significa che l'operazione di crittografia NON GODE DEL PRINCIPIO DI SOVRAPPOSIZIONE DEGLI EFFETTI!
							Esempio:
									A^= CRITTO[A];
									A = DECRITTO [A^];		ok!

									A^=CRITTO[A];	B^=CRITTO[B]; C=A^+B^
									DECRITTO[C] = DECRITTO[A^ + B^]  !=	DECRITTO[A^] + DECRITTO[B^]!!!		ATTENZIONE!!

			@param [in,out] io_page		La pagina che deve essere decrittografata!
			@param [in]		offset		L'offset del byte da dove iniziare la decrittografia (per una decrittografia partizionata)
			@param [in]		len			Il numero di byte (partendo da 'off_set') che devono essere decrittografati della pagina.
										Utilizzare il valore 'ModuleCryp::EndFile' per indicare 'fino alla fine della pagina'.
		*/
		void DeCrypMemoryPage(MemoryPage* io_page,size_t offset =0, size_t len = ModuleCryp::EndPage) const;


		//! Variabile statica costante che indica la fine del file.
		static const size_t EndPage;





	private:
		std::string m_keyCryp;
	};
}

#endif
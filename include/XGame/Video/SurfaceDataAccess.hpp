#ifndef __SURFACE_DATA_ACCESS__XGAME__HPP
#define __SURFACE_DATA_ACCESS__XGAME__HPP

#include <XGame/Video/Config.hpp>
#include <XGame/Video/Surface.hpp>
#include <stdexcept>

namespace xgame{
	class XGAME_API_VIDEO SurfaceDataAccess
	{
	public:
		/*! Costruttore.
			\param [in] ref_surface		Un riferimento ad una surface a cui accedere successivamente.
		*/
		SurfaceDataAccess(Surface& ref_surface) throw();

		//!	Distruttore. (Sblocca automaticamente la surface se bloccata in precedenza).
		~SurfaceDataAccess() throw();

		//! Costruttore di copia eliminato attualmente.
		SurfaceDataAccess(const SurfaceDataAccess&) = delete;

		//! Operatore di assegnazione eliminato attualmente.
		SurfaceDataAccess& operator=(const SurfaceDataAccess&) = delete;

		//! Costruttore di move eliminato attualmente.
		SurfaceDataAccess(SurfaceDataAccess&&) = delete;

		//! Operatore di move eliminato attualmente.
		SurfaceDataAccess& operator=(SurfaceDataAccess&&) = delete;

		/*! Blocca la surface a cui riferisce questo oggetto [indicata col costruttore].
			Una volta bloccata la surface con questo metodo è possibile utilizzare tutti quei metodi 
			ottimizzati per la lettura/scrittura rapida dei pixel della surface.

			\throw Error		In caso non sia possibile bloccare la surface.
		*/
		void LockSurface() throw(Error);

		/*! Sblocca la surface a cui riferisce questo oggetto [dopo averla bloccata con l'apposito metodo].
			\note		E' necessario sbloccare la surface dopo aver finito eventuali operazioni a basso livello
						sui pixel!
		*/
		void UnLockSurface() throw();

		//!	\return		'true' se la surface è bloccata, altrimenti 'false'.
		inline const bool Is_Locked() const throw();

		/*!	Ritorna il valore del pixel della surface alle coordinate 'x' 'y'.
			\param [in] x					La coordinata 'x' del pixel da interrogare.
			\param [in] y					La coordinata 'y' del pixel da interrogare.
			\param [out] output_color		Il valore del pixel di output.

			\note							E' necessario bloccare la surface prima di chiamare questo metodo!
			\see							SurfaceDataAccess::LockSurface()
		*/
		inline void GetPixelData_at(const size_t x, const size_t y, Color& output_color) const;

		/*! Setta il valore del pixel della surface alle coordinate 'x' 'y'.
			\param [in] x					La coordinata 'x' del pixel da settare.
			\param [in] y					La coordinata 'y' del pixel da settare.
			\param [in] input_color			Il valore del pixel di input.

			\note							E' necessario bloccare la surface prima di chiamare questo metodo!
			\see							SurfaceDataAccess::LockSurface()
		*/
		inline void SetPixelData_at(const size_t x, const size_t y, const Color& input_color);


	private:
		bool m_surface_locked = false;
		Surface* m_surface_ref = nullptr;
		const Uint32* m_dataPixel_read = nullptr;
		Uint32* m_dataPixel_write = nullptr;
	};

	inline const bool SurfaceDataAccess::Is_Locked() const throw()
	{
		if (m_surface_locked)
			return true;
		return false;
	}

	inline void SurfaceDataAccess::GetPixelData_at(const size_t x, const size_t y, Color& output_color) const
	{
		if (m_surface_locked == false)
			throw Error("SurfaceDataAccess", "GetPixelData_at", "Impossibile accedere a basso livello ad una surface non bloccata!");
		if (x<0 || x>=(size_t)m_surface_ref->m_surface->w || y<0 || y>=(size_t)m_surface_ref->m_surface->h)
			throw Error("SurfaceDataAccess", "GetPixelData_at", "Parametri out-of-range!");

		output_color = Color(m_dataPixel_read[x + y*m_surface_ref->m_surface->w]);
	}

	inline void SurfaceDataAccess::SetPixelData_at(const size_t x, const size_t y, const Color& input_color)
	{
		if (m_surface_locked == false)
			throw Error("SurfaceDataAccess", "GetPixelData_at", "Impossibile accedere a basso livello ad una surface non bloccata!");
		if (x<0 || x >= (size_t)m_surface_ref->m_surface->w || y<0 || y >= (size_t)m_surface_ref->m_surface->h)
			throw Error("SurfaceDataAccess", "GetPixelData_at", "Parametri out-of-range!");

		m_dataPixel_write[x + y*m_surface_ref->m_surface->w] = static_cast<Uint32>(input_color);
	}
}

#endif
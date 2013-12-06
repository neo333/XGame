#include <XGame/Video/SurfaceDataAccess.hpp>

namespace xgame{
	SurfaceDataAccess::SurfaceDataAccess(Surface& ref_surface) throw() :
		m_surface_ref(&ref_surface)
	{
	}

	SurfaceDataAccess::~SurfaceDataAccess() throw()
	{
		this->UnLockSurface();
	}

	void SurfaceDataAccess::LockSurface() throw(Error)
	{
		if (!m_surface_locked)
		{
			if (SDL_LockSurface(m_surface_ref->m_surface) != 0)
				throw Error("SurfaceDataAccess", "LockSurface", "Impossibile accedere ai dati della surface!\n%s", SDL_GetError());
			m_surface_locked = true;
			m_dataPixel_read = static_cast<const Uint32*>(m_surface_ref->m_surface->pixels);
			m_dataPixel_write = static_cast<Uint32*>(m_surface_ref->m_surface->pixels);
		}
	}
	void SurfaceDataAccess::UnLockSurface() throw()
	{
		if (m_surface_locked)
		{
			SDL_UnlockSurface(m_surface_ref->m_surface);
			m_surface_locked = false;
		}
	}
}
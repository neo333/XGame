#ifndef __NON_COPYABLE__XGAME__HPP
#define __NON_COPYABLE__XGAME__HPP

namespace xgame{
	class noncopyable{
	protected:
		noncopyable() = default;
		~noncopyable() = default;
		noncopyable(const noncopyable&) = delete;
		noncopyable& operator=(const noncopyable&) = delete;
	};
}

#endif
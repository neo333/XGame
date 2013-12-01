#include <XGame/Video/KeyPoints.hpp>

namespace xgame{

	std::vector<Point> KeyPoints::GetAs_VectPoint() const throw(){
		std::vector<Point> rts;
		rts.reserve(m_key.size()*4);
		for (auto& y:m_key)
		for (auto& x : y)
			rts.push_back(x);
		return rts;
	}
}
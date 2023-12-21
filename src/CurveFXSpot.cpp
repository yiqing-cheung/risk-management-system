#include "CurveFXSpot.h"
#include "Market.h"
#include "Streamer.h"

namespace minirisk {
    CurveFXSpot::CurveFXSpot(Market* mkt, const Date& today, const string& curve_name)
        : m_today(today)
        , m_name(curve_name) //curve_name : FX.SPOT.CCY1.CCY2
        , m_rate(mkt->get_fx_spot(curve_name.substr(fx_spot_prefix.length(), 3), curve_name.substr(fx_spot_prefix.length() + 4, 3)))
    {
    }

    double CurveFXSpot::spot() const
    {
        return m_rate;
    }

} // namespace minirisk
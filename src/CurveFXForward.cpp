#include "CurveFXForward.h"
#include "Market.h"
#include "CurveDiscount.h"
#include "CurveFXSpot.h"
#include "Streamer.h"

namespace minirisk {

    CurveFXForward::CurveFXForward(Market* mkt, const Date& today, const string& name)
        : m_mkt(mkt)
        , m_today(today)
        , m_name(name)
    {
    }

    double CurveFXForward::fwd(const Date& t) const
    {
        string m_ccy1 = m_name.substr(fx_forward_prefix.length(), 3);
        string m_ccy2 = m_name.substr(fx_forward_prefix.length()+4, 3);

        string m_fx_ccy = fx_spot_name(m_ccy1, m_ccy2); 
        const ptr_fx_spot_curve_t fxp = m_mkt->get_fx_spot_curve(m_fx_ccy);
        double ST_0 = fxp->spot();

        const ptr_disc_curve_t disc1 = m_mkt->get_discount_curve(ir_curve_discount_prefix + m_ccy1);
        double B_1 = disc1->df(t);

        const ptr_disc_curve_t disc2 = m_mkt->get_discount_curve(ir_curve_discount_prefix + m_ccy2);
        double B_2 = disc2->df(t);

        return ST_0*B_1/B_2;
    }

} // namespace minirisk
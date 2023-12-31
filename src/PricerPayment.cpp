#include "PricerPayment.h"
#include "TradePayment.h"
#include "CurveDiscount.h"

namespace minirisk {

PricerPayment::PricerPayment(const TradePayment& trd, const std::string& base_currency)
    : Pricer(base_currency)
    , m_amt(trd.quantity())
    , m_dt(trd.delivery_date())
    , m_ir_curve(ir_curve_discount_name(trd.ccy()))
    , m_fx_ccy(trd.ccy() == m_baseccy? "" : fx_spot_name(trd.ccy(), m_baseccy)) //prefix+ccy1+basecurrency
{
}

double PricerPayment::price(Market& mkt, const FixingDataServer& fds) const
{
    ptr_disc_curve_t disc = mkt.get_discount_curve(m_ir_curve);
    double df = disc->df(m_dt); // this throws an exception if m_dt<today

    // This PV is expressed in m_ccy. It must be converted in USD.
    if (!m_fx_ccy.empty()) {
        double fx_rate;
        fx_rate = mkt.get_fx_spot_curve(m_fx_ccy)->spot();
        df *= fx_rate;
    }
    return m_amt * df;
}

} // namespace minirisk



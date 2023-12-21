#include "PricerFXForward.h"
#include "TradeFXForward.h"
#include "CurveDiscount.h"
#include "CurveFXSpot.h"
#include "CurveFXForward.h"

namespace minirisk {

	PricerFXForward::PricerFXForward(const TradeFXForward& trd, const string& base_currency)
		: Pricer(base_currency)
		, m_amt(trd.quantity())
		, m_ft(trd.fixing_date())
		, m_st(trd.settle_date())
		, m_s(trd.strike())
		, m_ccy1(trd.ccy1())
		, m_ccy2(trd.ccy2())
	{
	}

	double PricerFXForward::price(Market& mkt, const FixingDataServer& fds) const
	{
		ptr_disc_curve_t discountCurve = mkt.get_discount_curve(ir_curve_discount_name(m_ccy2));
		if (!discountCurve) {
			throw std::runtime_error("Discount curve not found.");
		}
		double discountFactor = discountCurve->df(m_st); // throws an exception if m_st < today or m_st > max tenor

		ptr_fx_spot_curve_t FXSpotCurve = mkt.get_fx_spot_curve(fx_spot_name(m_ccy2, m_baseccy));
		if (!FXSpotCurve) {
			throw std::runtime_error("FX Spot curve not found.");
		}
		double spotPrice = FXSpotCurve->spot();
		double forwardPrice;
		//determine the appropriate forward price based on the market date
		if (mkt.today() > m_ft) {   //use fixing data when the fixing date is before today
			forwardPrice = fds.get(fx_spot_name(m_ccy1, m_ccy2), m_ft);
		}
		else {
			if (mkt.today() == m_ft) {   //use fixing data first, otherwise market data if the fixing date is today
				auto fixingData = fds.lookup(fx_spot_name(m_ccy1, m_ccy2), m_ft);
				if (fixingData.second)
					forwardPrice = fixingData.first;
				else {
					ptr_fx_spot_curve_t FXSpotCurve_fwd = mkt.get_fx_spot_curve(fx_spot_name(m_ccy1, m_ccy2));
					forwardPrice = FXSpotCurve_fwd->spot();
				}
			}
			else {  //use market data if the fixing date is in the future
				ptr_fx_forward_curve_t FXForwardCurve = mkt.get_fx_forward_curve(fx_forward_name(m_ccy1, m_ccy2));
				forwardPrice = FXForwardCurve->fwd(m_ft);
			}
		}

		return m_amt * discountFactor * (forwardPrice - m_s) * spotPrice; //calculate and return value of the payoff from the FX Forward trade
	}

} // namespace minirisk
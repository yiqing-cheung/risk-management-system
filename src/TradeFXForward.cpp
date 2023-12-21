#include "TradeFXForward.h"
#include "PricerFXForward.h"

namespace minirisk {
    //function to create a pricer for a TradeFXForward object
    ppricer_t TradeFXForward::pricer(const string& base_currency) const {
        //returning the pricer as a ppricer_t (pointer to PricerFXForward)
        return ppricer_t(new PricerFXForward(*this, base_currency));
    }

} // namespace minirisk
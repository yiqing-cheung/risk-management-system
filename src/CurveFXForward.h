#pragma once
#include "ICurve.h"

namespace minirisk {

    struct Market;

    struct CurveFXForward : ICurveFXForward
    {
        virtual string name() const { return m_name; }

        CurveFXForward(Market* mkt, const Date& today, const string& name); //fx_forward_prefix.ccy1.ccy2

        // compute the forward
        double fwd(const Date& t) const;

        virtual Date today() const { return m_today; }

    private:
        Market* m_mkt;
        Date m_today;
        string m_name;
    };

} // namespace minirisk


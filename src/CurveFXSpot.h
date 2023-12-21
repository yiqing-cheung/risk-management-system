#pragma once
#include "ICurve.h"

namespace minirisk {

    struct Market;

    struct CurveFXSpot : ICurveFXSpot
    {
        virtual string name() const { return m_name; }

        CurveFXSpot(Market* mkt, const Date& today, const string& curve_name);

        virtual Date today() const { return m_today; }

        //return fx spot price
        double spot() const;

    private:
        Date   m_today;
        string m_name;
        double m_rate;
    };

} // namespace minirisk
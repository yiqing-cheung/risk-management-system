#include "CurveDiscount.h"
#include "Market.h"
#include "Streamer.h"

#include <cmath>


namespace minirisk {
    /*
    CurveDiscount::CurveDiscount(Market *mkt, const Date& today, const string& curve_name)
        : m_today(today)
        , m_name(curve_name)
        , m_rate(mkt->get_yield(curve_name.substr(ir_curve_discount_prefix.length(),3)))
    {
    }
    */
    CurveDiscount::CurveDiscount(Market* mkt, const Date& today, const string& curve_name)
        : m_today(today)
        //, m_name(curve_name) 
        // curve_name now is actually the curve_pattern 
        , m_name(curve_name)
        , m_rate(mkt->get_yield(curve_name.substr(ir_curve_discount_prefix.length(), 3)))
    {
    }


    double  CurveDiscount::df(const Date& t) const
    {
        MYASSERT((!(t < m_today)), "Curve " << m_name << ", DF not available before anchor date " << m_today << ", requested " << t);
        MYASSERT((!(unsigned(t - m_today) > m_rate.rbegin()->first)), "Curve " << m_name << ", DF not available beyond last tenor date " << Date(m_today.get_serial() + m_rate.rbegin()->first) << ", requested " << t);
        /*

        |____|___________|________|_______|__________|
          m_today        T0       t       T1

        */
        auto find_T1 = m_rate.upper_bound(t - m_today);// find_T1->first and second extract the key and value, that the value is greater than t-m_day
        unsigned T1 = find_T1->first;
        double rT1 = find_T1->second;

        unsigned T0 = (--find_T1)->first; //decrement the iterator r, which gives the max value less than t-m_today. and extract the key 
        double rT0 = find_T1->second;
        unsigned dt = t - m_today;

        return std::exp(static_cast<double>(-rT0 - (rT1 - rT0) / (T1 - T0) * (dt - T0)) / 365.0);
        //T1 and TO are measured in days, thus for dt we do not use the time_frac function ,which returns days/365 
        //  need another assert 
    }

} // namespace minirisk

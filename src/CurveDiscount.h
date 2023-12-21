#pragma once
//preprocessor directive that ensures the file is included only once in a single 
//compilation, thus preventing duplicate definitions.
#include "ICurve.h"
#include <map>

namespace minirisk {

    struct Market;
    //tells the compiler that this struct will be defined later 
    struct CurveDiscount : ICurveDiscount
    {
        virtual string name() const { return m_name; }
        //a function that returns the name of the curve
        CurveDiscount(Market* mkt, const Date& today, const string& curve_name);

        // compute the discount factor
        double df(const Date& t) const;

        virtual Date today() const { return m_today; }



    private:
        Date   m_today;
        string m_name;
        //double m_rate;
        std::map<unsigned, double> m_rate;
        //Note: changed here, to make m_rate a map. 
    };

} // namespace minirisk

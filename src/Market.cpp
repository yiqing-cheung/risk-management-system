#include "Market.h"
#include "CurveDiscount.h"
#include "CurveFXSpot.h"
#include "CurveFXForward.h"

#include <regex>
#include <vector>
#include <limits>

namespace minirisk {

template <typename I, typename T>
std::shared_ptr<const I> Market::get_curve(const string& name)
{
    ptr_curve_t& curve_ptr = m_curves[name];
    if (!curve_ptr.get())
        curve_ptr.reset(new T(this, m_today, name));
    std::shared_ptr<const I> res = std::dynamic_pointer_cast<const I>(curve_ptr);
    MYASSERT(res, "Cannot cast object with name " << name << " to type " << typeid(I).name());
    return res;
}

const ptr_disc_curve_t Market::get_discount_curve(const string& name) //name: prefix+ccy
{
    return get_curve<ICurveDiscount, CurveDiscount>(name);
}

const ptr_fx_spot_curve_t Market::get_fx_spot_curve(const string& name) //name: prefix+ccy1+ccy2
{
    return get_curve<ICurveFXSpot, CurveFXSpot>(name);
}

const ptr_fx_forward_curve_t Market::get_fx_forward_curve(const string& name) //name: prefix+ccy1+ccy2
{
    return get_curve<ICurveFXForward, CurveFXForward>(name);
}

double Market::from_mds(const string& objtype, const string& name)
{
    auto ins = m_risk_factors.emplace(name, std::numeric_limits<double>::quiet_NaN());
    if (ins.second) { // just inserted, need to be populated
        MYASSERT(m_mds, "Cannot fetch " << objtype << " " << name << " because the market data server has been disconnnected");
        ins.first->second = m_mds->get(name);
    }
    return ins.first->second;
}

unsigned Market::regex_to_days(const string& name, const string& ccy)
{
    //stoi: converting a string to integer. 
    //Extract the number in the name 
    /*
    IR.1M.EUR
    |      |
    |      cyy
    prefix
    starting point : ir_rate_prefix.length()
    expand: name.length() - ccy.length() - ir_rate_prefix.length() - 2 (minus 2 because of the c and . before cyy)
    */
    unsigned p = std::stoi(name.substr(ir_rate_prefix.length(), name.length() - ccy.length() - ir_rate_prefix.length() - 2));

    unsigned char c = *(name.end() - ccy.length() - 2); //-2 because name.end() points to the next element of the last element. *(name.end()-1) is the last element in name.
    unsigned q = 0;
    switch (c) {
    case 'D': q = 1; break;
    case 'W': q = 7; break;
    case 'M': q = 30; break;
    case 'Y': q = 365; break;
    default: MYASSERT(c, "Cannot fetch yield curve because no such interest rate");
    }
    return p * q;
}

const std::map<unsigned, double> Market::get_yield(const string& ccy)
{
    string expr = ir_rate_prefix + "\\d+[DWMY]." + ccy;
    std::map<unsigned, double> rates;
    unsigned T;
    rates.emplace(0, 0.0);    //for tenor < smallest T
    // store T and rT 
    if (m_mds) {
        std::vector<std::string> names = m_mds->match(expr);
        for (const auto& n : names) {
            T = regex_to_days(n, ccy);
            rates.emplace(T, T * from_mds("yield curve", n));
        }
    }
    else {
        std::regex reg(expr);
        //std::cout << "no market dataserver exists" << std::endl;
        for (const auto& r : m_risk_factors) {
            if (std::regex_match(r.first, reg)) {
                T = regex_to_days(r.first, ccy);
                rates.emplace(T, T * from_mds("yield curve", r.first));
            }
        }
    }
    return rates;
}

const double Market::get_fx_spot(const string& ccy1_name, const string& ccy2_name)
{
    double ccy1, ccy2;

    if (ccy1_name == "USD") {
        ccy1 = 1.0;
    }
    else {
        ccy1 = from_mds("fx spot", fx_spot_prefix + ccy1_name);
    }

    if (ccy2_name == "USD") {
        ccy2 = 1.0;
    }
    else {
        ccy2 = from_mds("fx spot", fx_spot_prefix + ccy2_name);
    }

    return ccy1 / ccy2;
}


void Market::set_risk_factors(const vec_risk_factor_t& risk_factors)
{
    clear();
    for (const auto& d : risk_factors) {
        auto i = m_risk_factors.find(d.first);
        MYASSERT((i != m_risk_factors.end()), "Risk factor not found " << d.first);
        i->second = d.second;
    }
}

Market::vec_risk_factor_t Market::get_risk_factors(const std::string& expr) const
{
    vec_risk_factor_t result;
    std::regex r(expr);
    for (const auto& d : m_risk_factors)
        if (std::regex_match(d.first, r))
            result.push_back(d);
    return result;
}

} // namespace minirisk

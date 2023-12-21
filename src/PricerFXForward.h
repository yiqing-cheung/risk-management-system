#pragma once
#include "Pricer.h"
#include "TradeFXForward.h"

namespace minirisk {
    //implement the corresponding pricer object
    struct PricerFXForward : Pricer
    {
        PricerFXForward(const TradeFXForward& trd, const string& base_currency);
        virtual double price(Market& m, const FixingDataServer& fds) const;
        //calculate the price of the FX Forward trade

    private:
        double m_amt; //amount of the trade
        Date m_ft; //the date when the exchange rate is fixed
        Date m_st; //the date when the trade settles
        double m_s; //predefined strike price
        string m_ccy1;//first currency in the pair
        string m_ccy2;//second currency in the pair
    };

}

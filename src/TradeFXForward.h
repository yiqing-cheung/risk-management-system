#pragma once

#include "Trade.h"

namespace minirisk {

	struct TradeFXForward : Trade<TradeFXForward> {

		friend struct Trade<TradeFXForward>;

		static const guid_t m_id;
		static const std::string m_name;
		//method to create a pricer
		virtual ppricer_t pricer(const string& baseccy) const;
		//accessor methods for trade properties
		const string& ccy1() const { return m_ccy1; }
		const string& ccy2() const { return m_ccy2; }
		const Date& fixing_date() const { return m_fixing_date; }
		const Date& settle_date() const { return m_settle_date; }
		const double strike() const { return m_strike; }

	private: 
		void save_details(my_ofstream& os) const {
			//write trade details to an output stream
			os << m_ccy1 << m_ccy2 << m_strike << m_fixing_date << m_settle_date;
		}
		void load_details(my_ifstream& is) {
			//read trade details from an input stream
			is >> m_ccy1 >> m_ccy2 >> m_strike >> m_fixing_date >> m_settle_date;
		}
		void print_details(std::ostream& os) const {
			os << format_label("Strike level") << m_strike << std::endl;
			os << format_label("Base Currency") << m_ccy1 << std::endl;
			os << format_label("Quote Currency") << m_ccy2 << std::endl;
			os << format_label("Fixing Date") << m_fixing_date << std::endl;
			os << format_label("Settlement Date") << m_settle_date << std::endl;
		}

	private:
		string m_ccy1;
		string m_ccy2;
		double m_strike;
		Date m_fixing_date; //date when the exchange rate is fixed
		Date m_settle_date; //date when the trade is settled
	};

} // namespace minirisk


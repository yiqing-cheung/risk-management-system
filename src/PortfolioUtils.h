#pragma once

#include <vector>

#include "ITrade.h"
#include "IPricer.h"
#include "FixingDataServer.h"

namespace minirisk {

struct Market;

typedef std::vector<std::pair<double, string>> portfolio_values_t;
typedef std::pair<double, std::vector<std::pair<size_t, string>>> portfolio_total_t;

// get pricer for each trade
std::vector<ppricer_t> get_pricers(const portfolio_t& portfolio, const string& base_currency);

// compute prices
portfolio_values_t compute_prices(const std::vector<ppricer_t>& pricers, Market& mkt, const FixingDataServer& fds);

// compute the cumulative book value
portfolio_total_t portfolio_total(const portfolio_values_t& values);

// Compute PV01 (i.e. sensitivity with respect to interest rate dV/dr)
// Use central differences, absolute bump of 0.01%, rescale result for rate movement of 0.01%
//std::vector<std::pair<string, portfolio_values_t>> compute_pv01(const std::vector<ppricer_t>& pricers, const Market& mkt);

// save portfolio to file
void save_portfolio(const string& filename, const std::vector<ptrade_t>& portfolio);

// load portfolio from file
std::vector<ptrade_t>  load_portfolio(const string& filename);

// print portfolio to cout
void print_portfolio(const portfolio_t& portfolio);

// print portfolio to cout
void print_price_vector(const string& name, const portfolio_values_t& values);

std::vector<std::pair<string, portfolio_values_t>> compute_pv01_bucketed(const std::vector<ppricer_t>& pricers, const Market& mkt, const FixingDataServer& fds);

// Compute PV01 parallel(i.e. sensitivity with respect to interest rates moving at one time dV/dr)
// Use central differences, absolute bump of 0.01%, rescale result for rate movement of 0.01%
std::vector<std::pair<string, portfolio_values_t>> compute_pv01_parallel(const std::vector<ppricer_t>& pricers, const Market& mkt, const FixingDataServer& fds);

std::vector<std::pair<string, portfolio_values_t>> fx_delta(const std::vector<ppricer_t>& pricers, const Market& mkt, const FixingDataServer& fds);

} // namespace minirisk


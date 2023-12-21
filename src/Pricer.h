#pragma once

#include <memory>

#include "IPricer.h"

namespace minirisk {

	struct Pricer : IPricer {
		protected: //make it avaiable to subclasses 
			Pricer(const string& base_currency) : m_baseccy(base_currency) {}
			string m_baseccy;

	};
}

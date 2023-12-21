#pragma once

#include "Macros.h"
#include <string>
#include <array>

namespace minirisk {

    struct Date
    {
    public:
        static const unsigned first_year = 1900;
        static const unsigned last_year = 2200;
        static const unsigned n_years = last_year - first_year;

    private:
        static std::string padding_dates(unsigned);

        // number of days elapsed from beginning of the year
        unsigned day_of_year() const;

        friend long operator-(const Date& d1, const Date& d2);

        static const std::array<unsigned, 12> days_in_month;  // num of days in month M in a normal year
        static const std::array<unsigned, 12> days_ytd;      // num of days since 1-jan to 1-M in a normal year
        static const std::array<unsigned, 12> days_leap_ytd;
        static const std::array<unsigned, n_years> days_epoch;   // num of days since 1-jan-1900 to 1-jan-yyyy (until 2200)
        unsigned serial; //days from 1900-1-1

    public:
        // Default constructor
        Date() : serial(25567) {}

        // Constructor where the input value is checked.
        Date(unsigned year, unsigned month, unsigned day)
        {
            init(year, month, day);
        }

        void init(unsigned year, unsigned month, unsigned day)
        {
            check_valid(year, month, day);
            serial = day + (is_leap_year(year) ? days_leap_ytd[month - 1] : days_ytd[month - 1]);
            unsigned leap_year = (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400
                - (1900 - 1) / 4 + (1900 - 1) / 100 - (1900 - 1) / 400;
            serial += (year - 1900) * 365 + leap_year - 1;
        }

        Date(unsigned serial_in)
        {
            init(serial_in);
        }

        void init(unsigned serial_in)
        {
            check_valid(serial_in);
            serial = serial_in;
        }

        static void check_valid(unsigned y, unsigned m, unsigned d);
        static void check_valid(unsigned s);

        bool operator<(const Date& d) const
        {
            return serial < d.serial;
        }

        bool operator==(const Date& d) const
        {
            return serial == d.serial;
        }

        bool operator>(const Date& d) const
        {
            return serial > d.serial;
        }

        // number of days since 1-Jan-1900
        unsigned get_serial() const
        {
            return serial;
        }

        static bool is_leap_year(unsigned yr);

        // In YYYYMMDD format
        std::string to_string(bool pretty = true) const
        {
            unsigned day = serial;
            unsigned year = 1900;
            while (day >= 366) {
                day -= is_leap_year(year) ? 366 : 365;
                year++;
            }
            if (!is_leap_year(year) && day >= 365) year++, day -= 365;
            day++;
            const std::array<unsigned, 12>& days_y = is_leap_year(year) ? days_leap_ytd : days_ytd;
            unsigned month = day > days_y[11] ? 12 : static_cast<unsigned>(std::find_if(std::begin(days_y), std::end(days_y), [day](unsigned i) { return i >= day; }) - std::begin(days_y));

            day -= days_y[month - 1];
            return pretty
                ? std::to_string((int)day) + "-" + std::to_string((int)month) + "-" + std::to_string(year)
                : std::to_string(year) + padding_dates((int)month) + padding_dates((int)day);
        }

    };

    long operator-(const Date& d1, const Date& d2);

    inline double time_frac(const Date& d1, const Date& d2)
    {
        return static_cast<double>(d2 - d1) / 365.0;
    }

} // namespace minirisk

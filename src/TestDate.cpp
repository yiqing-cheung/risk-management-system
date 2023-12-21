#include<cstdlib>
#include<ctime>
#include<iostream>
#include<random>
#include "Date.h"
using namespace minirisk;

const std::array<unsigned, 12> days_in_month = { {31,28,31,30,31,30,31,31,30,31,30,31} };

void test1()
{
    bool issuccess = true;
    const int test_cases = 1000;
    std::random_device rd;
    int random_seed = rd();
    std::srand(random_seed);

    for (int i = 0; i < test_cases; ++i) {
        try {
            unsigned year = std::rand() % (Date::last_year - Date::first_year) + Date::first_year;
            unsigned month = (std::rand() % 12) + 1;
            unsigned day = 1;

            switch (i % 3) {
            case 0:
                year = (i % 6 == 0) ? std::rand() % Date::first_year : std::rand() % 1000 + Date::last_year;
            case 1:
                month = std::rand() % 10 + 13;
            case 2:
                day = std::rand() % 10 + days_in_month[month - 1] + 1;
                if (month == 2 && Date::is_leap_year(year))
                    day++;
            }
            Date::check_valid(year, month, day);
            issuccess = false;
            std::cout << "Test failed for date " << day << "-" << month << "-" << year << std::endl;
            break;
        }
        catch (const std::exception& e) {
        }
    }
    if(issuccess)
        std::cout << "Test1 SUCCESS " << std::endl;
}

void test2()
{
    try {
        for (unsigned year = Date::first_year; year < Date::last_year; year++)
            for (unsigned month = 1; month <= 12; month++) {
                unsigned day_max = (month == 2 && Date::is_leap_year(year)) ? 29 : days_in_month[month - 1];
                for (unsigned day = 1; day <= day_max; day++) {
                    Date date_test = Date(year, month, day);
                    std::string original_date = std::to_string(day) + '-' + std::to_string(month) + '-' + std::to_string(year);
                    std::string calendar_format = date_test.to_string();
                    MYASSERT(original_date == calendar_format, "Test failed for date " << day << "-" << month << "-" << year << ", got " << calendar_format);
                }
            }
        std::cout << "Test2 SUCCESS " << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void test3()
{
    try {
        Date pre_date = Date(1900, 1, 1);
        for (unsigned year = Date::first_year; year < Date::last_year; year++)
            for (unsigned month = 1; month <= 12; month++) {
                unsigned day_max = (month == 2 && Date::is_leap_year(year)) ? 29 : days_in_month[month - 1];
                for (unsigned day = 1; day <= day_max; day++) {
                    if (year == 1900 && month == 1 && day == 1) continue;
                    Date date_test = Date(year, month, day);
                    MYASSERT(date_test.get_serial() - pre_date.get_serial() == 1,
                        "Test failed between date " << day << "-" << month << "-" << year << " and the previous date");
                    pre_date = date_test;
                }
            }
        std::cout << "Test3 SUCCESS " << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

int main()
{
    test1();
    test2();
    test3();
    return 0;
}


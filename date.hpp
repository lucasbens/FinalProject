#ifndef DATE_HPP
#define DATE_HPP

#include "features_definition_loan.hpp"
#include <string>

class Date
{
public:
      Date() : m_xMonth(), m_uiYear() {}
      Date(const Month &xMonth, const Year &xYear) : m_xMonth(xMonth), m_uiYear(xYear.get()) {}
      Date(const Date &xDate) : m_xMonth(xDate.m_xMonth), m_uiYear(xDate.m_uiYear) {}

      void operator=(const Date &xDate)
      {
         m_xMonth = xDate.m_xMonth;
         m_uiYear = xDate.m_uiYear;
      }

      void operator+=(unsigned int uiIncrementMonth);
      void increment(const FrequencyPayments &xFrequencyPayments);

      std::string printStandardFormat() const;

private:
      Month        m_xMonth;
      unsigned int m_uiYear;

      unsigned int convertMonthIntoIndex(const Month &xMonth) const;
      Month convertIndexIntoMonth(unsigned int uiIndex) const;
};

#endif // DATE_HPP

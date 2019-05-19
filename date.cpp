#include "date.hpp"

unsigned int Date::convertMonthIntoIndex(const Month &xMonth) const
{
    unsigned int uiIndex = 0;

    if (xMonth == Month::february)
        uiIndex = 1;
    else if (xMonth == Month::march)
        uiIndex = 2;
    else if (xMonth == Month::april)
        uiIndex = 3;
    else if (xMonth == Month::may)
        uiIndex = 4;
    else if (xMonth == Month::june)
        uiIndex = 5;
    else if (xMonth == Month::july)
        uiIndex = 6;
    else if (xMonth == Month::august)
        uiIndex = 7;
    else if (xMonth == Month::september)
        uiIndex = 8;
    else if (xMonth == Month::october)
        uiIndex = 9;
    else if (xMonth == Month::november)
        uiIndex = 10;
    else if (xMonth == Month::december)
        uiIndex = 11;

    return uiIndex;
}

Month Date::convertIndexIntoMonth(unsigned int uiIndex) const
{
    Month xMonth = Month::january;
    unsigned int uiIndexAdjusted = uiIndex % 12;

    if (uiIndexAdjusted == 1)
        xMonth = Month::february;
    else if (uiIndexAdjusted == 2)
        xMonth = Month::march;
    else if (uiIndexAdjusted == 3)
        xMonth = Month::april;
    else if (uiIndexAdjusted == 4)
        xMonth = Month::may;
    else if (uiIndexAdjusted == 5)
        xMonth = Month::june;
    else if (uiIndexAdjusted == 6)
        xMonth = Month::july;
    else if (uiIndexAdjusted == 7)
        xMonth = Month::august;
    else if (uiIndexAdjusted == 8)
        xMonth = Month::september;
    else if (uiIndexAdjusted == 9)
        xMonth = Month::october;
    else if (uiIndexAdjusted == 10)
        xMonth = Month::november;
    else if (uiIndexAdjusted == 11)
        xMonth = Month::december;

    return xMonth;
}

void Date::operator+=(unsigned int uiIncrementMonth)
{
    unsigned int uiIndexCurrentMonth = convertMonthIntoIndex(m_xMonth);
    unsigned int uiJumpNbYears = (uiIndexCurrentMonth + uiIncrementMonth) / 12;
    m_uiYear += uiJumpNbYears;
    m_xMonth = convertIndexIntoMonth((uiIndexCurrentMonth + uiIncrementMonth) % 12);
}

void Date::increment(const FrequencyPayments &xFrequencyPayments)
{
    if (xFrequencyPayments == FrequencyPayments::yearly)
        *this += 12;
    else if (xFrequencyPayments == FrequencyPayments::semi_annually)
        *this += 6;
    else if (xFrequencyPayments == FrequencyPayments::quarterly)
        *this += 3;
    else if (xFrequencyPayments == FrequencyPayments::monthly)
        *this += 1;
}

std::string Date::printStandardFormat() const
{
    std::string sDate;
    unsigned int uiIndexMonth = convertMonthIntoIndex(m_xMonth) + 1;

    if (uiIndexMonth < 10)
        sDate += "0" + std::to_string(uiIndexMonth);
    else //if (uiIndexMonth < 13)
        sDate += std::to_string(uiIndexMonth);

    sDate += "/" + std::to_string(m_uiYear);

    return sDate;
}

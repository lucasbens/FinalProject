#ifndef FEATURES_DEFINITION_LOAN_HPP
#define FEATURES_DEFINITION_LOAN_HPP

#include <utility>

#define MY_PI 3.14159265358979323846

enum class Month { january, february, march, april, may, june, july, august, september, october, november, december };

enum class FrequencyPayments { yearly, semi_annually, quarterly, monthly };

template <typename T, typename Parameter>
class NamedType
{
public:
    explicit NamedType(T const& xValue) : m_xValue(xValue) {}
    explicit NamedType(T&& xValue) : m_xValue(std::move(xValue)) {}

    T& get() { return m_xValue; }
    T const& get() const { return m_xValue; }

    void set(T xValue) { m_xValue = xValue; }

private:
    T m_xValue;
};

using Year = NamedType<unsigned int, struct YearParameter>;

#endif // FEATURES_DEFINITION_LOAN_HPP

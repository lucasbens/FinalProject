#ifndef LOAN_HPP
#define LOAN_HPP

#include "features_definition_loan.hpp"
#include <string>
#include "date.hpp"
#include <vector>
#include <utility>

enum class TypeLoan { fixedRate, floatingRate };

using Principal = NamedType<double, struct PrincipalParameter>;
using CostLoan = NamedType<double, struct CostLoanParameter>;
using YearlyFixedRate = NamedType<double, struct YearlyFixedRateParameter>;
using YearlyMarginRate = NamedType<double, struct YearlyMarginRateParameter>;
using StandardDeviation = NamedType<double, struct StandardDeviationParameter>;
using NbPayments = NamedType<unsigned int, struct NbPaymentsParameter>;
using FixedInstalment = NamedType<double, struct FixedInstalmentParameter>;
using SeedPseudoRandomGenerator = NamedType<int, struct SeedPseudoRandomGeneratorParameter>;

class ILoanBaseClass
{
public:
   ILoanBaseClass(const Principal &xPrincipal, const Month &xMonthFirstPayment, const Year &xYearFirstPayment, const FrequencyPayments &xFrequencyPayments, const NbPayments &xNbPayments) : m_dPrincipal(xPrincipal.get()), m_xDateFirstPayment(xMonthFirstPayment, xYearFirstPayment), m_xFrequencyPayments(xFrequencyPayments), m_uiNbPayments(xNbPayments.get()) {}

    virtual void printCSV(const std::string &sNameFileCSV) const = 0;

protected:
    double            m_dPrincipal;
    Date              m_xDateFirstPayment;
    FrequencyPayments m_xFrequencyPayments;
    unsigned int      m_uiNbPayments;
};

class LoanFixedRate : public ILoanBaseClass
{
public:
    LoanFixedRate(const Principal &xPrincipal, const CostLoan &xCostLoan, const YearlyFixedRate &xYearlyFixedRate, const Month &xMonthFirstPayment, const Year &xYearFirstPayment, const FrequencyPayments &xFrequencyPayments, const FixedInstalment &xFixedInstalment);
    LoanFixedRate(const Principal &xPrincipal, const CostLoan &xCostLoan, const YearlyFixedRate &xYearlyFixedRate, const Month &xMonthFirstPayment, const Year &xYearFirstPayment, const FrequencyPayments &xFrequencyPayments, const NbPayments &xNbPayments);
    LoanFixedRate(const Principal &xPrincipal, const CostLoan &xCostLoan, const Month &xMonthFirstPayment, const Year &xYearFirstPayment, const FrequencyPayments &xFrequencyPayments, const NbPayments &xNbPayments, const FixedInstalment &xFixedInstalment);
    LoanFixedRate(const CostLoan &xCostLoan, const YearlyFixedRate &xYearlyFixedRate, const Month &xMonthFirstPayment, const Year &xYearFirstPayment, const FrequencyPayments &xFrequencyPayments, const NbPayments &xNbPayments, const FixedInstalment &xFixedInstalment);

    void printCSV(const std::string &sNameFileCSV) const;

private:
    double m_dGlobalRateOfLoan; // represents cost of loan : e.g, a loan of 1000.00 USD at rate 10.00 % has a cost equal to 1000.00 x 10.00 % = 100 USD
    double m_dYearlyFixedRate;
    double m_dFixedRateBasedOnFrequency;
    double m_dFixedInstalment;

    void convertYearlyRateIntoRateBasedOnFrequency();
    void convertRateBasedOnFrequencyIntoYearlyRate();
    void FindFixedRateBasedOnFrequencyDichotomy(double dPrecision = 1.e-10);
};

class LoanFloatingRate : public ILoanBaseClass
{
public:
    LoanFloatingRate(const Principal &xPrincipal, const YearlyMarginRate &xYearlyMarginRate, const StandardDeviation &xStandardDeviationBenchmarkRates, const SeedPseudoRandomGenerator &xSeedPseudoRandomGenerator, const Month &xMonthFirstPayment, const Year &xYearFirstPayment, const FrequencyPayments &xFrequencyPayments, const NbPayments &xNbPayments);

    void printCSV(const std::string &sNameFileCSV) const;

private:
    double              m_dYearlyMarginRate;
    double              m_dMarginRateBasedOnFrequency;
    double              m_dStandardDeviationBenchmarkRates;
    int                 m_iSeedPseudoRandomGenerator;
    std::vector<double> m_vdYearlyBenchmarkRates;
    std::vector<double> m_vdBenchmarkRatesBasedOnFrequency;
    std::vector<double> m_vdFloatingRatesBasedOnFrequency;

    std::pair<double, double> sampleNormalLawBoxMuller() const;
    void convertYearlyRatesIntoRatesBasedOnFrequency();
};

double ChoosePrincipal();
double ChooseCostOfLoan();
double ChooseYearlyFixedRate(const TypeLoan &xTypeLoan);
double ChooseFixedInstalment();
unsigned int ChooseNumberPayments();
double ChooseVolatilityYearlyBenchmarkRates();

#endif // LOAN_HPP

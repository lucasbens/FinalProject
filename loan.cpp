#include "loan.hpp"
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <utility>
#include <cstdlib>

void LoanFixedRate::convertYearlyRateIntoRateBasedOnFrequency()
{
    if (m_xFrequencyPayments == FrequencyPayments::yearly)
        m_dFixedRateBasedOnFrequency = m_dYearlyFixedRate;
    else if (m_xFrequencyPayments == FrequencyPayments::semi_annually)
        m_dFixedRateBasedOnFrequency = m_dYearlyFixedRate / 2.;
    else if (m_xFrequencyPayments == FrequencyPayments::quarterly)
        m_dFixedRateBasedOnFrequency = m_dYearlyFixedRate / 4.;
    else if (m_xFrequencyPayments == FrequencyPayments::monthly)
        m_dFixedRateBasedOnFrequency = m_dYearlyFixedRate / 12.;
}

void LoanFixedRate::convertRateBasedOnFrequencyIntoYearlyRate()
{
    if (m_xFrequencyPayments == FrequencyPayments::yearly)
        m_dYearlyFixedRate = m_dFixedRateBasedOnFrequency;
    else if (m_xFrequencyPayments == FrequencyPayments::semi_annually)
        m_dYearlyFixedRate = m_dFixedRateBasedOnFrequency * 2.;
    else if (m_xFrequencyPayments == FrequencyPayments::quarterly)
        m_dYearlyFixedRate = m_dFixedRateBasedOnFrequency * 4.;
    else if (m_xFrequencyPayments == FrequencyPayments::monthly)
        m_dYearlyFixedRate = m_dFixedRateBasedOnFrequency * 12.;
}

LoanFixedRate::LoanFixedRate(const Principal &xPrincipal, const CostLoan &xCostLoan, const YearlyFixedRate &xYearlyFixedRate, const Month &xMonthFirstPayment, const Year &xYearFirstPayment, const FrequencyPayments &xFrequencyPayments, const FixedInstalment &xFixedInstalment) : ILoanBaseClass(xPrincipal, xMonthFirstPayment, xYearFirstPayment, xFrequencyPayments, NbPayments(0)), m_dGlobalRateOfLoan(xCostLoan.get()), m_dYearlyFixedRate(xYearlyFixedRate.get()), m_dFixedRateBasedOnFrequency(), m_dFixedInstalment(xFixedInstalment.get())
{
    convertYearlyRateIntoRateBasedOnFrequency();

    double dNbPayments = -log(1. - m_dFixedRateBasedOnFrequency * m_dPrincipal / m_dFixedInstalment) / log(1. + m_dFixedRateBasedOnFrequency);

    if (dNbPayments == floor(dNbPayments))
        m_uiNbPayments = static_cast<unsigned int>(dNbPayments);
    else
        m_uiNbPayments = static_cast<unsigned int>(dNbPayments) + 1;
}

LoanFixedRate::LoanFixedRate(const Principal &xPrincipal, const CostLoan &xCostLoan, const YearlyFixedRate &xYearlyFixedRate, const Month &xMonthFirstPayment, const Year &xYearFirstPayment, const FrequencyPayments &xFrequencyPayments, const NbPayments &xNbPayments) : ILoanBaseClass(xPrincipal, xMonthFirstPayment, xYearFirstPayment, xFrequencyPayments, xNbPayments), m_dGlobalRateOfLoan(xCostLoan.get()), m_dYearlyFixedRate(xYearlyFixedRate.get()), m_dFixedRateBasedOnFrequency(), m_dFixedInstalment()
{
    convertYearlyRateIntoRateBasedOnFrequency();
    m_dFixedInstalment = m_dPrincipal * (1. + m_dGlobalRateOfLoan) / static_cast<double>(m_uiNbPayments);
}

void LoanFixedRate::FindFixedRateBasedOnFrequencyDichotomy(double dPrecision /*expected to be > 0.*/)
{
    double dNbPayments = static_cast<double>(m_uiNbPayments);
    double dBoundInf = -0.5;

    // find an adequate sup boundary to define an interval [a ; b] on which dichotomy could be launched with a successful end
    double dBoundSup = 1.; // arbitrary initial value
    double dTermPow = pow(1. + dBoundSup, dNbPayments);

    while (dBoundSup * dTermPow / (dTermPow - 1.) - m_dFixedInstalment / m_dPrincipal <= 0.)
    {
        dBoundSup = 2 * dBoundSup + 10.; // + 10. arbitrarily chosen
        dTermPow = pow(1. + dBoundSup, dNbPayments);
    }

    double dPrecisionAdjusted = std::min<double>(fabs(dPrecision), 1.e-10);

    while (dBoundSup - dBoundInf > dPrecisionAdjusted)
    {
        double dMiddle = (dBoundInf + dBoundSup) / 2.;
        dTermPow = pow(1. + dMiddle, dNbPayments);

        if (dMiddle * dTermPow / (dTermPow - 1.) - m_dFixedInstalment / m_dPrincipal > 0.)
           dBoundSup = dMiddle;
        else
           dBoundInf = dMiddle;
    }

    m_dFixedRateBasedOnFrequency = dBoundInf;
}

LoanFixedRate::LoanFixedRate(const Principal &xPrincipal, const CostLoan &xCostLoan, const Month &xMonthFirstPayment, const Year &xYearFirstPayment, const FrequencyPayments &xFrequencyPayments, const NbPayments &xNbPayments, const FixedInstalment &xFixedInstalment) : ILoanBaseClass(xPrincipal, xMonthFirstPayment, xYearFirstPayment, xFrequencyPayments, xNbPayments), m_dGlobalRateOfLoan(xCostLoan.get()), m_dYearlyFixedRate(), m_dFixedRateBasedOnFrequency(), m_dFixedInstalment(xFixedInstalment.get())
{
    FindFixedRateBasedOnFrequencyDichotomy(1.e-15);
    convertRateBasedOnFrequencyIntoYearlyRate();
}

LoanFixedRate::LoanFixedRate(const CostLoan &xCostLoan, const YearlyFixedRate &xYearlyFixedRate, const Month &xMonthFirstPayment, const Year &xYearFirstPayment, const FrequencyPayments &xFrequencyPayments, const NbPayments &xNbPayments, const FixedInstalment &xFixedInstalment) : ILoanBaseClass(Principal(0.), xMonthFirstPayment, xYearFirstPayment, xFrequencyPayments, xNbPayments), m_dGlobalRateOfLoan(xCostLoan.get()), m_dYearlyFixedRate(xYearlyFixedRate.get()), m_dFixedRateBasedOnFrequency(), m_dFixedInstalment(xFixedInstalment.get())
{
    convertYearlyRateIntoRateBasedOnFrequency();
    m_dPrincipal = m_dFixedInstalment / (1. + m_dGlobalRateOfLoan) * static_cast<double>(m_uiNbPayments);
}

void LoanFixedRate::printCSV(const std::string &sNameFileCSV) const
{
    std::ofstream ofFileCSV;
    ofFileCSV.open(sNameFileCSV.empty() == false ? sNameFileCSV.c_str() : "schedule_loan_fixed_rate.csv", std::ios::out | std::ios::trunc);

    if (ofFileCSV)
    {
        ofFileCSV << std::fixed << std::setprecision(15);

        ofFileCSV << ";;yearly rate;R;" << m_dYearlyFixedRate * 100. << "%\n";
        ofFileCSV << "#;date;opening balance;principal;interest rate;interest;instalment;closing balance\n";
        ofFileCSV << "i;t(i);PV(i-1);P(i);r(i);int(i);PMT(i);PV(i)\n";

        Date xDatePayment = m_xDateFirstPayment;
        double dOpeningBalance = m_dPrincipal;
        double dSumPrincipalPayments = 0.;

        for (unsigned int uiPayment = 0; uiPayment < m_uiNbPayments; ++uiPayment)
        {
            double dInterest = dOpeningBalance * m_dFixedRateBasedOnFrequency;
            double dPrincipalPayment = m_dFixedInstalment - dInterest;
            double dClosingBalance = dOpeningBalance - dPrincipalPayment;

            if (fabs(dClosingBalance) <= 1.e-08)
               dClosingBalance = 0.;

            ofFileCSV << uiPayment + 1 << ";" << xDatePayment.printStandardFormat().c_str() << ";" << dOpeningBalance << ";" << dPrincipalPayment << ";" << m_dFixedRateBasedOnFrequency * 100. << "%;" << dInterest << ";" << m_dFixedInstalment << ";" << dClosingBalance << "\n";

            dOpeningBalance = dClosingBalance;
            dSumPrincipalPayments += dPrincipalPayment;
            xDatePayment.increment(m_xFrequencyPayments);
        }

        ofFileCSV << ";;TOTAL;" << dSumPrincipalPayments << "\n";

        ofFileCSV.close();
    }
    else
        std::cout << "Failure in opening file " << sNameFileCSV << " => operation aborted !" << std::endl;
}

LoanFloatingRate::LoanFloatingRate(const Principal &xPrincipal, const YearlyMarginRate &xYearlyMarginRate, const StandardDeviation &xStandardDeviationBenchmarkRates, const SeedPseudoRandomGenerator &xSeedPseudoRandomGenerator, const Month &xMonthFirstPayment, const Year &xYearFirstPayment, const FrequencyPayments &xFrequencyPayments, const NbPayments &xNbPayments) : ILoanBaseClass(xPrincipal, xMonthFirstPayment, xYearFirstPayment, xFrequencyPayments, xNbPayments), m_dYearlyMarginRate(xYearlyMarginRate.get()), m_dMarginRateBasedOnFrequency(), m_dStandardDeviationBenchmarkRates(xStandardDeviationBenchmarkRates.get()), m_iSeedPseudoRandomGenerator(xSeedPseudoRandomGenerator.get()), m_vdYearlyBenchmarkRates(), m_vdBenchmarkRatesBasedOnFrequency(), m_vdFloatingRatesBasedOnFrequency()
{
    // simulate sequence of (yearly) benchmark rates
    srand(m_iSeedPseudoRandomGenerator);

    for (unsigned int uiBenchmarkRate = 0; uiBenchmarkRate < m_uiNbPayments; uiBenchmarkRate += 2)
    {
        std::pair<double, double> xPairYearlyBenchmarkRates = sampleNormalLawBoxMuller();

        m_vdYearlyBenchmarkRates.push_back(std::max(xPairYearlyBenchmarkRates.first, 0.));


        if (uiBenchmarkRate + 1 < m_uiNbPayments)
            m_vdYearlyBenchmarkRates.push_back(std::max(xPairYearlyBenchmarkRates.second, 0.));
    }

    convertYearlyRatesIntoRatesBasedOnFrequency();
}

std::pair<double, double> LoanFloatingRate::sampleNormalLawBoxMuller() const
{
    double dU1 = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    double dU2 = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    double dFactor = sqrt(-2. * log(dU1));
    double dZ1 = dFactor * cos(2. * MY_PI * dU2);
    double dZ2 = dFactor * sin(2. * MY_PI * dU2);

    return std::pair<double, double>(m_dYearlyMarginRate + m_dStandardDeviationBenchmarkRates * dZ1, m_dYearlyMarginRate + m_dStandardDeviationBenchmarkRates * dZ2);
}

void LoanFloatingRate::convertYearlyRatesIntoRatesBasedOnFrequency()
{
    if (m_xFrequencyPayments == FrequencyPayments::semi_annually)
       m_dMarginRateBasedOnFrequency = m_dYearlyMarginRate / 2.;
    else if (m_xFrequencyPayments == FrequencyPayments::quarterly)
       m_dMarginRateBasedOnFrequency = m_dYearlyMarginRate / 4.;
    else if (m_xFrequencyPayments == FrequencyPayments::monthly)
       m_dMarginRateBasedOnFrequency = m_dYearlyMarginRate / 12.;

    for (unsigned int uiBenchmarkRate = 0; uiBenchmarkRate < m_uiNbPayments; ++uiBenchmarkRate)
    {
        double dYearlyBenchmarkRate = m_vdYearlyBenchmarkRates[uiBenchmarkRate];
        double dBenchmarkRateBasedOnFrequency = dYearlyBenchmarkRate;
        double dFloatingRateBasedOnFrequency = m_dYearlyMarginRate + dYearlyBenchmarkRate;

        if (m_xFrequencyPayments == FrequencyPayments::semi_annually)
        {
           dBenchmarkRateBasedOnFrequency /= 2.;
           dFloatingRateBasedOnFrequency /= 2.;
        }
        else if (m_xFrequencyPayments == FrequencyPayments::quarterly)
        {
           dBenchmarkRateBasedOnFrequency /= 4.;
           dFloatingRateBasedOnFrequency /= 4.;
        }
        else if (m_xFrequencyPayments == FrequencyPayments::monthly)
        {
           dBenchmarkRateBasedOnFrequency /= 12.;
           dFloatingRateBasedOnFrequency /= 12.;
        }

        m_vdBenchmarkRatesBasedOnFrequency.push_back(dBenchmarkRateBasedOnFrequency);
        m_vdFloatingRatesBasedOnFrequency.push_back(dFloatingRateBasedOnFrequency);
    }
}

void LoanFloatingRate::printCSV(const std::string &sNameFileCSV) const
{
    std::ofstream ofFileCSV;
    ofFileCSV.open(sNameFileCSV.empty() == false ? sNameFileCSV.c_str() : "schedule_loan_floating_rates.csv", std::ios::out | std::ios::trunc);

    if (ofFileCSV)
    {
        ofFileCSV << std::fixed << std::setprecision(15);

        ofFileCSV << "#;date;opening balance;principal;margin;benchmark rate;interest rate;interest;instalment;closing balance\n";
        ofFileCSV << "i;t(i);PV(i-1);P(i);m;b(i);r(i);int(i);PMT(i);PV(i)\n";

        double dPrincipalPayment = m_dPrincipal / static_cast<double>(m_uiNbPayments);
        Date xDatePayment = m_xDateFirstPayment;
        double dOpeningBalance = m_dPrincipal;
        double dSumPrincipalPayments = 0.;

        for (unsigned int uiPayment = 0; uiPayment < m_uiNbPayments; ++uiPayment)
        {
            double dInterest = dOpeningBalance * m_vdFloatingRatesBasedOnFrequency[uiPayment];
            double dInstalment = dPrincipalPayment + dInterest;
            double dClosingBalance = dOpeningBalance - dPrincipalPayment;

            if (fabs(dClosingBalance) <= 1.e-08)
               dClosingBalance = 0.;

            ofFileCSV << uiPayment + 1 << ";" << xDatePayment.printStandardFormat().c_str() << ";" << dOpeningBalance << ";" << dPrincipalPayment << ";" << m_dMarginRateBasedOnFrequency * 100. << "%;" << m_vdBenchmarkRatesBasedOnFrequency[uiPayment] * 100. << "%;" << m_vdFloatingRatesBasedOnFrequency[uiPayment] * 100. << "%;" << dInterest << ";" << dInstalment << ";" << dClosingBalance << "\n";

            dOpeningBalance = dClosingBalance;
            dSumPrincipalPayments += dPrincipalPayment;
            xDatePayment.increment(m_xFrequencyPayments);
        }

        ofFileCSV << ";;TOTAL;" << dSumPrincipalPayments << "\n";

        ofFileCSV.close();
    }
    else
        std::cout << "Failure in opening file " << sNameFileCSV << " => operation aborted !" << std::endl;
}

double ChoosePrincipal()
{
    std::cout << "Enter principal of loan (value > 0. expected) :\n";

    double dPrincipal = 0.;
    std::cin >> dPrincipal;

    while (dPrincipal <= 0.)
    {
        std::cout << "invalid value => retry :\n";
        std::cin >> dPrincipal;
    }

    return dPrincipal;
}

double ChooseCostOfLoan()
{
   std::cout << "Enter cost of loan (global rate in %, value > 0. expected) :\n";

   double dGlobalRate = 0.;
   std::cin >> dGlobalRate;

   while (dGlobalRate <= 0.)
   {
      std::cout << "invalid value => retry :\n";
      std::cin >> dGlobalRate;
   }

   return dGlobalRate / 100.;
}

double ChooseYearlyFixedRate(const TypeLoan &xTypeLoan)
{
   if (xTypeLoan == TypeLoan::fixedRate)
      std::cout << "Enter yearly fixed rate of loan (in %, value > 0. expected) :\n";
   else if (xTypeLoan == TypeLoan::floatingRate)
      std::cout << "Enter yearly margin rate of loan (in %, value > 0. expected) :\n";

    double dFixedRate = 0.;
    std::cin >> dFixedRate;

    while (dFixedRate <= 0.)
    {
        std::cout << "invalid value => retry :\n";
        std::cin >> dFixedRate;
    }

    return dFixedRate / 100.;
}

double ChooseFixedInstalment()
{
    std::cout << "Enter fixed instalment for loan (value > 0. expected) :\n";

    double dFixedInstalment = 0.;
    std::cin >> dFixedInstalment;

    while (dFixedInstalment <= 0.)
    {
        std::cout << "invalid value => retry :\n";
        std::cin >> dFixedInstalment;
    }

    return dFixedInstalment;
}

unsigned int ChooseNumberPayments()
{
    std::cout << "Enter number of principal payments for loan (integer value >= 1 expected) :\n";

    unsigned int uiNbPayments = 0;
    std::cin >> uiNbPayments;

    while (uiNbPayments < 1)
    {
        std::cout << "invalid value => retry :\n";
        std::cin >> uiNbPayments;
    }

    return uiNbPayments;
}

double ChooseVolatilityYearlyBenchmarkRates()
{
   std::cout << "Enter volatility of yearly benchmark rates (in %, value > 0. expected) :\n";

   double dVolatility = 0.;
   std::cin >> dVolatility;

   while (dVolatility <= 0.)
   {
      std::cout << "invalid value => retry :\n";
      std::cin >> dVolatility;
   }

   return dVolatility / 100.;
}

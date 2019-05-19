#include "loan.hpp"
#include <iostream>
#include <cstdlib>
#include <string>


static void printBannerWelcome()
{
    std::cout << "####################################################\n";
    std::cout << "#                                                  #\n";
    std::cout << "# Welcome in our solution to manage loan schedules #\n";
    std::cout << "#                                                  #\n";
    std::cout << "####################################################\n\n";
}

static void printBannerGoodBye()
{
    std::cout << "######################################################\n";
    std::cout << "#                                                    #\n";
    std::cout << "#  Thanks for using our solution. Hope the service   #\n";
    std::cout << "# quality has been satisfying, and to see you soon ! #\n";
    std::cout << "#                                                    #\n";
    std::cout << "######################################################\n\n";
}

static TypeLoan chooseTypeLoan()
{
    std::cout << "Choose type of loan to consider : 0 for fixed rate and instalment, 1 for floating rate\n";

    unsigned int uiTypeLoan = 0;
    std::cin >> uiTypeLoan;

    while (uiTypeLoan != 0 && uiTypeLoan != 1)
    {
        std::cout << "invalid choice => retry :\n";
        std::cin >> uiTypeLoan;
    }

    return uiTypeLoan == 0 ? TypeLoan::fixedRate : TypeLoan::floatingRate;
}

enum class UnknownParameterLoanFixedRate { principal, yearlyRate, instalment, nbPayments };

static UnknownParameterLoanFixedRate chooseUnknownParameterLoanFixedRate()
{
    std::cout << "Choose which feature you do NOT know to define the loan ?\n";
    std::cout << "0 for principal, 1 for yearly fixed rate, 2 for fixed instalment, 3 for number of payments :\n";

    int iUnknownParameter = 0;
    std::cin >> iUnknownParameter;

    while (iUnknownParameter != 0 && iUnknownParameter != 1 && iUnknownParameter != 2 && iUnknownParameter != 3)
    {
        std::cout << "invalid choice => retry :\n";
        std::cin >> iUnknownParameter;
    }

    UnknownParameterLoanFixedRate xUnknownParameter = UnknownParameterLoanFixedRate::principal;

    if (iUnknownParameter == 1)
        xUnknownParameter = UnknownParameterLoanFixedRate::yearlyRate;
    else if (iUnknownParameter == 2)
        xUnknownParameter = UnknownParameterLoanFixedRate::instalment;
    else if (iUnknownParameter == 3)
        xUnknownParameter = UnknownParameterLoanFixedRate::nbPayments;

    return xUnknownParameter;
}

static void chooseMonthFirstPayment(Month &xMonthFirstPayment)
{
    std::cout << "Choose month (integer between 1 and 12) for first instalment payment :\n";

    int iMonth = 0;
    std::cin >> iMonth;

    while (iMonth < 1 || iMonth > 12)
    {
        std::cout << "invalid choice => retry :\n";
        std::cin >> iMonth;
    }

    if (iMonth == 1)
        xMonthFirstPayment = Month::january;
    else if (iMonth == 2)
        xMonthFirstPayment = Month::february;
    else if (iMonth == 3)
        xMonthFirstPayment = Month::march;
    else if (iMonth == 4)
        xMonthFirstPayment = Month::april;
    else if (iMonth == 5)
        xMonthFirstPayment = Month::may;
    else if (iMonth == 6)
        xMonthFirstPayment = Month::june;
    else if (iMonth == 7)
        xMonthFirstPayment = Month::july;
    else if (iMonth == 8)
        xMonthFirstPayment = Month::august;
    else if (iMonth == 9)
        xMonthFirstPayment = Month::september;
    else if (iMonth == 10)
        xMonthFirstPayment = Month::october;
    else if (iMonth == 11)
        xMonthFirstPayment = Month::november;
    else if (iMonth == 12)
        xMonthFirstPayment = Month::december;
}

static void chooseYearFirstPayment(Year &xYearFirstPayment)
{
    std::cout << "Choose year (format AAAA => 4 digits max) for first instalment payment :\n";

    unsigned int uiYear = 0;
    std::cin >> uiYear;

    while (uiYear > 10000)
    {
        std::cout << "invalid choice => retry :\n";
        std::cin >> uiYear;
    }

    xYearFirstPayment.set(uiYear);
}

static void chooseFrequencyPayments(FrequencyPayments &xFrequencyPayments)
{
    std::cout << "Choose frequency for instalment payments within a year window ?\n";
    std::cout << "1 for yearly, 2 for semi-annually, 3 for quarterly, 4 for monthly :\n";

    int iFrequency = 0;
    std::cin >> iFrequency;

    while (iFrequency < 1 || iFrequency > 4)
    {
        std::cout << "invalid choice => retry :\n";
        std::cin >> iFrequency;
    }

    if (iFrequency == 1)
        xFrequencyPayments = FrequencyPayments::yearly;
    else if (iFrequency == 2)
        xFrequencyPayments = FrequencyPayments::semi_annually;
    else if (iFrequency == 3)
        xFrequencyPayments = FrequencyPayments::quarterly;
    else if (iFrequency == 4)
        xFrequencyPayments = FrequencyPayments::monthly;
}

static bool chooseToPursueExecution()
{
    std::cout << "Do you wish to treat another loan ? (1 for yes, anything else otherwise\n";

    int iTreatAnotherLoan = 1;
    std::cin >> iTreatAnotherLoan;

    return iTreatAnotherLoan == 1 ? true : false;
}

int main()
{
    printBannerWelcome();

    bool bPursueExecution = true;
    TypeLoan xTypeLoan = TypeLoan::fixedRate;
    Month xMonthFirstPayment = Month::january;
    Year xYearFirstPayment(0);
    FrequencyPayments xFrequencyPayments = FrequencyPayments::yearly;
    std::string sDefaultPathFileCsv;

    while (bPursueExecution)
    {
        xTypeLoan = chooseTypeLoan();

        chooseMonthFirstPayment(xMonthFirstPayment);
        chooseYearFirstPayment(xYearFirstPayment);
        chooseFrequencyPayments(xFrequencyPayments);

        if (xTypeLoan == TypeLoan::fixedRate)
        {
            UnknownParameterLoanFixedRate xUnknownParameter = chooseUnknownParameterLoanFixedRate();
            double dPrincipal = 0.;
            double dCostLoan = 0.;
            double dYearlyFixedRate = 0.;
            double dFixedInstalment = 0.;
            unsigned int uiNbPayments = 0;

            if (xUnknownParameter == UnknownParameterLoanFixedRate::principal)
            {
                dCostLoan = ChooseCostOfLoan();
                dYearlyFixedRate = ChooseYearlyFixedRate(xTypeLoan);
                uiNbPayments = ChooseNumberPayments();
                dFixedInstalment = ChooseFixedInstalment();

                LoanFixedRate xLoan(CostLoan(dCostLoan), YearlyFixedRate(dYearlyFixedRate), xMonthFirstPayment, xYearFirstPayment, xFrequencyPayments, NbPayments(uiNbPayments), FixedInstalment(dFixedInstalment));
                xLoan.printCSV(sDefaultPathFileCsv);
            }
            else if (xUnknownParameter == UnknownParameterLoanFixedRate::yearlyRate)
            {
                dPrincipal = ChoosePrincipal();
                dCostLoan = ChooseCostOfLoan();
                uiNbPayments = ChooseNumberPayments();
                dFixedInstalment = dPrincipal * (1. + dCostLoan) / static_cast<double>(uiNbPayments);

                LoanFixedRate xLoan(Principal(dPrincipal), CostLoan(dCostLoan), xMonthFirstPayment, xYearFirstPayment, xFrequencyPayments, NbPayments(uiNbPayments), FixedInstalment(dFixedInstalment));
                xLoan.printCSV(sDefaultPathFileCsv);
            }
            else if (xUnknownParameter == UnknownParameterLoanFixedRate::instalment)
            {
                dPrincipal = ChoosePrincipal();
                dCostLoan = ChooseCostOfLoan();
                dYearlyFixedRate = ChooseYearlyFixedRate(xTypeLoan);
                uiNbPayments = ChooseNumberPayments();

                LoanFixedRate xLoan(Principal(dPrincipal), CostLoan(dCostLoan), YearlyFixedRate(dYearlyFixedRate), xMonthFirstPayment, xYearFirstPayment, xFrequencyPayments, NbPayments(uiNbPayments));
                xLoan.printCSV(sDefaultPathFileCsv);
            }
            else if (xUnknownParameter == UnknownParameterLoanFixedRate::nbPayments)
            {
                dPrincipal = ChoosePrincipal();
                dCostLoan = ChooseCostOfLoan();
                dYearlyFixedRate = ChooseYearlyFixedRate(xTypeLoan);
                dFixedInstalment = ChooseFixedInstalment();

                LoanFixedRate xLoan(Principal(dPrincipal), CostLoan(dCostLoan), YearlyFixedRate(dYearlyFixedRate), xMonthFirstPayment, xYearFirstPayment, xFrequencyPayments, FixedInstalment(dFixedInstalment));
                xLoan.printCSV(sDefaultPathFileCsv);
            }
        }
        else
        {
           double dPrincipal = ChoosePrincipal();
           double dYearlyMarginRate = ChooseYearlyFixedRate(xTypeLoan);
           double dStandardDeviationBenchmarkRates = ChooseVolatilityYearlyBenchmarkRates();
           unsigned int uiNbPayments = ChooseNumberPayments();

           LoanFloatingRate xLoan(Principal(dPrincipal), YearlyMarginRate(dYearlyMarginRate), StandardDeviation(dStandardDeviationBenchmarkRates), SeedPseudoRandomGenerator(-1), xMonthFirstPayment, xYearFirstPayment, xFrequencyPayments, NbPayments(uiNbPayments));
           xLoan.printCSV(sDefaultPathFileCsv);
        }


        bPursueExecution = chooseToPursueExecution();
    }

    printBannerGoodBye();

    return 0;
}

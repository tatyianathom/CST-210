#ifndef AIRGEAD_BANKING_INVESTMENT_H_
#define AIRGEAD_BANKING_INVESTMENT_H_
#include <vector>

// Groups the three values displayed in one annual report row.
struct AnnualResults {
    int year;
    double balance;
    double interestEarned;
};

// Stores investment settings and calculates monthly compound growth.
class Investment {
public:
    // Initialize the investment settings.
    // NOTE: The implementation uses monthly deposit as the second argument and annual interest rate as the third argument.
    Investment(double t_initialInvestment, double t_monthlyDeposit, double t_annualInterestRate, int t_years);

    std::vector<AnnualResults> calculateAnnualResults(bool t_includeMonthlyDeposits) const;

    private:
    double m_initialInvestment;
    double m_monthlyDeposit;
    double m_annualInterestRate;
    int m_years;
};

#endif //AIRGEAD_BANKING_INVESTMENT_H
#include "Investment.h"
#include <cmath>
#include <stdexcept>

// Store the supplied settings using a member initializer list.
Investment::Investment(
        double t_initialInvestment, 
        double t_monthlyDeposit, 
        double t_annualInterestRate, 
        int t_years
    ) : 
        m_initialInvestment(t_initialInvestment), 
        m_monthlyDeposit(t_monthlyDeposit), 
        m_annualInterestRate(t_annualInterestRate), 
        m_years(t_years) {
    
        // Protect class if called outside the console interface with invalid values
        if (!std::isfinite(m_initialInvestment) || m_initialInvestment <= 0.0 || !std::isfinite(m_monthlyDeposit) || m_monthlyDeposit <= 0.0 || !std::isfinite(m_annualInterestRate) || m_annualInterestRate < 0.0 || m_years <= 0) {
            throw std::invalid_argument("Invalid Investment settings.");
        }
}

std::vector<AnnualResults> Investment::calculateAnnualResults(bool t_includeMonthlyDeposits) const {

    double balance = m_initialInvestment;
    const double monthlyInterestRate = (m_annualInterestRate / 100.0) / 12.0;
    const double depositAmount = t_includeMonthlyDeposits ? m_monthlyDeposit : 0.0;
    std::vector<AnnualResults> annualRows;

    for (int yearIndex = 0; yearIndex < m_years; ++yearIndex) {
        double yearInterest = 0.0;

        for (int month = 0; month < 12; ++month) {
            const double openingBalance = balance;
            const double total = openingBalance + depositAmount;
            const double monthInterest = total * monthlyInterestRate;

            balance = total + monthInterest;
            yearInterest += monthInterest;

            if (!std::isfinite(balance) || !std::isfinite(yearInterest)) {
                throw std::overflow_error("The investment exceeds the supported range.");
            }
        }
        // Preserve precision; round only when displaying the report.
        annualRows.push_back({yearIndex + 1, balance, yearInterest});
    }

    return annualRows;
}
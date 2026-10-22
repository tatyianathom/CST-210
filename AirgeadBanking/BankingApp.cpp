#include "BankingApp.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace {
// A distinct exception allows closed input to end the program gracefully.
class InputClosed : public std::runtime_error {
public:
    InputClosed() : std::runtime_error("Input stream closed.") {}
};
}

std::string BankingApp::readLine(const std::string& t_prompt) const {
    std::cout << t_prompt;

    std::string input;
    if (!std::getline(std::cin, input)) {
        throw InputClosed();
    }

    return input;
}

double BankingApp::readAmount(const std::string& t_prompt,
                              bool t_allowZero) const {
    while (true) {
        std::istringstream input(readLine(t_prompt));
        double value = 0.0;

        if (input >> value) {
            input >> std::ws;

            // Reject partially numeric input such as 50abc.
            if (input.eof() && std::isfinite(value) &&
                (t_allowZero ? value >= 0.0 : value > 0.0)) {
                return value;
            }
        }

        std::cout << (t_allowZero
            ? "Enter a finite number greater than or equal to zero.\n"
            : "Enter a finite number greater than zero.\n");
    }
}

int BankingApp::readYears() const {
    while (true) {
        std::istringstream input(readLine("Number of Years: "));
        int years = 0;

        if (input >> years) {
            input >> std::ws;

            // Reject fractional years and extra characters.
            if (input.eof() && years > 0) {
                return years;
            }
        }

        std::cout
            << "Enter a positive whole number within the integer range.\n";
    }
}

bool BankingApp::readRunAgain() const {
    while (true) {
        std::istringstream input(
            readLine("Try another scenario? (y/n): "));

        std::string answer;
        input >> answer;
        input >> std::ws;

        // Accept answers regardless of capitalization.
        std::transform(answer.begin(), answer.end(), answer.begin(),
                       [](unsigned char t_character) {
                           return static_cast<char>(
                               std::tolower(t_character));
                       });

        if (input.eof()) {
            if (answer == "y" || answer == "yes") {
                return true;
            }

            if (answer == "n" || answer == "no") {
                return false;
            }
        }

        std::cout << "Enter y or n.\n";
    }
}

void BankingApp::displayReport(
    const std::string& t_title,
    const std::vector<AnnualResults>& t_rows) const {

    std::cout << '\n' << t_title << '\n'
              << std::string(72, '=') << '\n'
              << std::left << std::setw(10) << "Year"
              << std::right << std::setw(26) << "Year End Balance"
              << std::setw(30) << "Year End Earned Interest" << '\n'
              << std::string(72, '-') << '\n';

    for (const AnnualResults& row : t_rows) {
        // Format separately so dollar signs align with their amounts.
        std::ostringstream balanceText;
        std::ostringstream interestText;

        balanceText << '$' << std::fixed
                    << std::setprecision(2) << row.balance;

        interestText << '$' << std::fixed
                     << std::setprecision(2) << row.interestEarned;

        std::cout << std::left << std::setw(10) << row.year
                  << std::right << std::setw(26) << balanceText.str()
                  << std::setw(30) << interestText.str() << '\n';
    }
}

void BankingApp::run() {
    try {
        bool runAgain = true;

        while (runAgain) {
            std::cout << "\n**********************************\n"
                      << "******** AIRGEAD BANKING *********\n"
                      << "*********** Data Input ***********\n";

            const double initialInvestment =
                readAmount("Initial Investment Amount: $", false);

            const double monthlyDeposit =
                readAmount("Monthly Deposit: $", false);

            const double annualInterest =
                readAmount("Annual Interest (%): ", true);

            const int numberOfYears = readYears();

            std::cout << std::fixed << std::setprecision(2)
                      << "\nInitial Investment Amount: $"
                      << initialInvestment
                      << "\nMonthly Deposit: $" << monthlyDeposit
                      << "\nAnnual Interest: " << annualInterest << '%'
                      << "\nNumber of Years: " << numberOfYears << '\n';

            readLine("Press Enter to continue...");

            try {
                const Investment investment(
                    initialInvestment,
                    monthlyDeposit,
                    annualInterest,
                    numberOfYears);

                // Calculate both reports before displaying the results.
                const auto withoutDepositRows =
                    investment.calculateAnnualResults(false);

                const auto withDepositRows =
                    investment.calculateAnnualResults(true);

                displayReport(
                    "Balance and Interest Without Additional Monthly Deposits",
                    withoutDepositRows);

                displayReport(
                    "Balance and Interest With Additional Monthly Deposits",
                    withDepositRows);

            } catch (const std::exception& error) {
                std::cout << "Unable to calculate reports: "
                          << error.what()
                          << "\nTry smaller input values.\n";
            }

            runAgain = readRunAgain();
        }

        std::cout << "Thank you for using Airgead Banking.\n";

    } catch (const InputClosed&) {
        std::cout << "\nInput closed. Goodbye.\n";
    }
}
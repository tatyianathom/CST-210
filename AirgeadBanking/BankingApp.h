#ifndef AIRGEAD_BANKING_BANKING_APP_H_
#define AIRGEAD_BANKING_BANKING_APP_H_

#include "Investment.h"

#include <string>
#include <vector>

// Handles user input, report formatiting, and program flow control. The Investment class handles the calculations.
class BankingApp {
public:
    void run();

private:
    std::string readLine(const std::string& t_prompt) const;
    double readAmount(const std::string& t_prompt, bool t_allowZero) const;
    int readYears() const;
    bool readRunAgain() const;

    void displayReport(const std::string& t_title,
                       const std::vector<AnnualResults>& t_rows) const;
};

#endif  // AIRGEAD_BANKING_BANKING_APP_H_
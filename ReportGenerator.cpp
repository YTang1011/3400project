#include "ReportGenerator.h"
#include <iostream>
#include <map>
#include <fstream>

void ReportGenerator::generateSalesReport(const std::vector<UsageRecord>& records) {
    std::map<std::string, double> salesByUtility;
    for (const auto& r : records) {
        salesByUtility[r.utilityType] += r.totalCost;
    }

    std::cout << "\n--- Sales Report by Utility ---" << std::endl;
    for (const auto& pair : salesByUtility) {
        std::cout << pair.first << ": $" << pair.second << std::endl;
    }
}

void ReportGenerator::generateOverdueReport(const std::vector<UsageRecord>& records) {
    std::cout << "\n--- Overdue Accounts Report ---" << std::endl;
    bool found = false;
    for (const auto& r : records) {
        if (!r.isPaid) {
            found = true;
            std::cout << "Customer: " << r.customerName
                      << " | Utility: " << r.utilityType
                      << " | Amount Due: $" << r.totalCost << std::endl;
        }
    }
    if (!found) {
        std::cout << "No overdue accounts found.\n";
    }
}


void ReportGenerator::exportSalesReport(const std::vector<UsageRecord>& records, const std::string& filename) {
    std::map<std::string, double> salesByUtility;
    for (const auto& r : records) {
        salesByUtility[r.utilityType] += r.totalCost;
    }

    std::ofstream file(filename);
    if (!file) {
        std::cerr <<"Failed to write sales report"<<std::endl;
        return;
    }

    file << "--- Sales Report by Utility ---\n";
    for (const auto& pair : salesByUtility) {
        file << pair.first << ": $" << pair.second << "\n";
    }

    file.close();
    std::cout << "Sales report '" << filename<<"' has been generated." <<std::endl;
}



void ReportGenerator::exportOverdueReport(const std::vector<UsageRecord>& records, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to write overdue report" <<std::endl;
        return;
    }

    file << "--- Overdue Accounts Report ---\n";
    bool found = false;
    for (const auto& r : records) {
        if (!r.isPaid) {
            found = true;
            file << "Customer: " << r.customerName
                 << " | Utility: " << r.utilityType
                 << " | Amount Due: $" << r.totalCost << "\n";
        }
    }
    if (!found) {
        file << "No overdue accounts found.\n";
    }

    file.close();
    std::cout << "Overdue report '"<< filename<<"' has been generated."<<std::endl;
}

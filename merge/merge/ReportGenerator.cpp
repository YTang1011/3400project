//Member3:ZiheZhao
#include "ReportGenerator.h"
#include <iostream>
#include <fstream>
#include <map>

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
        std::cout << "Cannot found overdue accounts." << std::endl;
    }
}

void ReportGenerator::exportSalesReport(const std::vector<UsageRecord>& records, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open '" <<  filename << "' for writing report." << std::endl;
        return;
    }

    std::map<std::string, double> salesByUtility;
    for (const auto& r : records) {
        salesByUtility[r.utilityType] += r.totalCost;
    }

    file << "--- Sales Report by Utility ---\n";
    for (const auto& pair : salesByUtility) {
        file << pair.first << ": $" << pair.second << "\n";
    }

    file.close();
    std::cout << "Sales report exported to '" <<filename  << "' successfully.\n";
}

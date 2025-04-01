//Member3:ZiheZhao
#include "DataManager.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void DataManager::saveUsageRecords(const std::vector<UsageRecord>& records, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open '" << filename << "' for saving usage records." << std::endl;
        return;
    }
    for (const auto& r : records) {
        file << r.customerName << "," << r.utilityType << ","
             << r.usageAmount << "," << r.totalCost << ","
             << r.isPaid << "\n";
    }
    file.close();
}

std::vector<UsageRecord> DataManager::loadUsageRecords(const std::string& filename) {
    std::vector<UsageRecord> records;
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open '" << filename << "' for loading usage records." << std::endl;
        return records;
    }

    std::string name, type;
    double usage, cost;
    bool paid;

    while (std::getline(file, name, ',') &&
           std::getline(file, type, ',') &&
           (file >> usage) && file.get() &&
           (file >> cost) && file.get() &&
           (file >> paid)) {
        file.get();
        records.push_back({ name, type, usage, cost, paid });
    }

    file.close();
    return records;
}

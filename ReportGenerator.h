// Member3: Zihe Zhao
#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <vector>
#include <string>
#include "UsageRecord.h"

class ReportGenerator {
public:
    void generateSalesReport(const std::vector<UsageRecord>& records);
    void generateOverdueReport(const std::vector<UsageRecord>& records);
    
    void exportSalesReport(const std::vector<UsageRecord>& records, const std::string& filename);
    void exportOverdueReport(const std::vector<UsageRecord>& records, const std::string& filename);
};

#endif

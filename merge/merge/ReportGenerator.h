//Member3:ZiheZhao
#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <vector>
#include "UsageRecord.h"

class ReportGenerator {
public:
    void generateSalesReport(const std::vector<UsageRecord>& records);
    void generateOverdueReport(const std::vector<UsageRecord>& records);
    void exportSalesReport(const std::vector<UsageRecord>& records, const std::string& filename);
};

#endif

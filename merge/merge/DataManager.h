//Member3:ZiheZhao
#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <vector>
#include <string>
#include "UsageRecord.h"

class DataManager {
public:
    void saveUsageRecords(const std::vector<UsageRecord>& records, const std::string& filename);
    std::vector<UsageRecord> loadUsageRecords(const std::string& filename);
    void generateOverdueReport(const std::vector<UsageRecord>& records);

};

#endif

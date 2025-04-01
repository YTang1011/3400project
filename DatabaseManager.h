#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include "UsageRecord.h"

class DatabaseManager {
private:
    sqlite3* db;
    void executeQuery(const std::string& query);
public:
    DatabaseManager(const std::string& dbName);
    ~DatabaseManager();

    void initializeTables();
    
    void addRegion(const std::string& regionName);
    void addCustomer(const std::string& name, const std::string& address, const std::string& region);
    
    // Billing Records Management – dates in mm/dd/yyyy format.
    void addBillingRecord(const std::string& customerName, const std::string& utility, double usage, double totalCost, const std::string& billingDate);
    void recordPayment(const std::string& customerName, const std::string& utility, const std::string& paymentDate);
    
    std::vector<UsageRecord> getUsageRecords();
    
    std::vector<UsageRecord> getOverdueRecords(const std::string& currentDate);

    std::vector<std::string> loadRegions();
};

#endif

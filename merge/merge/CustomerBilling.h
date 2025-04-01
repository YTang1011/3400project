#ifndef CUSTOMERBILLING_H
#define CUSTOMERBILLING_H

#include <string>
#include <vector>
#include <map>
#include "UsageRecord.h"

// BillingRecord
struct BillingRecord {
    std::string utilityType;
    double usageAmount;
    double usageCost;
    double fixedCost;
    double totalCost;
    int billingDay;
    int paymentDay;

    BillingRecord(const std::string& utility, double usage, double usageCost, double fixedCost, int billingDay)
        : utilityType(utility), usageAmount(usage), usageCost(usageCost), fixedCost(fixedCost),
          totalCost(usageCost + fixedCost), billingDay(billingDay), paymentDay(-1) {}
};

// Customer class
class Customer {
private:
    std::string name;
    std::string address;
    std::vector<BillingRecord> bills;

public:
    Customer(const std::string& name, const std::string& address);

    void addBill(const BillingRecord& record);
	
    void recordPayment(size_t billIndex, int paymentDay);
    
    const std::string& getName() const;
    const std::vector<BillingRecord>& getBills() const;
    std::vector<UsageRecord> getUsageRecords() const;

    // get owing functions
    double getOwingForUtility(const std::string& utilityType) const;
	
    double getTotalOwing() const;
};

// BillingSystem class
class BillingSystem {
private:
    std::map<std::string, double> meterRates;

public:
    BillingSystem();
	
    void setMeterRate(const std::string& utilityType, double rate);
	
    double getMeterRate(const std::string& utilityType) const;
	
    double calculateTotalCost(double usage, double unitCost, const std::string& utilityType) const;
	
    BillingRecord createBillingRecord(const std::string& utilityType, double usage, double unitCost, int billingDay) const;
	
    bool isOverdue(const BillingRecord& record, int currentDay) const;
	
    void showOverdueBillsWithInterest(const std::vector<Customer>& customers, int currentDay) const;
};

#endif 

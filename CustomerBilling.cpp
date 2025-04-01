#include "CustomerBilling.h"
#include <iostream>

// Customer Class Implementation 
Customer::Customer(const std::string& name, const std::string& address)
    : name(name), address(address) {}

void Customer::addBill(const BillingRecord& record) {
    bills.push_back(record);
}

void Customer::recordPayment(size_t billIndex, int paymentDay) {
    if (billIndex < bills.size()) {
        bills[billIndex].paymentDay = paymentDay;
    } else {
        std::cerr << "Invalid bill index." << std::endl;
    }
}

const std::string& Customer::getName() const {
    return name;
}

const std::vector<BillingRecord>& Customer::getBills() const {
    return bills;
}

std::vector<UsageRecord> Customer::getUsageRecords() const {
    std::vector<UsageRecord> usageRecords;
    for (const auto& bill : bills) {
        usageRecords.push_back({ name, bill.utilityType, bill.usageAmount, bill.totalCost, (bill.paymentDay != -1) });
    }
    return usageRecords;
}

// Get owing amount for a specific utility
double Customer::getOwingForUtility(const std::string& utilityType) const {
    double totalOwed = 0.0;
    for (const auto& bill : bills) {
        if (bill.utilityType == utilityType && bill.paymentDay == -1) {
            totalOwed += bill.totalCost;
        }
    }
    return totalOwed;
}

// Get total owing amount for all utilities
double Customer::getTotalOwing() const {
    double totalOwed = 0.0;
    for (const auto& bill : bills) {
        if (bill.paymentDay == -1) {
            totalOwed += bill.totalCost;
        }
    }
    return totalOwed;
}

// BillingSystem Class Implementation
BillingSystem::BillingSystem() {
    meterRates["Natural Gas"] = 5.00;
    meterRates["TV"] = 8.00;
    meterRates["Mobile Phone"] = 10.00;
    meterRates["Home Phone"] = 7.50;
    meterRates["Electric"] = 10.00;
    meterRates["Water"] = 5.00;
    meterRates["Sewerage"] = 6.00;
}

void BillingSystem::setMeterRate(const std::string& utilityType, double rate) {
    meterRates[utilityType] = rate;
}

double BillingSystem::getMeterRate(const std::string& utilityType) const {
    auto it = meterRates.find(utilityType);
    return (it != meterRates.end()) ? it->second : 0.0;
}

double BillingSystem::calculateTotalCost(double usage, double unitCost, const std::string& utilityType) const {
    double usageCost = unitCost * usage;
    double fixedCost = getMeterRate(utilityType);
    return usageCost + fixedCost;
}

BillingRecord BillingSystem::createBillingRecord(const std::string& utilityType, double usage, double unitCost, int billingDay) const {
    double usageCost = unitCost * usage;
    double fixedCost = getMeterRate(utilityType);
    return BillingRecord(utilityType, usage, usageCost, fixedCost, billingDay);
}

bool BillingSystem::isOverdue(const BillingRecord& record, int currentDay) const {
    return (record.paymentDay == -1 && (currentDay - record.billingDay) > 30);
}

// Show overdue bills with 10% interest
void BillingSystem::showOverdueBillsWithInterest(const std::vector<Customer>& customers, int currentDay) const {
    std::cout << "\n--- Overdue Bills with Interest ---\n";
    bool found = false;

    for (const auto& customer : customers) {
        for (const auto& bill : customer.getBills()) {
            if (isOverdue(bill, currentDay)) {
                found = true;
                double interest = bill.totalCost * 0.10;  // % interest
                double newTotal = bill.totalCost + interest;

                std::cout << "Customer: " << customer.getName()
                          << " | Utility: " << bill.utilityType
                          << " | Original Due: $" << bill.totalCost
                          << " | Interest: $" << interest
                          << " | New Due: $" << newTotal
                          << " | Billing Day: " << bill.billingDay << "\n";
            }
        }
    }

    if (!found) {
        std::cout << "No overdue bills found.\n";
    }
}

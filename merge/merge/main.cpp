#include <iostream>
#include <vector>
#include <memory>
#include "CustomerBilling.h"
#include "ReportGenerator.h"
#include "UtilityProviders.h"

using namespace std;

int displayMenuAndGetChoice(const vector<UtilityProvider*>& providers) {
    cout << "\nSelect a Utility Provider:" << endl;
    for (size_t i = 0; i < providers.size(); i++) {
        cout << i + 1 << ". " << providers[i]->getName() 
             << " (Base Price: $" << providers[i]->calculateCost(1) << " per unit)" << endl;
    }
    
    int choice;
    cout << "Enter your choice: ";
    cin >> choice;
    
    return choice - 1;
}

int main() {
    BillingSystem billingSystem;
    //rates for all utilities (fixed costs)
    billingSystem.setMeterRate("Crude Oil", 5.00);
    billingSystem.setMeterRate("Solar", 4.00);
    billingSystem.setMeterRate("Nuclear", 6.00);
    billingSystem.setMeterRate("Natural Gas", 5.00); 

    vector<UtilityProvider*> providers = {
        new CrudeOilProvider(2.50),
        new SolarProvider(1.75),
        new NuclearProvider(3.00),
        new NaturalGasProvider(2.00)
    };

    vector<Customer> customers;
    customers.emplace_back("John Doe", "123 Main St");
    Customer& currentCustomer = customers.back();

    // Customer selects a utility
    int selectedIndex = displayMenuAndGetChoice(providers);
    if (selectedIndex < 0 || selectedIndex >= providers.size()) {
        cerr << "Invalid selection!" << endl;
        return 1;
    }
    UtilityProvider* provider = providers[selectedIndex];
    string utilityType = provider->getName();
    double unitCost = provider->calculateCost(1);

    // Get usage and billing day
    double usage;
    int billingDay;
    cout << "Enter usage (units): ";
    cin >> usage;
    cout << "Enter billing day: ";
    cin >> billingDay;

    // Create and add billing record
    BillingRecord record = billingSystem.createBillingRecord(utilityType, usage, unitCost, billingDay);
    currentCustomer.addBill(record);

    // Record payment
    int paymentDay;
    cout << "Enter payment day: ";
    cin >> paymentDay;
    currentCustomer.recordPayment(0, paymentDay);

    // Generate reports
    ReportGenerator reportGen;
    auto usageRecords = currentCustomer.getUsageRecords();
    reportGen.generateSalesReport(usageRecords);
    reportGen.generateOverdueReport(usageRecords);

    // Check overdue bills
    int currentDay;
    cout << "Enter current day: ";
    cin >> currentDay;
    billingSystem.showOverdueBillsWithInterest(customers, currentDay);

    // Cleanup
    for (auto p : providers) delete p;
    return 0;
}
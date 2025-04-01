#include <iostream>
#include <string>
#include <vector>
#include "DatabaseManager.h"
#include "ReportGenerator.h"
#include "UtilityProviders.h"
#include <limits>

void printMenu() {
    std::cout << "\n--- Utility Billing System Menu ---\n";
    std::cout << "1. Add Region\n";
    std::cout << "2. Add Customer\n";
    std::cout << "3. Add Billing Record\n";
    std::cout << "4. Record Payment\n";
    std::cout << "5. Generate Sales Report\n";
    std::cout << "6. Generate Overdue Report\n";
    std::cout << "7. Exit\n";
    std::cout << "Enter your choice: ";
}

void sendEmailReminder(const std::string& customerName, const std::string& utility) {
    std::cout << "Sending email reminder to " << customerName 
              << " for overdue " << utility << " bill.\n";
}

int main() {
    DatabaseManager dbManager("billing.db");
    dbManager.initializeTables();

    std::vector<std::string> defaultRegions = { "North", "South", "East", "West", "Central" };
    for (const auto& region : defaultRegions) {
        dbManager.addRegion(region);
    }

    ReportGenerator reportGen;
    
    CrudeOilProvider crudeOil(2.50);
    SolarProvider solar(1.75);
    NuclearProvider nuclear(3.00);
    NaturalGasProvider naturalGas(2.00);
    
    bool running = true;
    int choice = 0;
    while (running) {
        printMenu();
        std::cin >> choice;
        if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input! Please enter a number.\n";
        continue;
        }
        std::cin.ignore(); // Clear newline

        switch (choice) {
            case 1: {
                std::string regionName;
                std::cout << "Enter Region Name: ";
                std::getline(std::cin, regionName);
                dbManager.addRegion(regionName);
                std::cout << "Region added.\n";
                break;
            }
            case 2: {
                std::string name, address, region;
                std::cout << "Enter Customer Name: ";
                std::getline(std::cin, name);
                std::cout << "Enter Address: ";
                std::getline(std::cin, address);
                
                std::cout << "Available Regions:\n";
                auto regions = dbManager.loadRegions(); //load from the database
                for (const auto& r : regions) {
                std::cout << "- " << r << "\n";
                }

                std::cout << "Enter Region (choose from above): ";
                std::getline(std::cin, region);
                
                dbManager.addCustomer(name, address, region);
                std::cout << "Customer added in the " << region << " region.\n";
                break;
            }
            case 3: {
                std::string customerName, utility, billingDate;
                double usage, unitCost;
                std::cout << "Enter Customer Name: ";
                std::getline(std::cin, customerName);
                std::cout << "Enter Utility Type (Crude Oil, Solar, Nuclear, Natural Gas): ";
                std::getline(std::cin, utility);
                std::cout << "Enter Usage (units): ";
                std::cin >> usage;
                std::cin.ignore();
                std::cout << "Enter Billing Date (mm/dd/yyyy): ";
                std::getline(std::cin, billingDate);
                
                if (utility == "Crude Oil")
                    unitCost = crudeOil.calculateCost(1);
                else if (utility == "Solar")
                    unitCost = solar.calculateCost(1);
                else if (utility == "Nuclear")
                    unitCost = nuclear.calculateCost(1);
                else if (utility == "Natural Gas")
                    unitCost = naturalGas.calculateCost(1);
                else {
                    std::cout << "Unknown utility type. Billing record not added.\n";
                    break;
                }
                
                double totalCost = unitCost * usage;
                dbManager.addBillingRecord(customerName, utility, usage, totalCost, billingDate);
                std::cout << "Billing record added for " << customerName << ".\n";
                break;
            }
            case 4: {
                std::string customerName, utility, paymentDate;
                std::cout << "Enter Customer Name: ";
                std::getline(std::cin, customerName);
                std::cout << "Enter Utility Type for which payment is made: ";
                std::getline(std::cin, utility);
                std::cout << "Enter Payment Date (mm/dd/yyyy): ";
                std::getline(std::cin, paymentDate);
                dbManager.recordPayment(customerName, utility, paymentDate);
                std::cout << "Payment recorded for " << customerName << ".\n";
                break;
            }
            case 5: {
                std::vector<UsageRecord> usageRecords = dbManager.getUsageRecords();
                reportGen.generateSalesReport(usageRecords);
                reportGen.exportSalesReport(usageRecords, "sales_report.txt");
                break;
            }
            case 6: {
                std::string currentDate;
                std::cout << "Enter Current Date (mm/dd/yyyy): ";
                std::getline(std::cin, currentDate);
                std::vector<UsageRecord> overdueRecords = dbManager.getOverdueRecords(currentDate);
                reportGen.generateOverdueReport(overdueRecords);
                reportGen.exportOverdueReport(overdueRecords, "overdue_report.txt");
                for (const auto& record : overdueRecords) {
                    sendEmailReminder(record.customerName, record.utilityType);
                }
                break;
            }
            case 7: {
                running = false;
                std::cout << "Exiting program.\n";
                break;
            }
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}

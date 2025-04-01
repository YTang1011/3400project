#include "DatabaseManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace {
    // Helper: convert date string "mm/dd/yyyy" into time_t.
    time_t convertDateToTimeT(const std::string& dateStr) {
        std::tm tm = {};
        std::istringstream ss(dateStr);
        ss >> std::get_time(&tm, "%m/%d/%Y");
        if (ss.fail()) {
            std::cerr << "Failed to parse date: " << dateStr << std::endl;
        }
        return mktime(&tm);
    }
}

DatabaseManager::DatabaseManager(const std::string& dbName) {
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    }
}

DatabaseManager::~DatabaseManager() {
    sqlite3_close(db);
}

void DatabaseManager::executeQuery(const std::string& query) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL Error: " << errMsg << "\nQuery: " << query << std::endl;
        sqlite3_free(errMsg);
    }
}

void DatabaseManager::initializeTables() {
    // Create regions table.
    std::string regionsTable = "CREATE TABLE IF NOT EXISTS regions ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "regionName TEXT UNIQUE);";
    executeQuery(regionsTable);

    // Create customers table.
    std::string customersTable = "CREATE TABLE IF NOT EXISTS customers ("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                 "name TEXT UNIQUE, "
                                 "address TEXT, "
                                 "region TEXT, "
                                 "FOREIGN KEY(region) REFERENCES regions(regionName));";
    executeQuery(customersTable);

    // Create billing records table (dates stored as TEXT in mm/dd/yyyy format).
    std::string billsTable = "CREATE TABLE IF NOT EXISTS bills ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "customerName TEXT, "
                             "utility TEXT, "
                             "usageAmount REAL, "
                             "totalCost REAL, "
                             "billingDate TEXT, "
                             "paymentDate TEXT DEFAULT '-1', "
                             "FOREIGN KEY(customerName) REFERENCES customers(name));";
    executeQuery(billsTable);
}

void DatabaseManager::addRegion(const std::string& regionName) {
    std::string query = "INSERT OR IGNORE INTO regions (regionName) VALUES ('" + regionName + "');";
    executeQuery(query);
}

void DatabaseManager::addCustomer(const std::string& name, const std::string& address, const std::string& region) {
    std::string query = "INSERT OR IGNORE INTO customers (name, address, region) VALUES ('" +
                        name + "', '" + address + "', '" + region + "');";
    executeQuery(query);
}

void DatabaseManager::addBillingRecord(const std::string& customerName, const std::string& utility, double usage, double totalCost, const std::string& billingDate) {
    std::string query = "INSERT INTO bills (customerName, utility, usageAmount, totalCost, billingDate) "
                        "VALUES ('" + customerName + "', '" + utility + "', " + std::to_string(usage) +
                        ", " + std::to_string(totalCost) + ", '" + billingDate + "');";
    executeQuery(query);
}

void DatabaseManager::recordPayment(const std::string& customerName, const std::string& utility, const std::string& paymentDate) {
    std::string query = "UPDATE bills SET paymentDate = '" + paymentDate +
                        "' WHERE customerName = '" + customerName + "' AND utility = '" + utility +
                        "' AND paymentDate = '-1';";
    executeQuery(query);
}

std::vector<UsageRecord> DatabaseManager::getUsageRecords() {
    std::vector<UsageRecord> records;
    std::string query = "SELECT customerName, utility, usageAmount, totalCost, paymentDate FROM bills;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            bool isPaid = (std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))) != "-1");
            records.push_back({
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                sqlite3_column_double(stmt, 2),
                sqlite3_column_double(stmt, 3),
                isPaid
            });
        }
    }
    sqlite3_finalize(stmt);
    return records;
}

std::vector<UsageRecord> DatabaseManager::getOverdueRecords(const std::string& currentDate) {
    std::vector<UsageRecord> records;
    // Query unpaid bills.
    std::string query = "SELECT customerName, utility, usageAmount, totalCost, billingDate FROM bills WHERE paymentDate = '-1';";
    sqlite3_stmt* stmt;
    
    time_t currentTime = convertDateToTimeT(currentDate);
    
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string billingDateStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            time_t billingTime = convertDateToTimeT(billingDateStr);
            double diffInSeconds = difftime(currentTime, billingTime);
            int diffInDays = diffInSeconds / (60 * 60 * 24);
            
            if (diffInDays > 30) { // Overdue condition
                records.push_back({
                    reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
                    reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                    sqlite3_column_double(stmt, 2),
                    sqlite3_column_double(stmt, 3),
                    false
                });
            }
        }
    }
    sqlite3_finalize(stmt);
    return records;
}



std::vector<std::string> DatabaseManager::loadRegions() {
    std::vector<std::string> regions;
    std::string query = "SELECT regionName FROM regions;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* text = sqlite3_column_text(stmt, 0);
            if (text) {
                regions.push_back(reinterpret_cast<const char*>(text));
            }
        }
    }
    sqlite3_finalize(stmt);
    return regions;
}

//Member3:ZiheZhao
#ifndef USAGERECORD_H
#define USAGERECORD_H

#include <string>

struct UsageRecord {
    std::string customerName;
    std::string utilityType;
    double usageAmount;
    double totalCost;
    bool isPaid;
};

#endif

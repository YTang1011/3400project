#include "UtilityProviders.h"
#include <iostream>

UtilityProvider::UtilityProvider(const std::string& n, double price) 
    : name(n), basePrice(price) {}

double UtilityProvider::calculateCost(double usage) const {
    return basePrice * usage;
}

void UtilityProvider::display() const {
    std::cout << "Utility: " << name << " | Base Price: $" << basePrice << " per unit" << std::endl;
}

std::string UtilityProvider::getName() const { return name; }

CrudeOilProvider::CrudeOilProvider(double price) 
    : UtilityProvider("Crude Oil", price) {}

SolarProvider::SolarProvider(double price) 
    : UtilityProvider("Solar", price) {}

NuclearProvider::NuclearProvider(double price) 
    : UtilityProvider("Nuclear", price) {}

NaturalGasProvider::NaturalGasProvider(double price) 
    : UtilityProvider("Natural Gas", price) {}
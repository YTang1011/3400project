#ifndef UTILITYPROVIDERS_H
#define UTILITYPROVIDERS_H

#include <string>

class UtilityProvider {
protected:
    std::string name;
    double basePrice;
public:
    UtilityProvider(const std::string& n, double price);
    virtual double calculateCost(double usage) const;
    virtual void display() const;
    std::string getName() const;
};

class CrudeOilProvider : public UtilityProvider {
public:
    CrudeOilProvider(double price);
};

class SolarProvider : public UtilityProvider {
public:
    SolarProvider(double price);
};

class NuclearProvider : public UtilityProvider {
public:
    NuclearProvider(double price);
};

class NaturalGasProvider : public UtilityProvider {
public:
    NaturalGasProvider(double price);
};

#endif

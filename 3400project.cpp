//classes for utility providers + pricing
//set up how customers pick utilities
//cost calculations

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class UtilityProvider {
protected:
    string name;
    double basePrice; 
public:
    UtilityProvider(const string& n, double price) : name(n), basePrice(price) {}
    
    virtual double calculateCost(double usage) const {
        return basePrice * usage;
    }
    
    virtual void display() const {
        cout << "Utility: " << name << " | Base Price: $" << basePrice << " per unit" << endl;
    }
    
    string getName() const { return name; }
};

class CrudeOilProvider : public UtilityProvider {
public:
    CrudeOilProvider(double price) : UtilityProvider("Crude Oil", price) {}
};

class SolarProvider : public UtilityProvider {
public:
    SolarProvider(double price) : UtilityProvider("Solar", price) {}
};

class NuclearProvider : public UtilityProvider {
public:
    NuclearProvider(double price) : UtilityProvider("Nuclear", price) {}
};

class NaturalGasProvider : public UtilityProvider {
public:
    NaturalGasProvider(double price) : UtilityProvider("Natural Gas", price) {}
};

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
    //sample base prices
    vector<UtilityProvider*> providers;
    providers.push_back(new CrudeOilProvider(2.50));     
    providers.push_back(new SolarProvider(1.75));          
    providers.push_back(new NuclearProvider(3.00));        
    providers.push_back(new NaturalGasProvider(2.00));     

    int selectedIndex = displayMenuAndGetChoice(providers);
    if (selectedIndex < 0 || selectedIndex >= providers.size()) {
        cout << "Invalid selection!" << endl;
        return 1;
    }
    
    UtilityProvider* selectedProvider = providers[selectedIndex];
    cout << "\nYou selected: " << selectedProvider->getName() << endl;

    double usage;
    cout << "Enter usage (in units): ";
    cin >> usage;
    
    double cost = selectedProvider->calculateCost(usage);
    cout << "The cost for " << usage << " units of " 
         << selectedProvider->getName() << " is: $" << cost << endl;

    for (auto provider : providers) {
        delete provider;
    }
    
    return 0;
}

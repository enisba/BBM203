#ifndef REALMSHAPER_H
#define REALMSHAPER_H

#include <string>
#include <vector>
#include "Isle.h"

class RealmShaper
{
private:
    const std::string name;        // Name of the player
    int collectedEnergyPoints = 0; // Energy collected from Items
    int honour = 1000;             // Honour player holds, if 0 player should be eracted from the rankings
    Isle* currentIsle = nullptr; 

public:
    RealmShaper(std::string name, int honour); // Constructor declaration
    // Getters
    const std::string &getName() const; // Name getter
    int getHonour();                    // Honour getter
    int getEnergyLevel();               // Energy getter
    Isle* getCurrentIsle() const;
    void setCurrentIsle(Isle* isle);
    void gainHonour();           // If duel is won, +150
    void loseHonour();           // If duel is lost, -150
    void collectItem(Item item); // Collect energy from item
    void loseEnergy();           // Energy is lost after Isle crafting
    bool hasEnoughEnergy();      // Checks if player has energy for Isle crafting
    bool operator==(const RealmShaper &other) const;
    friend std::ostream &operator<<(std::ostream &os, const RealmShaper &p);
    static std::vector<RealmShaper *> readFromFile(const std::string &filename);
};

#endif
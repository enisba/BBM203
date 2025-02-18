#include "RealmShaper.h"
#include <fstream>
#include <sstream>
#include <iostream>
#define NECESSARY_ENERGY 2000 

RealmShaper::RealmShaper(std::string name, int honour) : name(name)
{
    this->honour = honour;
}

const std::string &RealmShaper::getName() const
{
    return this->name;
}

int RealmShaper::getEnergyLevel()
{
    return this->collectedEnergyPoints;
}

Isle* RealmShaper::getCurrentIsle() const {
    return currentIsle;
}

void RealmShaper::setCurrentIsle(Isle* isle) {
    currentIsle = isle;
}

int RealmShaper::getHonour()
{
    return this->honour;
}

void RealmShaper::gainHonour()
{
    this->honour += 150;
}

void RealmShaper::loseHonour() {
    this->honour -= 150;
    if (this->honour < 0) {
        this->honour = 0;
    }
}

void RealmShaper::collectItem(Item item)
{
    if(item> 0){
        this->collectedEnergyPoints += item;
    }   
}

void RealmShaper::loseEnergy()
{
    if(this->collectedEnergyPoints >= NECESSARY_ENERGY){
        this->collectedEnergyPoints -= NECESSARY_ENERGY;
    }else{
    }
}  

bool RealmShaper::hasEnoughEnergy()
{
    return this->collectedEnergyPoints >= NECESSARY_ENERGY;
}

std::vector<RealmShaper *> RealmShaper::readFromFile(const std::string &filename)
{
    std::vector<RealmShaper *> players;
    std::ifstream file(filename);
    if (!file.is_open())return players;   
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string playerName;
        int honourPoints;
        if (iss>>playerName>>honourPoints)
        {
            RealmShaper *player = new RealmShaper(playerName, honourPoints);
            players.push_back(player);
        }
    }
    file.close();
    return players;
}
bool RealmShaper::operator==(const RealmShaper &other) const
{
    return this->name == other.name;
}
std::ostream &operator<<(std::ostream &os, const RealmShaper &p)
{
    // Outs the player name
    return (os << p.name); // << std::endl);
}
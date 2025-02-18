#include "Isle.h"

Isle::Isle(std::string name) : name(name){}
const std::string& Isle::getName() const
{
    return this->name;
}
Item Isle::getItem()
{
    return item;
}
void Isle::setItem(Item item)
{
    this->item = item;
}
int Isle::getShaperCount()
{
    return shapers.size();
}
bool Isle::addShaper(const std::string &shaperName) {
    return shapers.insert(shaperName).second; // Yeni eklenmişse true döner
}
bool Isle::removeShaper(const std::string &shaperName) {
    return shapers.erase(shaperName) > 0; // Silinen varsa true döner
}
bool Isle::increaseShaperCount(){

   if (shaperCount < capacity)
    {
        shaperCount++;
        return false;
    }
    return true; 

}

bool Isle::decreaseShaperCount()
{
    bool isEmpty = true;

    if (shaperCount > 0)
    {
        shaperCount--;
        isEmpty = false;
    }

    return isEmpty;
}

bool Isle::operator==(const Isle &other) const
{
    return this->name == other.name;
}

bool Isle::operator<(const Isle &other) const
{
   return this->name < other.name;
}

bool Isle::operator>(const Isle &other) const
{
    return this->name > other.name; 
}

// Implementation of readFromFile
std::vector<Isle *> Isle::readFromFile(const std::string &filename)
{
    std::vector<Isle *> isles;
    std::ifstream   file(filename);

    if (!file.is_open())return isles;

    std::string line;
    while(std::getline(file, line)){
        if(line.empty())continue;

        Isle *isle = new Isle(line);
        isles.push_back(isle);
 
    }
    file.close();
    

    return isles;
}

std::ostream &operator<<(std::ostream &os, const Isle &p)
{
    // Prints to terminal with color
    // This function might cause some problems in terminals that are not Linux based
    // If so, comment out the colors while testing on local machine
    // But open them back up while submitting or using TurBo

    std::string einsteiniumColor = "\033[38;2;6;134;151m";
    std::string goldiumColor = "\033[38;2;255;198;5m";
    std::string amazoniteColor = "\033[38;2;169;254;255m";
    std::string resetColorTag = "\033[0m";

    if (p.item == EINSTEINIUM)
        return (os << einsteiniumColor << p.name << resetColorTag);
    if (p.item == GOLDIUM)
        return (os << goldiumColor << p.name << resetColorTag);
    if (p.item == AMAZONITE)
        return (os << amazoniteColor << p.name << resetColorTag);
    return (os << p.name);
}
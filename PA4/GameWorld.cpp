#include "GameWorld.h"

GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
}
Map& GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree& GameWorld::getShaperTree()
{
    return shaperTree;
}

// hasAcces function checks if a player has access to a specific isle
bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle) {
    if (!realmShaper || !isle) {
        std::cerr << "[Access Control] " << "RealmShaper not found!\n" ;
        return false;
    }
    int playerDepth = shaperTree.getDepth(realmShaper);
    if (playerDepth == -1)
    {
        std::cerr << "[Access Control] " << "RealmShaper not found!\n" ;
        return false;
    }
    int isleDepth = mapTree.getIsleDepth(isle);
if (isleDepth == -1) {
    std::cerr << "[Access Control] Isle not found!\n";
    return false;
}
    int totalShaperDepth = shaperTree.getDepth();
    int totalMapDepth = mapTree.getDepth();
if (totalShaperDepth < 0 || totalMapDepth < 0) {
    std::cerr << "[Access Control] Invalid tree depths!\n";
    return false;
}
    if (playerDepth == 0) return true; 
    if (playerDepth == totalShaperDepth - 1 && totalShaperDepth > 0) { 
        if (isleDepth == totalMapDepth - 1)return true;
        else return false;
    }
    int acces = mapTree.calculateMinMapDepthAccess(playerDepth, totalShaperDepth, totalMapDepth);
    return isleDepth >= acces;
}

// exploreArea function is used to move a player to a specific isle
void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle) {
    if (!realmShaper || !isle) {
                std::cout << "[Explore Area] " << realmShaper->getName() 
                  << " does not have access to explore area " << *isle << std::endl;
        return;
    }
    if (!hasAccess(realmShaper, isle)) {
        std::cout << "[Explore Area] " << realmShaper->getName() 
                  << " does not have access to explore area " << *isle << std::endl;
        return;
    }
    if (realmShaper->getCurrentIsle() && realmShaper->getCurrentIsle() != isle) {
        bool wasEmpty = realmShaper->getCurrentIsle()->decreaseShaperCount();
        realmShaper->getCurrentIsle()->removeShaper(realmShaper->getName()); 
        if (wasEmpty) {
            std::cout << "[Empty Isle] " << realmShaper->getName() 
                      << " left " << realmShaper->getCurrentIsle()->getName() << std::endl;
        }
    }
    realmShaper->setCurrentIsle(isle);
    if (isle->addShaper(realmShaper->getName())) {
        bool isCrowded = isle->increaseShaperCount();
        if (isCrowded) {
            std::cout << "[Owercrowding] " << isle->getName() 
          << " self-destructed, it will be removed from the map." << std::endl;
            mapTree.remove(isle);
            realmShaper->setCurrentIsle(nullptr); 
            return; 
        }
    }
    if (isle->getItem() != EMPTY) {
        realmShaper->collectItem(isle->getItem());
        isle->setItem(EMPTY);
    }
    std::cout << "[Explore Area] " << realmShaper->getName() 
              << " visited " << isle->getName() << std::endl;
    std::cout << "[Energy] " << realmShaper->getName() 
              << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
}
// craft function is used to craft a new isle
void GameWorld::craft(RealmShaper *shaper, const std::string &isleName){
if (!shaper->hasEnoughEnergy()) {
    std::cout << "[Energy] " << shaper->getName() 
              << " does not have enough energy points: " 
              << shaper->getEnergyLevel() << std::endl;
    return;
}
    Isle *newIsle = new Isle(isleName);
    mapTree.insert(newIsle);
    shaper->loseEnergy();
    std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;
std::cout << "[Energy] " << shaper->getName() 
          << " has enough energy points: " 
          << shaper->getEnergyLevel() << std::endl;
          
    }



void GameWorld::displayGameState()
{
    // mapTree.displayMap();
    // shaperTree.displayTree();
}

// processGameEvents function reads the access and duel logs and processes them
void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs)
{
    std::ifstream accessFile(accessLogs);
    std::ifstream duelFile(duelLogs);

    if (!accessFile.is_open() || !duelFile.is_open()) return;
    std::string accessLine, duelLine;
    int accessCount = 0;
    while (std::getline(accessFile, accessLine))
    {
        if (accessLine.empty() || accessLine[0] == '#')continue; 
        std::istringstream iss(accessLine);
        std::string playerName, placeName;
        if (!(iss >> playerName >> placeName))continue;
        RealmShaper *player = shaperTree.findPlayer(playerName);
        Isle *place = mapTree.findIsle(placeName);

        if (!player) {
            std::cerr << "[Error] Player not found in access log: " << playerName << std::endl;
            continue;
        }

        if (!place)
        {
            if (player->hasEnoughEnergy())
            {
                craft(player, placeName); 
                place = mapTree.findIsle(placeName);
            }
            else
            {
                std::cerr << "[Error] Isle not found, and player cannot craft: " << placeName << std::endl;
                continue;
            }
        }
        exploreArea(player, place);

        if (++accessCount % 5 == 0 && std::getline(duelFile, duelLine))processDuelLine(duelLine);
    }

    while (std::getline(duelFile, duelLine))processDuelLine(duelLine);
    accessFile.close();
    duelFile.close();
    displayGameState();
}

// processDuelLine function processes a single line from the duel logs
void GameWorld::processDuelLine(const std::string &duelLine)
{
    if (duelLine.empty() || duelLine[0] == '#') return; 
    std::string cleanedLine = trim(duelLine);
    std::istringstream duelIss(cleanedLine);
    std::string challengerName;
    int resultInt;
    if (!(duelIss >> challengerName >> resultInt))return;
    if (resultInt != 0 && resultInt != 1) {
        std::cerr << "[Error] Invalid duel result in log: " << duelLine << std::endl;
        return;
    }
    RealmShaper *challenger = shaperTree.findPlayer(challengerName);
    if (!challenger) {
        std::cerr << "[Error] Challenger not found in duel log: " << challengerName << std::endl;
        return;
    }
    shaperTree.duel(challenger, resultInt == 1);
}
// trim function is used to remove leading and trailing whitespaces from a string
std::string GameWorld::trim(const std::string &str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return ""; // Tüm karakterler boşluksa
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}
void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}
#include "RealmShapers.h"
#include <cmath>
#include <algorithm>
#include <queue>
#include <iostream>
#include <fstream>
#include <vector>

ShaperTree::ShaperTree(){}
ShaperTree::~ShaperTree() {
    for (auto *shaper : realmShapers) {
        if (shaper) {
            delete shaper;
            shaper = nullptr;
        }
    }
    realmShapers.clear();
}
void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    for (RealmShaper *shaper : shapers)this->insert(shaper);
}

int ShaperTree::getSize()
{
    return realmShapers.size();
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    bool isValid = false;
    if (index >= 0 && index < realmShapers.size())
        isValid = true;
    return isValid;
}

void ShaperTree::insert(RealmShaper *shaper)
{
    realmShapers.push_back(shaper);
}

// remove shaper from the tree
int ShaperTree::remove(RealmShaper *shaper) {
    int ind = findIndex(shaper);
    if (ind == -1)     return -1; 
        if (realmShapers[ind]) {
        delete realmShapers[ind];
        realmShapers[ind] = nullptr;
    }
    realmShapers.erase(realmShapers.begin() + ind);
    return ind;
}
// find the index of a shaper in the tree
int ShaperTree::findIndex(RealmShaper *shaper)
{
    if (!shaper)return -1;
    int i ;
    for(i=0;i<realmShapers.size();i++)if(realmShapers[i]==shaper)return i;
    return -1;
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    int ind = findIndex(shaper);
    if(ind==-1)return -1;  
    int depth = 0;
    while(ind>0){
        ind = (ind-1)/2;
        depth++;
    }
    return depth;
}

int ShaperTree::getDepth() {
    if (realmShapers.empty()) return -1; 
    int n = realmShapers.size();
    if (n == 0) return -1;
    return std::floor(std::log2(n)) + 1;
}

// duel between two shapers
RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    int challengerInd = findIndex(challenger);
    if (challengerInd == -1 || challengerInd == 0) {
        return *challenger;
    }
    int parentInd = (challengerInd - 1) / 2;
    RealmShaper *parent = realmShapers[parentInd];
    if (result) {
        challenger->gainHonour(); 
        parent->loseHonour();   
        std::cout << "[Duel] " << challenger->getName() << " won the duel" << std::endl;
        std::cout << "[Honour] New honour points: " 
                  << challenger->getName() << "-" << challenger->getHonour() << " " 
                  << parent->getName() << "-" << parent->getHonour() << std::endl;
        std::swap(realmShapers[challengerInd], realmShapers[parentInd]);
        if (parent->getHonour() <= 0) {
            std::cout << "[Duel] " << parent->getName() << " lost all honour, delete" << std::endl;
            remove(parent);
        }
        return *challenger;
    } else {
        challenger->loseHonour(); 
        parent->gainHonour();      
        if (challenger->getHonour() <= 0) {
            std::cout << "[Duel] " << challenger->getName() << " lost all honour, delete" << std::endl;
            remove(challenger);
        }
        return *parent;
    }
}
RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    int ind = findIndex(shaper);
    if (ind<=0)return nullptr;
    int parentInd = (ind-1)/2;
    return realmShapers[parentInd];
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    int lowInd = findIndex(player_low);
    int highInd = findIndex(player_high);
    if (lowInd==-1 || highInd==-1)return;
    std::swap(realmShapers[lowInd],realmShapers[highInd]);
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    for (auto *exist : realmShapers)if (exist && *exist == shaper) return exist;
    return nullptr;
}
RealmShaper *ShaperTree::findPlayer(std::string name)
{
    for (auto* player : realmShapers) {
        if (player && player->getName() == name) {
            return player;
        }
    }
    return nullptr;
}

// In-order traversal of the tree
std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    if (!isValidIndex(index))return result;
    std::vector<std::string> leftResult = inOrderTraversal(2*index + 1);
    result.insert(result.end(), leftResult.begin(), leftResult.end());
    result.push_back(realmShapers[index]->getName());
    std::vector<std::string> rightResult = inOrderTraversal(2*index + 2);
    result.insert(result.end(), rightResult.begin(), rightResult.end());
    return result;
}

// Pre-order traversal of the tree
std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    if (!isValidIndex(index))return result;
    result.push_back(realmShapers[index]->getName());
    std::vector<std::string> leftResult = preOrderTraversal(2*index + 1);
    result.insert(result.end(), leftResult.begin(), leftResult.end());
    std::vector<std::string> rightResult = preOrderTraversal(2*index + 2);
    result.insert(result.end(), rightResult.begin(), rightResult.end());
    return result;
}

// Post-order traversal of the tree
std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    if (!isValidIndex(index))return result;
    std::vector<std::string> leftResult = postOrderTraversal(2*index + 1);
    result.insert(result.end(), leftResult.begin(), leftResult.end());
    std::vector<std::string> rightResult = postOrderTraversal(2*index + 2);
    result.insert(result.end(), rightResult.begin(), rightResult.end());
    result.push_back(realmShapers[index]->getName());
    return result;
}

//  Pre-order traversal of the tree
void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    if (!isValidIndex(index))return; 
    outFile<< realmShapers[index]->getName() << std::endl;
    preOrderTraversal(2*index + 1, outFile);   
    preOrderTraversal(2*index + 2, outFile);
}

// Breadth-first traversal of the tree
void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{

    if (realmShapers.empty())return;
    std::queue<int> queue;
    queue.push(0);
    while(!queue.empty()){
        int currInd = queue.front();
        queue.pop();
        if (isValidIndex(currInd)){
            outFile << realmShapers[currInd]->getName() << std::endl;
            if (isValidIndex(2*currInd + 1))queue.push(2*currInd + 1);
            if (isValidIndex(2*currInd + 2))queue.push(2*currInd + 2);
        }
    }
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;
    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = 2 * index + 1; 
    int right = 2 * index + 2;
    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    std::ofstream outFile(filename);
    if (!outFile.is_open())return;  
    breadthFirstTraversal(outFile);
    outFile.close();
    std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
}

void ShaperTree::writeToFile(const std::string &filename)
{
    std::ofstream outFile(filename);
    if (!outFile.is_open())return;
    preOrderTraversal(0, outFile);
    outFile.close();
    std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;
}

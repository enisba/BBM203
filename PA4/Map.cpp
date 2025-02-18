#include "Map.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

Map::Map()
{
    this->root = nullptr;
    rebalanceCount = 0;
    isInitializing = false;
}

// destructor to delete the nodes
Map::~Map() {
    std::queue<MapNode *> nodes;
    if (root) nodes.push(root);

    while (!nodes.empty()) {
        MapNode *current = nodes.front();
        nodes.pop();

        if (current->left) nodes.push(current->left);
        if (current->right) nodes.push(current->right);

        if (current->isle) {
            delete current->isle;
            current->isle = nullptr;
        }

        delete current;
    }
    root = nullptr;
}
// initialize the map with isles isInitializing is used to prevent rebalancing during initialization
void Map::initializeMap(std::vector<Isle *> isles) {
    isInitializing = true; 
    for (Isle *isle : isles) insert(isle); 
    populateWithItems();
    rebalanceCount = 0;
    isInitializing = false;
}
// rotate the tree to the right
MapNode *Map::rotateRight(MapNode *current)
{
    if(!current || !current->left){
        std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
        return current;
    }
    MapNode *newRoot = current->left;
    current->left = newRoot->right;
    newRoot->right = current;
    current->height = std::max(height(current->left), height(current->right))+1;
    newRoot->height = std::max(height(newRoot->left), height(current))+1;
    return newRoot;
}
// rotate the tree to the left
MapNode *Map::rotateLeft(MapNode *current)
{
    if(!current || !current->right){
        std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
    return current;
    }
    MapNode *newRoot = current->right;
    current->right = newRoot->left;
    newRoot->left = current;
    current->height = std::max(height(current->left), height(current->right))+1;
    newRoot->height = std::max(height(newRoot->right), height(current))+1;
    return newRoot;
}
// calculate the minimum depth a player can access at the map
int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth) {
    if (totalShaperTreeHeight <= 0) return 0; 
    return std::round(totalMapDepth * playerDepth / totalShaperTreeHeight);
}
// calculate the height of a node
int Map::height(MapNode *node)
{
    if (node==nullptr)return 0;
    return node->height;
}
// insert a new isle to the map isInitializing is used to prevent rebalancing during initialization
MapNode* Map::insert(MapNode *node, Isle *isle) {
    if (node == nullptr) return new MapNode(isle);
    if (isle->getName() < node->isle->getName())node->left = insert(node->left, isle);
    else if (isle->getName() > node->isle->getName())node->right = insert(node->right, isle);
    else return node;
    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = height(node->left) - height(node->right);
    if (balance > 1 && isle->getName() < node->left->isle->getName()) {
        if (!isInitializing) {
            rebalanceCount++;
            if (rebalanceCount % 3 == 0) populateWithItems();
        }
        return rotateRight(node);
    }
    if (balance < -1 && isle->getName() > node->right->isle->getName()) {
        if (!isInitializing) {
            rebalanceCount++;
            if (rebalanceCount % 3 == 0) populateWithItems();
        }
        return rotateLeft(node);
    }
    if (balance > 1 && isle->getName() > node->left->isle->getName()) {
        if (!isInitializing) {
            rebalanceCount++;
            if (rebalanceCount % 3 == 0) populateWithItems();
        }
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && isle->getName() < node->right->isle->getName()) {
        if (!isInitializing) {
            rebalanceCount++;
            if (rebalanceCount % 3 == 0) populateWithItems();
        }
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}
void Map::insert(Isle *isle)
{
    root = insert((root), isle);
    // you might need to insert some checks / functions here depending on your implementation
}
// remove an isle from the map
MapNode* Map::remove(MapNode* node, Isle* isle) {
    if (!node) {
        std::cout << "[Remove] Tree is Empty" << std::endl;
        return nullptr;
    }
    if (isle->getName() < node->isle->getName())node->left = remove(node->left, isle);
    else if (isle->getName() > node->isle->getName()) node->right = remove(node->right, isle);
    else {
        if (!node->left && !node->right) {
            return node;
        } else if (!node->left) {
            MapNode* rightChild = node->right;
            return rightChild;
        } else if (!node->right) {
            MapNode* leftChild = node->left;
            return leftChild;
        } else {
            MapNode* temp = node->left;
            while (temp->right) temp = temp->right;
            std::string predecessorName = temp->isle->getName();
            Item predecessorItem = temp->isle->getItem();
            delete node->isle;
            node->isle = new Isle(predecessorName);
            node->isle->setItem(predecessorItem);
            Isle dummyIsle(predecessorName);
            node->left = remove(node->left, &dummyIsle);
        }
    }
    // AVL tree height update
    node->height = std::max(height(node->left), height(node->right)) + 1;
    int balance = height(node->left) - height(node->right);
    if (balance > 1 && height(node->left->left) >= height(node->left->right)) {
        if (!isInitializing) {
            rebalanceCount++;
            if (rebalanceCount % 3 == 0) populateWithItems();
        }
        return rotateRight(node);
    }
    if (balance > 1 && height(node->left->left) < height(node->left->right)) {
        if (!isInitializing) {
            rebalanceCount++;
            if (rebalanceCount % 3 == 0) populateWithItems();
        }
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && height(node->right->right) >= height(node->right->left)) {
        if (!isInitializing) {
            rebalanceCount++;
            if (rebalanceCount % 3 == 0) populateWithItems();
        }
        return rotateLeft(node);
    }
    if (balance < -1 && height(node->right->right) < height(node->right->left)) {
        if (!isInitializing) {
            rebalanceCount++;
            if (rebalanceCount % 3 == 0) populateWithItems();
        }
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

void Map::remove(Isle *isle)
{
    root = remove((root), isle);
    // you might need to insert some checks / functions here depending on your implementation
}
// to Display the values by Pre Order Method .. node - left - right
void Map::preOrderItemDrop(MapNode* current, int& count) {
    if (!current ||!current->isle) return;
    count++;
    if (count % 5 == 0 && current->isle->getItem() != AMAZONITE ) {
        current->isle->setItem(EINSTEINIUM);
        std::cout << "[Item Drop] EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }
    preOrderItemDrop(current->left, count);
    preOrderItemDrop(current->right, count);
}

// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode* current, int& count) {
    if (!current || !current->isle) return;
    postOrderItemDrop(current->left, count);
    postOrderItemDrop(current->right, count);
    count++;
    if (count % 3 == 0 && current->isle->getItem() != AMAZONITE) {
        current->isle->setItem(GOLDIUM);
        std::cout << "[Item Drop] GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }
}
// find the first empty isle in the map
MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    if(!node) return nullptr;
    if(node->isle->getItem() == EMPTY)return node; 
    MapNode* leftRes =  findFirstEmptyIsle(node->left);
    if(leftRes)return leftRes;
    return findFirstEmptyIsle(node->right);
}
// drop an Amazonite item to the first empty isle in the map
void Map::dropItemBFS() {
    if (!root) {
        std::cout << "[BFS Drop] No eligible Isle found for AMAZONITE drop." << std::endl;
        return;
    }
    std::queue<MapNode*> q;
    q.push(root);
    while (!q.empty()) {
        MapNode* current = q.front();
        q.pop();
        if (current->isle->getItem() == EMPTY) {
            current->isle->setItem(AMAZONITE);
            std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << current->isle->getName() << std::endl;
            return; 
        }
        if (current->left)  q.push(current->left);
        if (current->right) q.push(current->right);
    }
   std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;
}
void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}
int Map::getDepth(MapNode *node)
{
    if (!node)return 0;
    return std::max(getDepth(node->left),getDepth(node->right))+1;
}
// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    MapNode *curr = root;
    int depth = 0;
    while(curr){
        if(isle->getName()< curr->isle->getName()){
            curr= curr->left;
            depth++;
        }
        else if (isle->getName()> curr->isle->getName()){
            curr = curr->right;
            depth++;
        }
        else return depth;
        }
    return -1;
}

int Map::getDepth()
{
    return getDepth(root);
}

// Function to populate the map with items dropItemBFS is called every 3 rebalances
void Map::populateWithItems() {
    if (rebalanceCount == 0) {
        int goldiumCount = 0;
        int einsteiniumCount = 0;
        postOrderItemDrop(root, goldiumCount); 
        preOrderItemDrop(root, einsteiniumCount); 
    } else if (rebalanceCount % 3 == 0) {
        int goldiumCount = 0;
        int einsteiniumCount = 0;
        postOrderItemDrop(root, goldiumCount);
        preOrderItemDrop(root, einsteiniumCount); 
        dropItemBFS();
    }
}
Isle *Map::findIsle(Isle isle)
{
    MapNode * finde = findNode(isle);
    if(finde)return finde->isle;
    return nullptr;
}
Isle *Map::findIsle(std::string name)
{
    MapNode *finde = findNode(name);
    if(finde)return finde->isle;
    return nullptr;
}
MapNode *Map::findNode(Isle isle)
{
    MapNode* curr = root;
    while(curr){
        if(isle.getName() < curr->isle->getName())curr = curr->left;
        else if(isle.getName() > curr->isle->getName())curr = curr->right;
        else return curr;
    }
    return nullptr;
}

MapNode *Map::findNode(std::string name)
{
    MapNode *current = root;
    while(current){
        if(name < current->isle->getName())current = current->left;
        else if(name > current->isle->getName())current = current->right;
        else return current;
    }
    return nullptr;
}

void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}

// Function to read the map from a file
void Map::writeToFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
        return;
    std::queue<MapNode*> queue;
    if(root)queue.push(root);
    while(!queue.empty()){
        MapNode* current = queue.front();
        queue.pop();
        file << current->isle->getName() << std::endl;
        if (current->left)queue.push(current->left);
        if(current->right)queue.push(current->right);
    }
    file.close();
}

// Function to write the map to a file
void Map::writeIslesToFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())return ;
    std::vector<std:: string> isles;
    std::queue<MapNode*> queue;
    if(root)queue.push(root);
    while(!queue.empty()){
        MapNode *current = queue.front();
        queue.pop();
        isles.push_back(current->isle->getName());
        if(current->left)queue.push(current->left);
        if(current->right)queue.push(current->right);
    }
    std::sort(isles.begin(),isles.end());
    for (const auto &ada: isles)file<<ada<<std::endl;
    file.close();
    std::cout << "[Output] " << "Isles have been written to " << filename << std::endl;
}

#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map() {
    // Initialize all distances to a value representing no direct connection
    // Initialize all provinces as unvisited
    int i ,j;
    for (i = 0; i< MAX_SIZE;i++){
        for(j = 0; j < MAX_SIZE;j++){
            distanceMatrix[i][j] = -1;
        }
        visited[i]= false;
    }
    resetVisited();
    
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // Read each line in the CSV file
    // Read each cell separated by a comma
    // Convert cell to an integer and store in distanceMatrix
    std::ifstream file(filename);
    if(!file.is_open())return;

    std::string  line;
    int i = 0;
    while(i < MAX_SIZE && std::getline(file, line)){
        std::istringstream iss(line);
        std::string x;
        int j = 0;
        while(std::getline(iss, x, ',') && j < MAX_SIZE){
            distanceMatrix[i][j] = std::stoi(x);
            ++j;
        }
        ++i;
    }
    file.close();
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    if(distanceMatrix[provinceA][provinceB]== -1)return false ; // there is no any link
    return distanceMatrix[provinceA][provinceB] <=maxDistance; 
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    if (province >= 0 && province < MAX_SIZE)visited[province] = true;
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    if (province >= 0 && province < MAX_SIZE) return visited[province];
    else return false;
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    int i;
    for(i = 0; i < MAX_SIZE; i++){
        visited[i] = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    int number_of_visited = 0;
    int i;
    for(i=0;i < MAX_SIZE; i++){
        if (visited[i]) number_of_visited+= 1;
    }
    return number_of_visited;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    if(provinceA < MAX_SIZE && provinceB < MAX_SIZE &&
    provinceA >= 0 && provinceB >= 0) return distanceMatrix[provinceA][provinceB];
    return -1;
}
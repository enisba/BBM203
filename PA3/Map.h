#ifndef MAP_H
#define MAP_H

#include <string>

const int MAX_SIZE = 81;  

class Map {

public:

    int distanceMatrix[MAX_SIZE][MAX_SIZE];      
    bool visited[MAX_SIZE];  // Tracks visited provinces

    Map(); // Constructor to initialize the map

    // Loads distance data from an Excel file (or predefined format) and fills the distanceMatrix
    void loadDistanceData(const std::string& filename);

    // Checks if the distance between two provinces is within the allowed maxDistance range
    bool isWithinRange(int provinceA, int provinceB, int maxDistance) const;

    // Marks a province as visited
    void markAsVisited(int province);

    // Checks if a province has already been visited
    bool isVisited(int province) const;

    // Resets all provinces to unvisited
    void resetVisited();

    // Returns the count of visited provinces
    int countVisitedProvinces() const;  

    // Function to get the distance between two provinces
    int getDistance(int provinceA, int provinceB) const;
};

#endif // MAP_H

#include "RoutePlanner.h"
#include <iostream>
#include <fstream>

// Array to help you out with name of the cities in order
const std::string cities[81] = { 
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {
    map.loadDistanceData(distance_data);
    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);
    map.resetVisited();
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    std::string line;
    numPriorityProvinces = 0;
    while (std::getline(file, line) && numPriorityProvinces < MAX_PRIORITY_PROVINCES) {
        size_t start = line.find('(');
        size_t end = line.find(')');
        if (start != std::string::npos && end != std::string::npos) {
            try {
                // Parantez içindeki sayıyı al ve dönüştür
                std::string indStr = line.substr(start + 1, end - start - 1);
                int provinceInd = std::stoi(indStr);
                if (provinceInd >= 0 && provinceInd < 81) { // Geçerli indeks kontrolü
                    priorityProvinces[numPriorityProvinces++] = provinceInd;
                } else {
                    std::cerr << "ERROR: 1 " << provinceInd << filename << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "ERROR: 2 " << line << std::endl;
            }
        } else {
            std::cerr << "ERROR: 3 " << line << std::endl;
        }
    }

    file.close();
}



// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    std::string line;
    numWeatherRestrictedProvinces = 0;
    while (std::getline(file, line) && numWeatherRestrictedProvinces < MAX_WEATHER_RESTRICTED_PROVINCES) {
        size_t start = line.find('(');
        size_t end = line.find(')');
        if (start != std::string::npos && end != std::string::npos) {
            try {
                // Parantez içindeki sayıyı al ve dönüştür
                std::string indStr = line.substr(start + 1, end - start - 1);
                int provinceInd = std::stoi(indStr);
                if (provinceInd >= 0 && provinceInd < 81) { // Geçerli indeks kontrolü
                    weatherRestrictedProvinces[numWeatherRestrictedProvinces++] = provinceInd;
                } else {
                    std::cerr << "ERROR:1" << provinceInd << filename << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "ERROR:2 " << line << std::endl;
            }
        } else {
            std::cerr << "ERROR: 3" << line << std::endl;
        }
    }

    file.close();
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    int i = 0;
    for (i = 0; i < numPriorityProvinces ; i++){
        if(priorityProvinces[i] == province)return true;
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    for (int i = 0; i < numWeatherRestrictedProvinces; i++) {
        if (weatherRestrictedProvinces[i] == province) return true;
    }
    return false;
}


// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    map.markAsVisited(startingCity);
    stack.push(startingCity);
    route.push_back(startingCity);
    while (!stack.isEmpty()) {
        int currentCity = stack.peek();
        bool foundUnvisitedNeighbor = false;
        enqueueNeighbors(currentCity);
        while (!queue.isEmpty()) {
            int nextCity = queue.dequeue();
            if (!map.isVisited(nextCity)) {
                totalDistanceCovered += map.getDistance(currentCity, nextCity);
                map.markAsVisited(nextCity);
                stack.push(nextCity);
                route.push_back(nextCity);
                foundUnvisitedNeighbor = true;
                break;
            }
        }
        if (!foundUnvisitedNeighbor) backtrack();
        if (isExplorationComplete() && route.size() > 40) break;
    }

    // Clear the stack and queue
    while (!stack.isEmpty()) {
        stack.pop();
    }
    while (!queue.isEmpty()) {
        queue.dequeue();
    }

    // Display results if exploration was successful
    if (isExplorationComplete() && route.size() > 40) displayResults();
    else std::cerr << "ERROR:\n";
    for (int i = 0; i < numPriorityProvinces; i++) map.markAsVisited(priorityProvinces[i]);
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    enqueueNeighbors(province);
    while(!queue.isEmpty()){
        int nextCity =  queue.dequeue();
        if(!map.isVisited(nextCity)){
            totalDistanceCovered += map.getDistance(province, nextCity);
            map.markAsVisited(nextCity);
            stack.push(nextCity);
            route.push_back(nextCity);
            exploreFromProvince(nextCity);
        }
    }
    backtrack();
}

// Adds reachable neighbors to the queue
void RoutePlanner::enqueueNeighbors(int currentCity) {
    for (int i = 0; i < MAX_SIZE; i++) {
        if (map.isVisited(i)) {
            continue;
        }
        if (!map.isWithinRange(currentCity, i, maxDistance)) {
            continue;
        }
        if (isWeatherRestricted(i)) {
            continue;
        }
        if (isPriorityProvince(i)) {
            queue.enqueuePriority(i);
        } else {
            queue.enqueue(i);
        }
    }
}
// Backtracks if no further moves are possible
void RoutePlanner::backtrack() {
    stack.pop(); 
}

// Checks if the exploration is complete
bool RoutePlanner::isExplorationComplete() const {
    for (int i = 0; i < numPriorityProvinces; ++i) {
        if (!map.isVisited(priorityProvinces[i])) return false; 
    }
    return true;
}

// Displays the final results of the journey
void RoutePlanner::displayResults() const {
    std::cout << "----------------------------\n";
    std::cout << "Journey Completed!\n";
    std::cout << "----------------------------\n";
    std::cout << "Total Number of Provinces Visited: " << route.size() << "\n";
    std::cout << "Total Distance Covered: " << totalDistanceCovered << " km\n";
    std::cout << "Route Taken:\n";
    for (size_t i = 0; i < route.size(); ++i) {
        std::cout << cities[route[i]];
        if (i != route.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << " -> End\n";
    int visitedPriority = 0;
    std::cout << "\nPriority Provinces Status:\n";
    for (int i = 0; i < numPriorityProvinces; ++i) {
        std::cout << "- " << cities[priorityProvinces[i]] << " (";
        if (map.isVisited(priorityProvinces[i])) {
            std::cout << "Visited";
            visitedPriority++;
        } else std::cout << "Not Visited";
        std::cout << ")\n";
    }
    std::cout << "\nTotal Priority Provinces Visited: " << visitedPriority 
              << " out of " << numPriorityProvinces << "\n";
    if (visitedPriority == numPriorityProvinces) {
        std::cout << "Success: All priority provinces were visited.\n";
    } else {
        std::cout << "Warning: Not all priority provinces were visited.\n";
    }
}


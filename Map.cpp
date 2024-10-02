
#include "Map.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <algorithm>

// Territory class methods

Territory::Territory(std::string name, int x, int y, std::string continent, const std::vector<std::string>& adjNames) : name(name), x(x), y(y), continent(continent), adjacentTerritoryNames(adjNames) {}
    //Constructor(Name + X + Y + Continent + vertor (adjacentTerritoryNames) );

Territory::~Territory() {}

void Territory::addAdjacentTerritory(Territory* territory) {
    adjacentTerritories.push_back(territory);
}

std::string Territory::getName() const {return name;};
std::string Territory::getOwner() const {return owner;};
std::string Territory::getContinent() const {return continent;};
std::vector<std::string> Territory::getAdjacentTerritoryNames() const {return adjacentTerritoryNames;};
std::vector<Territory*> Territory::getAdjacentTerritories() const {return adjacentTerritories;};

void Territory::printTerritoryInfo() const {
    std::cout << "Territory: " << name << " (" << x << ", " << y << ") in " << continent
              << ". Adjacent territories: ";
    for (Territory* adj : adjacentTerritories) {
        std::cout << adj->getName() << " ";
    }
    std::cout << std::endl;
}

// Continent class methods

Continent::Continent(std::string name, int controlValue)
    : name(name), controlValue(controlValue) {}

Continent::~Continent(){};


void Continent::addTerritory(Territory* territory) {
    continents_Territory.push_back(territory);
}

std::vector<Territory*> Continent::getTerritories() const {
    return continents_Territory;
}

std::string Continent::getName() const {
    return name;
}

int Continent::getControlValue() const {
    return controlValue;
}

void Continent::printContinentInfo() const {
    std::cout << "Continent: " << name << " has control value: " << controlValue << std::endl;
    std::cout << "Territories in this continent: ";
    for (Territory* territory : continents_Territory) {
        std::cout << territory->getName() << " ";
    }
    std::cout << std::endl;
}

// Map class methods
Map::Map() : author("Nan"), warn("Nan"), image("Nan"), wrap("Nan"), horizontal("Nan") {}

Map::~Map() {
    // deletes all territory objects stored on heap.
    for (Territory *territory: territories) {
        delete territory;
    }
    for (Continent *continent: continents) {
        delete continent;
    }
}

void Map::setAuthor(const std::string& newAuthor) { author = newAuthor; }
void Map::setWarn(const std::string& newWarn) { warn = newWarn; }
void Map::setImage(const std::string& newImage) { image = newImage; }
void Map::setWrap(const std::string& newWrap) { wrap = newWrap; }
void Map::setHorizontal(const std::string& newHorizontal) { horizontal = newHorizontal; }

std::string Map::getAuthor() const { return author; }
std::string Map::getWarn() const { return warn; }
std::string Map::getImage() const { return image; }
std::string Map::getWrap() const { return wrap; }
std::string Map::getHorizontal() const { return horizontal; }

void Map::addTerritory(Territory* territory) {
    territories.push_back(territory);
}

void Map::addContinent(Continent* continent) {
    continents.push_back(continent);
}

std::vector<Continent*> Map::getContinents() const {
    return continents;
}

std::vector<Territory*> Map::getTerritories() const {
    return territories;
}

 Map* Map::loadMapFromFile(const std::string& filename) {
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return nullptr;
    }

    
    
    Map* map = new Map();


    std::string line;
    std::string section;
    
    // temp Territory pointer list for Add each Adjacent Territory
    std::vector<Territory*> tempTerritories;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        // Check section part in the map file
        if (line[0] == '[') {
            section = line;
            continue;
        }

        // switch case with different secton
        
        //   MAP Section
        if (section == "[Map]") {
            std::istringstream input(line);
            if (line.find("author=") == 0) {
                map->setAuthor(line.substr(7));  // update map's author
            } else if (line.find("image=") == 0) {
                map->setImage(line.substr(6)) ;   // update map's image
            } else if (line.find("wrap=") == 0) {
                map->setWrap(line.substr(5));    // update map's wrap type
            } else if (line.find("scroll=") == 0) {
                map->setHorizontal(line.substr(7)) ;  // update map's Horizontal type
            } else if (line.find("warn=") == 0) {
                map->setWarn(line.substr(5));    // update map's warn type
            }
        }
        
        //   Continents Section
        else if (section == "[Continents]") {
            std::istringstream string(line);
            std::string name;
            int controlValue;
            
            // read what befor'=' assign to name, the rest to controlValue
            if (std::getline(string, name, '=') && string >> controlValue) {
            map->addContinent(new Continent(name, controlValue));
}
        }
        
        
        //    Territories Section
        else if (section == "[Territories]") {
            std::istringstream string(line);
            std::string name, continent, adj;
            int x, y;

            // Read input line from the file 
            if (std::getline(string, name, ',') && string >> x && string.ignore() && string >> y && string.ignore() && std::getline(string, continent, ',')) {
                std::vector<std::string> adjNames;

                // Add adjacent Territory Names
                while (std::getline(string, adj, ',')) {
                    adjNames.push_back(adj);
                }

                // creat territory and add the territory to Map. 
                Territory* territory = new Territory(name, x, y, continent, adjNames);
                map->addTerritory(territory);
                tempTerritories.push_back(territory);  //And temp store the territory.
            }
        }
    }

    // Add each Adjacent Territory's pointer to each of the Territory
    for (Territory* territory : tempTerritories) {
        std::vector<std::string> adjNames = territory->getAdjacentTerritoryNames();
        for (const std::string& adjName : adjNames) {
            for (Territory* potentialAdj : tempTerritories) {
                if (potentialAdj->getName() == adjName) {
                    territory->addAdjacentTerritory(potentialAdj);  
                    break;
                }
            }
        }
    }

    file.close();
    return map;
}

bool Map::isConnectedGraph() const {
    if (territories.empty()) return true;

    std::vector<bool> visited(territories.size(), false);
    std::vector<Territory*> stack;
    stack.push_back(territories[0]);

    // Perform DFS to check if all territories are reachable
    while (!stack.empty()) {
        Territory* current = stack.back();
        stack.pop_back();

        int index = std::distance(territories.begin(), std::find(territories.begin(), territories.end(), current));
        if (!visited[index]) {
            visited[index] = true;

            for (Territory* adj : current->getAdjacentTerritories()) {
                int adjIndex = std::distance(territories.begin(), std::find(territories.begin(), territories.end(), adj));
                if (!visited[adjIndex]) {
                    stack.push_back(adj);
                }
            }
        }
    }

    // Check if all territories are visited
    return std::all_of(visited.begin(), visited.end(), [](bool v) { return v; });
}

bool Map::areContinentsConnected() const {
    for (const Continent* continent : continents) {
        std::vector<Territory*> territoriesInContinent = continent->getTerritories();

        if (territoriesInContinent.empty()) continue;

        std::vector<bool> visited(territoriesInContinent.size(), false);
        std::vector<Territory*> stack;
        stack.push_back(territoriesInContinent[0]);

        // Perform DFS to check if all territories in this continent are connected
        while (!stack.empty()) {
            Territory* current = stack.back();
            stack.pop_back();

            int index = std::distance(territoriesInContinent.begin(), std::find(territoriesInContinent.begin(), territoriesInContinent.end(), current));
            if (!visited[index]) {
                visited[index] = true;

                for (Territory* adj : current->getAdjacentTerritories()) {
                    if (adj->getContinent() == continent->getName()) {
                        int adjIndex = std::distance(territoriesInContinent.begin(), std::find(territoriesInContinent.begin(), territoriesInContinent.end(), adj));
                        if (!visited[adjIndex]) {
                            stack.push_back(adj);
                        }
                    }
                }
            }
        }

        // Check if all territories in the continent are visited
        if (!std::all_of(visited.begin(), visited.end(), [](bool v) { return v; })) {
            return false;
        }
    }

    return true;
}

bool Map::territoryBelongsToOneContinentsConnected() const {
    for (Territory* territory : territories) {
        int continentCount = 0;
        for (const Continent* continent : continents) {
            const std::vector<Territory*>& territoriesInContinent = continent->getTerritories();
            if (std::find(territoriesInContinent.begin(), territoriesInContinent.end(), territory) != territoriesInContinent.end()) {
                continentCount++;
            }
        }

        // If a territory belongs to more than one continent, return false
        if (continentCount != 1) {
            return false;
        }
    }

    return true;
}

bool Map::validate() const {
    return isConnectedGraph() && areContinentsConnected() && territoryBelongsToOneContinentsConnected();
}

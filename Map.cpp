#include <iostream>
#include <fstream>

#include "Map.h"

// remove leading & trailing whitespace (https://stackoverflow.com/a/1798170)
std::string trim(const std::string &str, const std::string &whitespace)
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

ScrollDirection getScrollDirectionFromString(const std::string &scrollDirectionString)
{
    if (scrollDirectionString == "horizontal")
        return ScrollDirection::HORIZONTAL;
    else if (scrollDirectionString == "vertical")
        return ScrollDirection::VERTICAL;
    else
        return ScrollDirection::NONE;
}

bool getBooleanFromString(const std::string &booleanString)
{
    return booleanString == "yes" || booleanString == "true" || booleanString == "1";
}

std::shared_ptr<Map> MapLoader::loadMap(const std::string &path)
{
    std::string line;
    std::ifstream mapFile(path);
    int8_t mapSection = -1;

    std::shared_ptr<Map> map = std::make_shared<Map>();
    std::unordered_map<std::string, std::vector<std::string>> adjacencyMap;

    if (!mapFile.is_open())
    {
        std::cout << "Could not open file: " << path << std::endl;
        return map;
    }

    while (std::getline(mapFile, line))
    {
        if (line.empty() || line == "\r" || line == "\n" || line == "\r\n")
            continue; // skip empty lines

        line = trim(line);

        int8_t tempSection = getMapSection(line); // 0: Map, 1: Continents, 2: Territories

        // if it's a header line, update the section and continue
        if (tempSection != -1 && tempSection != mapSection)
        {
            mapSection = tempSection;
            continue;
        }

        switch (mapSection)
        {
        case 0:
            if (!processMapLine(line, map))
                return map;
            break;

        case 1:
            if (!processContinentsLine(line, map))
                return map;
            break;

        case 2:
            if (!processTerritoriesLine(line, map, &adjacencyMap))
                return map;
            break;
        }
    }

    associateTerritories(map, adjacencyMap);

    for (const auto &t : map->territories)
    {
        std::cout << *t.second << std::endl;
    }

    return map;
}

uint8_t MapLoader::getMapSection(const std::string &line)
{
    std::smatch match;
    std::regex_search(line, match, HEADER_PATTERN);

    if (match.size() != 2)
        return -1; // invalid line, not enough matches (or too many matches)

    if (match[1].str() == "Map")
        return 0;
    else if (match[1].str() == "Continents")
        return 1;
    else if (match[1].str() == "Territories")
        return 2;
    else
        return -1; // Invalid section
}

bool MapLoader::processMapLine(const std::string &line, const std::shared_ptr<Map> &map)
{
    std::smatch match;
    std::regex_search(line, match, FIELD_PATTERN);

    if (match.size() != 3)
        return false; // invalid line, not enough matches (or too many matches)

    if (match[1].str() == "author")
        map->author = match[2].str();

    if (match[1].str() == "image")
        map->image = match[2].str();

    if (match[1].str() == "scroll")
        map->scroll = getScrollDirectionFromString(match[2].str());

    if (match[1].str() == "wrap")
        map->wrap = getBooleanFromString(match[2].str());

    if (match[1].str() == "warn")
        map->warn = getBooleanFromString(match[2].str());

    return true;
}

bool MapLoader::processContinentsLine(const std::string &line, const std::shared_ptr<Map> &map)
{
    std::shared_ptr<Continent> continent = std::make_shared<Continent>();
    std::smatch match;
    std::regex_search(line, match, FIELD_PATTERN);

    if (match.size() != 3)
        return false; // invalid line, not enough matches (or too many matches)

    continent->name = trim(match[1].str());
    continent->bonus = std::stoi(match[2].str());

    map->continents.emplace(continent->name, continent);

    std::cout << *continent << std::endl;

    return true;
}

bool MapLoader::processTerritoriesLine(const std::string &line, const std::shared_ptr<Map> &map, std::unordered_map<std::string, std::vector<std::string>> *adjacencyMap)
{
    std::shared_ptr<Territory> territory = std::make_shared<Territory>();
    std::vector<std::string> allMatches;
    std::string currentLine = line;
    std::smatch match;

    // gets all matches of the comma-separated pattern
    while (std::regex_search(currentLine, match, COMMA_SEPARATED_PATTERN))
    {
        allMatches.push_back(trim(match[1].str()));
        currentLine = match.suffix().str(); // update the line to search, because regex_search() only searches until the first find
    }

    if (allMatches.size() < 5)
        return false; // invalid line, not enough matches

    territory->name = trim(allMatches[0]);
    territory->x = std::stoi(allMatches[1]);
    territory->y = std::stoi(allMatches[2]);
    const auto continentName = trim(allMatches[3]);

    // add adjacent territories, to the adjacency map (key: territory name, value: vector of adjacent territory names), to be associated with the territory later
    adjacencyMap->emplace(territory->name, std::vector<std::string>(allMatches.begin() + 4, allMatches.end()));

    // associate the territory with its continent
    territory->continent = map->continents[continentName];

    // update related data structures (continent vector, map's unordered map of territories)
    map->continents[continentName]->territories.push_back(territory);
    map->territories.emplace(territory->name, territory);

    return true;
}

bool MapLoader::associateTerritories(const std::shared_ptr<Map> &map, const std::unordered_map<std::string, std::vector<std::string>> &adjacencyMap)
{
    // iterate through the adjacency map, and associate each territory with its adjacent territories
    for (const auto adjacencyPair : adjacencyMap)
    {
        // each pair is made up of the territory name and its adjacent territories names
        for (const auto adjacentTerritoryName : adjacencyPair.second)
        {
            map->territories[adjacencyPair.first]->adjacentTerritories.emplace_back(map->territories[adjacentTerritoryName]);
        }
    }

    return true;
}

/* MAP */

Map::Map()
{
    image = "";
    author = "";
    wrap = false;
    scroll = ScrollDirection::NONE;
    validity = MapValidity::UNKNOWN;
    warn = false;

    continents = std::unordered_map<std::string, std::shared_ptr<Continent>>();
    territories = std::unordered_map<std::string, std::shared_ptr<Territory>>();
}

Map::Map(const Map &map)
{
    author = map.author;
    image = map.image;
    scroll = map.scroll;
    wrap = map.wrap;
    warn = map.warn;
    validity = map.validity;

    // need to implement proper copy constructors for these
    continents = map.continents;
    territories = map.territories;
}

Map &Map::operator=(const Map &map)
{
    this->author = map.author;
    this->image = map.image;
    this->scroll = map.scroll;
    this->wrap = map.wrap;
    this->warn = map.warn;
    this->validity = map.validity;
    this->continents = map.continents;
    this->territories = map.territories;

    return *this;
}

// Map object stream insertion operator
std::ostream &operator<<(std::ostream &os, const Map &map)
{
    os << "Map: { Author: " << map.author << ", Image: " << map.image << ", Scroll: " << (int)map.scroll << ", Wrap: " << map.wrap << ", Warn: " << map.warn << ", Valid: " << (int)map.validity << ", # of Continents: " << map.continents.size() << " }";

    return os;
}

void Map::validate()
{
    validity = MapValidity::VALID;
}

std::string Map::getImage() const { return image; }
std::string Map::getAuthor() const { return author; }
bool Map::getWrap() const { return wrap; }
ScrollDirection Map::getScroll() const { return scroll; }
MapValidity Map::getValidity() const { return validity; }
bool Map::getWarn() const { return warn; }

/* CONTINENT */

Continent::Continent()
{
    name = "";
    bonus = -1;

    territories = std::vector<std::shared_ptr<Territory>>();
}

Continent::Continent(const Continent &continent)
{
    name = continent.name;
    bonus = continent.bonus;

    // need to implement proper copy constructors for these
    territories = continent.territories;
}

Continent &Continent::operator=(const Continent &continent)
{
    this->name = continent.name;
    this->bonus = continent.bonus;
    this->territories = continent.territories;

    return *this;
}

// Continent object stream insertion operator
std::ostream &operator<<(std::ostream &os, const Continent &continent)
{
    os << "Continent: { Name: " << continent.name << ", Bonus: " << continent.bonus << " }";

    return os;
}

std::string Continent::getName() const { return name; }
uint16_t Continent::getBonus() const { return bonus; }

/* TERRITORY */

Territory::Territory()
{
    name = "";
    x = -1;
    y = -1;

    continent = std::make_shared<Continent>();
    adjacentTerritories = std::vector<std::shared_ptr<Territory>>();
}

Territory::Territory(const Territory &territory)
{
    name = territory.name;
    x = territory.x;
    y = territory.y;

    // need to implement proper copy constructors for these
    continent = territory.continent;
    adjacentTerritories = territory.adjacentTerritories;
}

Territory &Territory::operator=(const Territory &territory)
{
    this->name = territory.name;
    this->x = territory.x;
    this->y = territory.y;
    this->continent = territory.continent;
    this->adjacentTerritories = territory.adjacentTerritories;

    return *this;
}

// Territory object stream insertion operator
std::ostream &operator<<(std::ostream &os, const Territory &territory)
{
    os << "Territory: { Name: " << territory.name << ", X: " << territory.x << ", Y: " << territory.y << ", Continent: " << territory.continent->getName() << ", # of Adjacent Territories: " << territory.adjacentTerritories.size() << " }";

    return os;
}

std::string Territory::getName() const { return name; }
uint16_t Territory::getX() const { return x; }
uint16_t Territory::getY() const { return y; }
#include <fstream>

#include "Map.h"

std::ostream &operator<<(std::ostream &os, const MapValidity &validity)
{
    switch (validity)
    {
    case MapValidity::INVALID:
        os << "Invalid";
        break;

    case MapValidity::VALID:
        os << "Valid";
        break;

    default:
        os << "Unknown";
        break;
    }

    return os;
}

std::ostream &operator<<(std::ostream &os, const ScrollDirection &direction)
{
    switch (direction)
    {
    case ScrollDirection::HORIZONTAL:
        os << "Horizontal";
        break;

    case ScrollDirection::VERTICAL:
        os << "Vertical";
        break;

    default:
        os << "None";
        break;
    }

    return os;
}

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

std::string getStringFromBoolean(const bool &boolean)
{
    return boolean ? "True" : "False";
}

std::shared_ptr<Map> MapLoader::loadMap(const std::string &path)
{
    std::string line;
    std::ifstream mapFile(path);
    int8_t mapSection = -1;

    std::shared_ptr<Map> map = std::make_shared<Map>();

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
            if (!processTerritoriesLine(line, map))
                return map;
            break;
        }
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
        *map->author = match[2].str();

    if (match[1].str() == "image")
        *map->image = match[2].str();

    if (match[1].str() == "scroll")
        *map->scroll = getScrollDirectionFromString(match[2].str());

    if (match[1].str() == "wrap")
        *map->wrap = getBooleanFromString(match[2].str());

    if (match[1].str() == "warn")
        *map->warn = getBooleanFromString(match[2].str());

    return true;
}

bool MapLoader::processContinentsLine(const std::string &line, const std::shared_ptr<Map> &map)
{
    std::shared_ptr<Continent> continent = std::make_shared<Continent>();
    std::smatch match;
    std::regex_search(line, match, FIELD_PATTERN);

    if (match.size() != 3)
        return false; // invalid line, not enough matches (or too many matches)

    *continent->name = trim(match[1].str());
    *continent->bonus = std::stoi(match[2].str());

    map->continents.emplace(*continent->name, continent);

    return true;
}

bool MapLoader::processTerritoriesLine(const std::string &line, const std::shared_ptr<Map> &map)
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

    *territory->name = trim(allMatches[0]);
    *territory->x = std::stoi(allMatches[1]);
    *territory->y = std::stoi(allMatches[2]);
    const auto continentName = trim(allMatches[3]);

    // add adjacent territories, to the adjacency map (key: territory name, value: vector of adjacent territory names), to be associated with the territory later
    map->adjacency.emplace(*territory->name, std::vector<std::string>(allMatches.begin() + 4, allMatches.end()));

    // associate the territory with its continent
    territory->continent = map->continents[continentName];
    (*territory->continent->territoryCount)++;

    // update related data structures (map's unordered map of territories)
    map->territories.emplace(*territory->name, territory);

    return true;
}

/* MAP */

Map::Map()
{
    image = new std::string("");
    author = new std::string("");
    wrap = new bool(false);
    scroll = new ScrollDirection(ScrollDirection::NONE);
    validity = new MapValidity(MapValidity::UNKNOWN);
    warn = new bool(false);

    continents = std::unordered_map<std::string, std::shared_ptr<Continent>>();
    territories = std::unordered_map<std::string, std::shared_ptr<Territory>>();
}

Map::Map(const Map &map) : continents(map.continents), territories(map.territories)
{
    author = map.author;
    image = map.image;
    scroll = map.scroll;
    wrap = map.wrap;
    validity = map.validity;
    warn = map.warn;
}

Map::~Map()
{
    delete image, author, wrap, scroll, validity, warn;
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
    os << "Map: { Author: " << *map.author << ", Image: " << *map.image << ", Scroll: " << *map.scroll << ", Wrap: " << getStringFromBoolean(*map.wrap) << ", Warn: " << getStringFromBoolean(*map.warn) << ", Validity: " << *map.validity << ", # of Continents: " << map.continents.size() << ", # of Territories: " << map.territories.size() << " }";

    return os;
}

SharedContinentsVector Map::getAllContinents(const Map &map)
{
    SharedContinentsVector continents{};

    for (auto &&pair : map.continents)
    {
        continents.emplace_back(pair.second);
    }

    return continents;
}

SharedTerritoriesVector Map::getAllTerritories(const Map &map)
{
    SharedTerritoriesVector territories{};

    for (auto &&pair : map.territories)
    {
        territories.emplace_back(pair.second);
    }

    return territories;
}

// This function is included for convenience, works identically to its overloaded version.
SharedTerritoriesVector Map::getAllTerritoriesInContinent(const Map &map, const Continent &continent)
{
    return Map::getAllTerritoriesInContinent(map, continent.getName());
}

SharedTerritoriesVector Map::getAllTerritoriesInContinent(const Map &map, const std::string &continent)
{
    SharedTerritoriesVector territoriesInContinent{};

    for (auto &&pair : map.territories)
    {
        if (pair.second->getContinent()->getName() == continent)
            territoriesInContinent.emplace_back(pair.second);
    }

    return territoriesInContinent;
}

// This function is included for convenience, works identically to its overloaded version.
SharedTerritoriesVector Map::getAdjacentTerritories(const Map &map, const Territory &territory)
{
    return Map::getAdjacentTerritories(map, territory.getName());
}

SharedTerritoriesVector Map::getAdjacentTerritories(const Map &map, const std::string &territory)
{
    SharedTerritoriesVector adjacentTerritories{};

    for (auto &&territoryName : map.adjacency.at(territory))
    {
        adjacentTerritories.push_back(map.territories.at(territoryName));
    }

    return adjacentTerritories;
}

bool Map::areAdjacent(const Map &map, const Territory &territory1, const Territory &territory2)
{
    return Map::areAdjacent(map, territory1.getName(), territory2.getName());
}

bool Map::areAdjacent(const Map &map, const std::string &territory1, const std::string &territory2)
{
    return std::find(map.adjacency.at(territory1).begin(), map.adjacency.at(territory1).end(), territory2) != map.adjacency.at(territory1).end();
}

void Map::countTraversedTerritories(const Map &map, const std::string &territory, std::unordered_set<std::string> &visited)
{
    visited.insert(territory);

    for (auto &&territoryName : map.adjacency.at(territory))
    {
        if (visited.find(territoryName) == visited.end())
            Map::countTraversedTerritories(map, territoryName, visited);
    }
}

void Map::countTraversedTerritoriesInContinent(const Map &map, const std::string &continent, const std::string &territory, std::unordered_set<std::string> &visited)
{
    visited.insert(territory);

    for (auto &&territoryName : map.adjacency.at(territory))
    {
        // if we haven't visited this territory before and is part of the continent we're looking at, continue the traversal
        if (visited.find(territoryName) == visited.end() && map.territories.at(territoryName)->getContinent()->getName() == continent)
            Map::countTraversedTerritoriesInContinent(map, continent, territoryName, visited);
    }
}

void Map::validate(Map &map)
{
    std::unordered_set<std::string> visitedTerritories;

    *map.validity = MapValidity::UNKNOWN;

    // 1: Map should be a connected graph

    Map::countTraversedTerritories(map, map.territories.begin()->first, visitedTerritories);
    if (visitedTerritories.size() != map.territories.size())
    {
        *map.validity = MapValidity::INVALID;
        return;
    }

    for (auto &&pair : map.continents)
    {
        visitedTerritories.clear();

        // 3: Each country belongs to one and only one continent

        const auto territoriesInContinent = Map::getAllTerritoriesInContinent(map, pair.first);

        // if the # of known territories in a continent is not the same as the # of loaded territories, the map is invalid
        if (territoriesInContinent.size() != pair.second->getTerritoryCount())
        {
            *map.validity = MapValidity::INVALID;
            return;
        }

        // 2: Continents are connected subgraphs

        Map::countTraversedTerritoriesInContinent(map, pair.first, territoriesInContinent.front()->getName(), visitedTerritories);

        // if the # of territories we traversed is not the same as the # of known territories in a continent, the map is invalid
        if (visitedTerritories.size() != territoriesInContinent.size())
        {
            *map.validity = MapValidity::INVALID;
            return;
        }
    }

    *map.validity = MapValidity::VALID;
}

std::string Map::getImage() const { return *image; }
std::string Map::getAuthor() const { return *author; }
bool Map::getWrap() const { return *wrap; }
ScrollDirection Map::getScroll() const { return *scroll; }
MapValidity Map::getValidity() const { return *validity; }
bool Map::getWarn() const { return warn; }

/* CONTINENT */

Continent::Continent()
{
    name = new std::string("");
    bonus = new uint16_t(0);
    territoryCount = new size_t(0);
}

Continent::Continent(const Continent &continent)
{
    name = continent.name;
    bonus = continent.bonus;
    territoryCount = continent.territoryCount;
}

Continent::~Continent()
{
    delete name, bonus, territoryCount;
}

Continent &Continent::operator=(const Continent &continent)
{
    this->name = continent.name;
    this->bonus = continent.bonus;
    this->territoryCount = continent.territoryCount;

    return *this;
}

// Continent object stream insertion operator
std::ostream &operator<<(std::ostream &os, const Continent &continent)
{
    os << "Continent: { Name: " << *continent.name << ", Bonus: " << *continent.bonus << " }";

    return os;
}

std::string Continent::getName() const { return *name; }
uint16_t Continent::getBonus() const { return *bonus; }
size_t Continent::getTerritoryCount() const { return *territoryCount; }

/* TERRITORY */

Territory::Territory()
{
    name = new std::string("");
    x = new uint16_t(0);
    y = new uint16_t(0);

    continent = std::make_shared<Continent>();
}

Territory::Territory(const Territory &territory) : continent(territory.continent)
{
    name = territory.name;
    x = territory.x;
    y = territory.y;
}

Territory::~Territory()
{
    delete name, x, y;
}

Territory &Territory::operator=(const Territory &territory)
{
    this->name = territory.name;
    this->x = territory.x;
    this->y = territory.y;
    this->continent = territory.continent;

    return *this;
}

// Territory object stream insertion operator
std::ostream &operator<<(std::ostream &os, const Territory &territory)
{
    os << "Territory: { Name: " << *territory.name << ", X: " << *territory.x << ", Y: " << *territory.y << ", Continent: " << territory.continent->getName() << " }";

    return os;
}

std::string Territory::getName() const { return *name; }
uint16_t Territory::getX() const { return *x; }
uint16_t Territory::getY() const { return *y; }
const std::shared_ptr<Continent> &Territory::getContinent() const { return continent; }

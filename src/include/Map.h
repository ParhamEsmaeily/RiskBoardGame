#pragma once

#define MAX_ADJACENT_TERRITORIES 10
#define MAX_CONTINENTS 32
#define MAX_TERRITORIES 255

#include <memory>
#include <vector>
#include <array>
#include <string>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

void testLoadMaps();

// forward declarations
class Player;

class Map;
class Continent;
class Territory;

// complex type's declarations
typedef std::unordered_map<std::string, std::vector<std::string>> AdjacencyMap;
typedef std::vector<std::shared_ptr<Territory>> SharedTerritoriesVector;
typedef std::vector<std::shared_ptr<Continent>> SharedContinentsVector;

enum class MapSection
{
    MAP,
    CONTINENTS,
    TERRITORIES
};

enum class MapValidity
{
    UNKNOWN,
    INVALID,
    VALID
};

std::ostream &operator<<(std::ostream &os, const MapValidity &validity);

enum class ScrollDirection
{
    NONE,
    HORIZONTAL,
    VERTICAL
};

std::ostream &operator<<(std::ostream &os, const ScrollDirection &direction);

// remove leading & trailing whitespace (https://stackoverflow.com/a/1798170)
static std::string trim(const std::string &str, const std::string &whitespace = " \t\r\n");

static ScrollDirection getScrollDirectionFromString(const std::string &scrollDirectionString);
static bool getBooleanFromString(const std::string &booleanString);
static std::string getStringFromBoolean(const bool &boolean);

class Continent
{
    friend class MapLoader;

private:
    uint16_t *bonus;
    std::string *name;
    size_t *territoryCount;

public:
    Continent();
    Continent(const Continent &continent);
    ~Continent();

    Continent &operator=(const Continent &continent);
    friend std::ostream &operator<<(std::ostream &os, const Continent &continent);

    std::string getName() const;
    uint16_t getBonus() const;
    size_t getTerritoryCount() const;
};

class Territory
{
    friend class MapLoader;

private:
    std::shared_ptr<Continent> continent;

    uint16_t *x, *y;
    std::string *name;
    Player *owner;

public:
    Territory();
    Territory(const Territory &territory);
    ~Territory();

    Territory &operator=(const Territory &territory);
    friend std::ostream &operator<<(std::ostream &os, const Territory &territory);

    std::string getName() const;
    uint16_t getX() const;
    uint16_t getY() const;
    const std::shared_ptr<Continent> &getContinent() const;

    Player *getOwner() const;
    void setOwner(Player *owner);
};

class Map
{
    friend class MapLoader;

private:
    AdjacencyMap adjacency;
    std::unordered_map<std::string, std::shared_ptr<Territory>> territories;
    std::unordered_map<std::string, std::shared_ptr<Continent>> continents;

    std::string *image;
    std::string *author;
    bool *wrap;
    ScrollDirection *scroll;
    MapValidity *validity;
    bool *warn;

public:
    Map();
    Map(const Map &map);
    ~Map();

    Map &operator=(const Map &map);
    friend std::ostream &operator<<(std::ostream &os, const Map &map);

    static SharedContinentsVector getAllContinents(const Map &map);
    static SharedTerritoriesVector getAllTerritories(const Map &map);
    static SharedTerritoriesVector getAllTerritoriesInContinent(const Map &map, const Continent &continent);
    static SharedTerritoriesVector getAllTerritoriesInContinent(const Map &map, const std::string &continent);

    /// @brief Checks if the list of passed territories are all part of the same continent and if the continent has no other territories
    /// @return true if all territories are in the same continent and the continent has no other territories
    static bool areAllTerritoriesInContinentExclusive(const Map &map, const std::string &continent, const std::vector<std::string> &territories);

    /// @brief Checks if the list of passed territories are all part of the same continent
    /// @return true if all territories are in the same continent
    static bool areAllTerritoriesInContinent(const Map &map, const std::string &continent, const std::vector<std::string> &territories);

    static SharedTerritoriesVector getAdjacentTerritories(const Map &map, const Territory &territory);
    static SharedTerritoriesVector getAdjacentTerritories(const Map &map, const std::string &territory);
    static bool areAdjacent(const Map &map, const Territory &territory1, const Territory &territory2);
    static bool areAdjacent(const Map &map, const std::string &territory1, const std::string &territory2);

    static void countTraversedTerritories(const Map &map, const std::string &territory, std::unordered_set<std::string> *visited);
    static void countTraversedTerritoriesInContinent(const Map &map, const std::string &continent, const std::string &territory, std::unordered_set<std::string> *visited);

    static void validate(Map *map);

    std::string getImage() const;
    std::string getAuthor() const;
    bool getWrap() const;
    ScrollDirection getScroll() const;
    MapValidity getValidity() const;
    bool getWarn() const;
};

class MapLoader
{
private:
    inline static const std::regex HEADER_PATTERN = std::regex("\\[(.+)\\]", std::regex_constants::icase);        // captures the section name (e.g. "[Map]", without the brackets)
    inline static const std::regex FIELD_PATTERN = std::regex("(^.+)=(.+)", std::regex_constants::icase);         // captures the field name (e.g. "name=", without the equals sign)
    inline static const std::regex COMMA_SEPARATED_PATTERN = std::regex("([^,]+)+", std::regex_constants::icase); // basically splits the string by commas

public:
    static std::shared_ptr<Map> loadMap(const std::string &path);

private:
    static uint8_t getMapSection(const std::string &line);

    static bool processMapLine(const std::string &line, const std::shared_ptr<Map> &map);
    static bool processContinentsLine(const std::string &line, const std::shared_ptr<Map> &map);
    static bool processTerritoriesLine(const std::string &line, const std::shared_ptr<Map> &map);
};
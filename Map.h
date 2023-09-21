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

// forward declarations
class Map;
class Continent;
class Territory;

// complex type's declarations
typedef std::unordered_map<std::string, std::vector<std::string>> AdjacencyList;
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

enum class ScrollDirection
{
    NONE,
    HORIZONTAL,
    VERTICAL
};

// remove leading & trailing whitespace (https://stackoverflow.com/a/1798170)
static std::string trim(const std::string &str, const std::string &whitespace = " \t\r\n");

static ScrollDirection getScrollDirectionFromString(const std::string &scrollDirectionString);
static bool getBooleanFromString(const std::string &booleanString);

class Continent
{
    friend class MapLoader;

private:
    uint16_t bonus;
    std::string name;

public:
    Continent();
    Continent(const Continent &continent);

    Continent &operator=(const Continent &continent);
    friend std::ostream &operator<<(std::ostream &os, const Continent &continent);

    std::string getName() const;
    uint16_t getBonus() const;
};

class Territory
{
    friend class MapLoader;

private:
    std::shared_ptr<Continent> continent;

    uint16_t x, y;
    std::string name;

public:
    Territory();
    Territory(const Territory &territory);

    Territory &operator=(const Territory &territory);
    friend std::ostream &operator<<(std::ostream &os, const Territory &territory);

    std::string getName() const;
    uint16_t getX() const;
    uint16_t getY() const;
    const std::shared_ptr<Continent> &getContinent() const;
};

class Map
{
    friend class MapLoader;

private:
    AdjacencyList adjacency;
    std::unordered_map<std::string, std::shared_ptr<Territory>> territories;
    std::unordered_map<std::string, std::shared_ptr<Continent>> continents;

    std::string image;
    std::string author;
    bool wrap;
    ScrollDirection scroll;
    MapValidity validity;
    bool warn;

public:
    Map();
    Map(const Map &map);

    Map &operator=(const Map &map);
    friend std::ostream &operator<<(std::ostream &os, const Map &map);

    static SharedContinentsVector getAllContinents(const Map &map);
    static SharedTerritoriesVector getAllTerritories(const Map &map);
    static SharedTerritoriesVector getAllTerritoriesInContinent(const Map &map, const Continent &continent);
    static SharedTerritoriesVector getAllTerritoriesInContinent(const Map &map, const std::string &continent);
    static SharedTerritoriesVector getAdjacentTerritories(const Map &map, const Territory &territory);
    static SharedTerritoriesVector getAdjacentTerritories(const Map &map, const std::string &territory);

    void validate();

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
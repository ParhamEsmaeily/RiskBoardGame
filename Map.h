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

class Map;
class Continent;
class Territory;

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

class Territory
{
    friend class MapLoader;

private:
    std::shared_ptr<Continent> continent;
    std::vector<std::shared_ptr<Territory>> adjacentTerritories;

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
};

class Continent
{
    friend class MapLoader;

private:
    std::vector<std::shared_ptr<Territory>> territories;

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

class Map
{
    friend class MapLoader;

private:
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
    static std::shared_ptr<Map>
    loadMap(const std::string &path);

private:
    static uint8_t getMapSection(const std::string &line);

    static bool processMapLine(const std::string &line, const std::shared_ptr<Map> &map);
    static bool processContinentsLine(const std::string &line, const std::shared_ptr<Map> &map);
    static bool processTerritoriesLine(const std::string &line, const std::shared_ptr<Map> &map, std::unordered_map<std::string, std::vector<std::string>> *adjacencyMap);

    static bool associateTerritories(const std::shared_ptr<Map> &map, const std::unordered_map<std::string, std::vector<std::string>> &adjacencyMap);
};
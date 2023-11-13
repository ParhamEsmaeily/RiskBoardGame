#include <set>
#include <map>
#include "Cards.h"
#include "Orders.h"
#include "Player.h"
#include "../Map/Map.h"

using namespace std;

Player::Player() : playerId(0), name("player"), order_list(new OrdersList()) {}

Player::Player(int playerID, string name)
    : playerId(playerID), name(name), order_list(new OrdersList()),
      hand(new Hand())
{
    // Deck deck;

    // Random index generator.
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 4);
    for (int i = 0; i < 15; i++)
    {
        int index = dist6(rng);
        // int index2 = dist6(rng);

        // Inserts random cards inside both deck and the hand.
        hand->insert(CardType(index));
        // deck.insert(Type(index2));
    }
}

// constructor for player with all parameters, name id territories hand and
// orderlists
Player::Player(int playerID, string name, vector<Territory *> &territories,
               Hand *hand, OrdersList *orders)
    : playerId(playerID), name(name), territories(territories), hand(hand),
      order_list(orders) {}

Player::Player(bool isNeutral) : Player()
{
    this->is_neutral = true;
}

// create copy player obj
Player::Player(const Player &p)
    : playerId(p.playerId), name(p.name), hand(new Hand(*(p.hand))),
      order_list(new OrdersList(*(p.order_list)))
{
    for (Territory *t : p.territories)
    {
        this->addTerritory(new Territory(*t));
    }
}

// destructor
Player::~Player()
{
    /*     for (Territory *t : territories)
        {
            delete t;
        } */
    delete order_list;
    delete hand;

    territories.clear();
}

// assign one Player object to another.
Player &Player::operator=(const Player &p)
{
    playerId = p.playerId;
    name = p.name;
    for (Territory *t : p.territories)
    {
        this->addTerritory(new Territory(*t));
    }
    this->hand = new Hand(*(p.hand));
    this->order_list = new OrdersList(*(p.order_list));
    return *this;
}

// check equality between player objects
bool Player::operator==(const Player &other)
{
    if (this->playerId == other.playerId)
        return true;
    return false;
}

bool Player::operator!=(const Player &other) { return !(*this == other); }

vector<Territory *> Player::toDefend() { return this->territories; }

vector<Territory *> Player::toAttack(const Map &gameMap)
{
    std::set<Territory *> all_adjacent_territories;
    std::set<Territory *> owned_territories(this->territories.begin(), this->territories.end());

    // get all adjacent territories (including owned territories)
    for (Territory *t : this->territories)
    {
        for (const std::shared_ptr<Territory> &adjacent_territory : Map::getAdjacentTerritories(gameMap, *t))
        {
            all_adjacent_territories.insert(adjacent_territory.get());
        }
    }

    // do the set difference between owned territories and adjacent
    // territories to get only attack-able territories
    vector<Territory *> attack_territories;
    std::set_difference(
        all_adjacent_territories.begin(),
        all_adjacent_territories.end(),
        owned_territories.begin(),
        owned_territories.end(),
        std::inserter(attack_territories, attack_territories.begin()));

    return attack_territories;
}

void Player::issueOrder(const Map &gameMap)
{
    // print all territories to defend
    cout << getName() << "'s territories to defend:" << endl;
    for (Territory *t : this->toDefend())
    {
        cout << *t << endl;
    }

    // Create a map of card types and their count of the player's hand
    map<CardType, int> cards_count;
    int nb_reinforcement_cards = 0;
    for (CardType card : this->getHand()->show_cards())
    {
        if (card != CardType::reinforcement)
        {
            cards_count[card]++;
        }
        else
        {
            nb_reinforcement_cards++;
        }
    }

    // while the user doesn't type 'end turn', keep asking for orders
    string input;
    while (input != "end turn")
    {
        /*
        / Issue Deploy() orders until the player has no more reinforcement cards
        */
        while (nb_reinforcement_cards > 0)
        {
            string terr_name;
            string str_reinforcements;
            bool orderAdded = false;

            cout << getName() << "'s turn, choose a territory to deploy reinforcements by writing the name: ";
            getline(cin, terr_name);
            cout << endl;
            cout << "Choose the number of reinforcements to send (" << nb_reinforcement_cards << " available): ";
            getline(cin, str_reinforcements);
            cout << endl;

            // if num_reinforcements is not a number, ask again
            if (!std::all_of(str_reinforcements.begin(), str_reinforcements.end(), ::isdigit))
            {
                cout << "Invalid number of reinforcements." << endl;
                continue;
            }

            // add to orders list the order to deploy the number of reinforcements to the territory
            for (Territory *t : this->toDefend())
            {
                if (t->getName() == terr_name)
                {
                    // TODO: add the deploy order to the order list
                    this->order_list->add(Deploy(this, &gameMap, t, 1));
                    orderAdded = true;
                    break;
                }
            }

            int num_reinforcements = std::stoi(str_reinforcements);
            if (orderAdded && num_reinforcements <= nb_reinforcement_cards && num_reinforcements > 0)
            {
                nb_reinforcement_cards -= num_reinforcements;
            }
            else
            {
                cout << "Invalid order." << endl;
            }
        }

        /*
        / Other orders
        */
        while (input != "end turn")
        {

            // print all cards in hand
            cout << getName() << "'s cards in hand:" << endl;
            cout << "advance: infinite" << endl;
            for (auto const &[card, count] : cards_count)
            {
                cout << cd::map(card) << ": " << count << " available" << endl;
            }

            // ask for an order
            cout << "Enter an order (or 'end turn'):" << endl;
            getline(cin, input);

            // create an order based on the input
            if (input == "end turn")
            {
                break;
            }
            else if (input == "advance")
            {
                // print all territories to attack (for visibility)
                cout << getName() << "'s territories to attack:" << endl;
                for (Territory *t : this->toAttack(gameMap))
                {
                    cout << *t << endl;
                }

                string terr_1;
                string terr_2;
                string str_num_armies;

                cout << "Choose a territory to attack from: ";
                getline(cin, terr_1);
                cout << endl;
                cout << "Choose a territory to attack: ";
                getline(cin, terr_2);
                cout << endl;
                cout << "Choose the number of armies to send: ";
                getline(cin, str_num_armies);
                cout << endl;

                // if num_armies is not a number, ask again
                if (!std::all_of(str_num_armies.begin(), str_num_armies.end(), ::isdigit))
                {
                    cout << "Invalid number of armies." << endl;
                    continue;
                }

                // Parse strings to territory objects
                Territory *source = nullptr;
                Territory *dest = nullptr;

                for (Territory *t : this->toAttack(gameMap))
                {
                    if (t->getName() == terr_1)
                    {
                        source = t;
                    }
                    else if (t->getName() == terr_2)
                    {
                        dest = t;
                    }
                }

                // TODO: add params to Advance constructor
                this->order_list->add(Advance(this, &gameMap, this, source, dest, std::stoi(str_num_armies)));
            }
            else if (input == "bomb" && cards_count[CardType::bomb] > 0)
            {
                this->order_list->add(Bomb(this, &gameMap, ));
                cards_count[CardType::bomb]--;
            }
            else if (input == "blockade" && cards_count[CardType::blockade] > 0)
            {
                this->order_list->add(Blockade());
                cards_count[CardType::blockade]--;
            }
            else if (input == "airlift" && cards_count[CardType::airlift] > 0)
            {
                this->order_list->add(Airlift());
                cards_count[CardType::airlift]--;
            }
            else if (input == "negotiate" && cards_count[CardType::diplomacy] > 0)
            {
                this->order_list->add(Negotiate());
                cards_count[CardType::diplomacy]--;
            }
            else
            {
                cout << "Invalid order." << endl;
            }
        }
    }
}

// issueOrder: creates an order object and adds it to the list of orders
//  void Player::issueOrder(){
//      order_list->add(new Deploy);
//  }

void Player::addTerritory(const Territory *t)
{
    territories.push_back(const_cast<Territory *>(t));
    units_map[t->getName()] = 0;
}

void Player::removeTerritory(const Territory *t)
{
    for (int i = 0; i < territories.size(); i++)
    {
        if (territories[i]->getName() == t->getName())
        {
            territories.erase(territories.begin() + i);
            break;
        }
    }
    units_map.erase(t->getName());
}

void Player::addAlly(const Player *p)
{
    this->allies.push_back(const_cast<Player *>(p));
}

void Player::resetTurnValues()
{
    this->conquered_this_turn = false;
    this->allies.clear();
}

bool Player::isAllied(Player *p)
{
    for (int i = 0; i < this->allies.size(); i++)
    {
        if (*p == *(allies[i]))
            return true;
    }
    return false;
}

bool Player::owns(const Territory *t) const
{
    for (int i = 0; i < territories.size(); i++)
    {
        if (territories[i]->getName() == t->getName())
            return true;
    }
    return false;
}

// getters
int Player::getPlayerId() { return playerId; }

string Player::getName() { return name; }

Hand *Player::getHand() { return &*hand; }
OrdersList *Player ::getPlayerOrderList() { return order_list; }

vector<Territory *> Player::getTerritories() { return territories; }

int Player::getTerritoryUnits(const Territory *t) const
{
    std::string str = t->getName();
    return units_map.at(str);
}

bool Player::conqueredThisTurn() { return conquered_this_turn; }

bool Player::isNeutral() { return this->is_neutral; }

// setters
void Player::setPlayerOrderList(OrdersList *orders)
{
    this->order_list = orders;
}

void Player::setTerritories(vector<Territory *> t) { this->territories = t; }

ostream &operator<<(ostream &os, Player &p)
{
    return os << "Name: " << p.getName() << " ID: " << p.getPlayerId();
}

void Player::setTerritoryUnits(const Territory *t, int units)
{
    units_map[t->getName()] = units;
}

void Player::setConqueredThisTurn(bool b)
{
    this->conquered_this_turn = b;
}
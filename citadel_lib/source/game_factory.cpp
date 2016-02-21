
#include "citadel/game.hpp"
#include "citadel/character.hpp"

struct build_content
{
	int n;
	std::string name;
	unsigned int cost;
	enum Citadel::Building::Type type;
};

static build_content game[] = {
	{ 5, "Tavern", 1, Citadel::Building::BUILD_TRADE },
	{ 4, "Apothecary", 2, Citadel::Building::BUILD_TRADE },
	{ 4, "Market", 2, Citadel::Building::BUILD_TRADE },
	{ 3, "Tradepost", 3, Citadel::Building::BUILD_TRADE },
	{ 3, "Docks", 4, Citadel::Building::BUILD_TRADE },
	{ 2, "City Hall", 5, Citadel::Building::BUILD_TRADE },

	{ 3, "Watch tower", 1, Citadel::Building::BUILD_MILITARY },
	{ 3, "Prison", 2, Citadel::Building::BUILD_MILITARY },
	{ 3, "Casern", 3, Citadel::Building::BUILD_MILITARY },
	{ 3, "Fortress", 5, Citadel::Building::BUILD_MILITARY },

	{ 3, "Temple", 1, Citadel::Building::BUILD_RELIGIOUS },
	{ 3, "Church", 2, Citadel::Building::BUILD_RELIGIOUS },
	{ 4, "Monastary", 3, Citadel::Building::BUILD_RELIGIOUS },
	{ 2, "Cathedral", 5, Citadel::Building::BUILD_RELIGIOUS },

	{ 5, "Manor", 3, Citadel::Building::BUILD_NOBLE },
	{ 5, "Castle", 4, Citadel::Building::BUILD_NOBLE },
	{ 2, "Palace", 5, Citadel::Building::BUILD_NOBLE },
};

void Citadel::GameFactory::populateDeck(Citadel::Deck &d)
{
	int i, c;

	for (i = 0; i < (sizeof(game) / sizeof(build_content)); i++)
	{
		for (c = 0; c < game[i].n; c++)
		{
			d.pushBack(
				new Building(game[i].name, game[i].cost, game[i].type));
		}
	}
}

struct character_content
{
	std::string name;
//	unsigned int level; // the order in the array gives the level
	unsigned int power;
	enum Citadel::Building::Type taxing;
	std::string description;
};

static character_content characters[] = {
	{ "Assassin", (Citadel::Character::POWER_KILL),
        Citadel::Building::BUILD_N,
        "Kill a character." },
	{ "Thief", (Citadel::Character::POWER_STEAL),
        Citadel::Building::BUILD_N,
	    "Steal from a character - not the Assassin nor the killed one." },
	{ "Magician", (Citadel::Character::POWER_CARDS_PLAYER
        | Citadel::Character::POWER_CARDS_DECK),
        Citadel::Building::BUILD_N,
	    "Replace all cards with another player OR replace some cards "\
        "with the deck." },
	{ "King", (Citadel::Character::POWER_CROWN),
        Citadel::Building::BUILD_NOBLE,
	    "Take the crown; Noble buildings can be taxed." },
	{ "Bishop", (Citadel::Character::POWER_IMMUNITY),
        Citadel::Building::BUILD_RELIGIOUS,
	    "Cannot be attacked by Condotierre; Religious buildings can be "\
        "taxed" },
	{ "Merchant", (Citadel::Character::POWER_RENT),
        Citadel::Building::BUILD_TRADE,
	    "Makes an additional gold per turn; Trade buildings can be taxed" },
	{ "Architect", (Citadel::Character::POWER_CARDS
        | Citadel::Character::POWER_BUILD),
        Citadel::Building::BUILD_N,
	    "Gets additional cards at beginning of turn; can build several "\
        "building per turn." },
	{ "Condotierre", (Citadel::Character::POWER_DESTROY),
        Citadel::Building::BUILD_MILITARY,
	    "Can muster an army to destroy a building; Military buildings "\
        "can be taxed" },
};

void Citadel::GameFactory::populateCharacters(
	std::vector<Citadel::Character *> &char_list)
{
	int i;

	for (i = 0; i < (sizeof(characters) / sizeof(character_content)); i++)
	{
		char_list.push_back(
			new Citadel::Character(characters[i].name, i,
			characters[i].power, characters[i].taxing, 
			characters[i].description) );
	}
}

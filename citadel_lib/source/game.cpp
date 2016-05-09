
#include "citadel/game.hpp"

#include <vector>
#include <list>

#include "citadel/character.hpp"

// official minimum is 4 but 2-3 is possible
#define MIN_PLAYERS 2
// could go up to 8 with queen/architect
#define MAX_PLAYERS 7

#define WIN_FIRST 4
#define WIN_NUMBER 2
#define WIN_COLOURS 3

#define GOAL_BUILDING 8

Citadel::Game::Game()
{
    goal_building = GOAL_BUILDING;
    win_first = WIN_FIRST;
    win_number = WIN_NUMBER;
    win_colours = WIN_COLOURS;

    min_players = MIN_PLAYERS;
    max_players = MAX_PLAYERS;

    crowned = 0;
    choose_hidden_before = 0;
    choose_visible_before = 0;
}

Citadel::Game::~Game()
{
    std::vector<Player *>::iterator p_it = players.begin();

    while (p_it != players.end())
    {
        delete *p_it;
        p_it++;
    }
    players.clear();

    std::vector<Character *>::iterator c_it = characters.begin();
    while (c_it != characters.end())
    {
        delete *c_it;
        c_it++;
    }
    characters.clear();
}

void Citadel::Game::initGame()
{
    crowned = players[rand() % players.size()];

    choose_hidden_before = 1;

    switch (players.size())
    {
    case 2:
        // 2 hidden before, 2 visible before, 2 taken, 2 hidden after
        choose_hidden_before = 2;
    case 3:
    case 4:
        // 1 hidden before, 2 visible before, 3-4 taken, 2-1 hidden after
        choose_visible_before = 2;
        break;
    case 5:
        // 1 hidden before, 1 visible before, 5 taken, 1 hidden after
        choose_visible_before = 1;
        break;

    case 6:
    case 7:
    case 8:
        // 1 hidden before, 0 visible before, 6-7 taken, 1-0 hidden after
        choose_visible_before = 0;
        break;
    }
}

bool Citadel::Game::isFinished() const
{
    std::vector<Player *>::const_iterator it = players.begin();

    while (it != players.end())
    {
        if ((*it)->built().size() >= goal_building)
        {
            return true;
        }
        it++;
    }
    return false;
}

int Citadel::Game::giveCharacters() const
{
    std::vector<Character *>characters_copy = characters;
    int c, pos;
    Character *character = 0;

    if (choose_hidden_before + choose_visible_before + players.size()
         > characters_copy.size())
    {
        return 0;
    }

    // remove cards, hidden
    for (c = 0; c < static_cast<int>(choose_hidden_before); c++)
    {
        pos = rand() % characters_copy.size();
        characters_copy.erase(characters_copy.begin() + pos);
    }

    // remove cards, visible, cannot be the King
    // c has to be int to be allowed to be negative if king is found
    for (c = 0; c < static_cast<int>(choose_visible_before); c++)
    {
        pos = rand() % characters_copy.size();
        character = characters_copy[pos];

        if (!(character->power&Character::POWER_CROWN))
        {
            // cannot be the king
            characters_copy[pos]->visible = true;
            characters_copy.erase(characters_copy.begin() + pos);
        }
        else
        {
            // try again
            c--;
        }
    }

    // give random cards to players starting with the crowned player
    int crown_index = getIndex(crowned);
    for (c = 0; c < static_cast<int>(players.size()); c++)
    {
        int p = (crown_index + c) % players.size();
        pos = rand() % characters_copy.size();

        character = characters_copy[pos];

        players[p]->setCharacter(character);
        players[p]->computePower();

        characters_copy.erase(characters_copy.begin() + pos);
    }

    return c;
}

unsigned int Citadel::Game::computePoints(const Citadel::Player &p)
{
    unsigned typePresent[Citadel::Building::BUILD_N];
    unsigned int points = 0;
    bool hasAllColours = true;

    std::list<Citadel::Building*>::const_iterator it = p.built().begin();

    while (it != p.built().end())
    {
        points += (*it)->value();
        typePresent[static_cast<int>((*it)->type())]++;

        it++;
    }

    if (p.isFirst())
    {
        points += win_first;
    }
    else if (p.built().size() >= goal_building)
    {
        points += win_number;
    }

    hasAllColours = true;  // assumes it has all types
    // todo remove -1 to check wonders when introduced
    for (int i = 0; i < Citadel::Building::BUILD_N - 1; i++)
    {
        if (typePresent[i] == 0)
        {
            hasAllColours = false;
        }
    }
    if (hasAllColours)
    {
        points += win_colours;
    }

    return points;
}

Citadel::Player* Citadel::Game::getPlayer(int level)
{
    std::vector<Player *>::iterator it = players.begin();
    Character *character = 0;

    while (it != players.end())
    {
        character = (*it)->getCharacter();
        if (character && character->level == level)
        {
            if (character->visible || character->dead)
            {
                return 0;
            }
            return *it;
        }
        it++;
    }

    return 0;
}

unsigned int Citadel::Game::getIndex(const Citadel::Player *player) const
{
    unsigned int c;

    for (c = 0; c < players.size(); c++)
    {
        if (players[c] == player)
        {
            return c;
        }
    }
    return c;
}

void Citadel::Game::resetCharacters()
{
    unsigned int i;

    for (i = 0; i < characters.size(); i++)
    {
        characters[i]->reset();
    }
    for (i = 0; i < players.size(); i++)
    {
        players[i]->setCharacter(0);
    }
}

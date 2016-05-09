
#include "citadel/deck.hpp"

#include <cstdlib>
#include <string>
#include <list>

#include "citadel/building.hpp"

//
// Deck
//

Citadel::Deck::~Deck()
{
    std::list<Building*>::iterator it = _cards.begin();
    while (it != _cards.end())
    {
        delete *it;
        it++;
    }
    _cards.clear();
}

void Citadel::Deck::shuffle()
{
    unsigned int size = _cards.size();
    std::list<Building*> new_cards;

    while (size > 0)
    {
        int stop = rand() % size;
        int i = 0;

        std::list<Building*>::iterator it = _cards.begin();

        while (i < stop)
        {
            it++;
            i++;
        }
        new_cards.push_back(*it);
        _cards.remove(*it);
        size--;
    }

    _cards = new_cards;
}

Citadel::Building* Citadel::Deck::getTop()
{
    Building *b = _cards.front();
    if (b)
    {
        _cards.pop_front();
    }
    return b;
}

void Citadel::Deck::pushBack(Building *b)
{
    _cards.push_back(b);
}

bool Citadel::Deck::take(Building *b)
{
    if (!search(b))
    {
        return false;
    }
    _cards.remove(b);
    return true;
}

//
// list access
//

const std::list<Citadel::Building*>& Citadel::Deck::cards(void) const
{
    return _cards;
}

const Citadel::Building* Citadel::Deck::search(const std::string &name) const
{
    std::list<Citadel::Building*>::const_iterator it = _cards.begin();

    while (it != _cards.end())
    {
        if ((*it)->name() == name)
        {
            return *it;
        }
        it++;
    }
    return 0;
}

const Citadel::Building* Citadel::Deck::search(const Citadel::Building *b) const
{
    std::list<Citadel::Building*>::const_iterator it = _cards.begin();

    while (it != _cards.end())
    {
        if ((*it) == b)
        {
            return *it;
        }
        it++;
    }
    return 0;
}

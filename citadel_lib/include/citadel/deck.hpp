/**
 * Copyright (c) 2016 Thomas Richard
 *
 * Following MIT license (see copying.txt)
 *
 * The software is provided "as is", without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement.
 */
#ifndef CITADELLIB_DECK
#define CITADELLIB_DECK

#include <list>
#include <string>

namespace Citadel {

class Building;

class Deck
{
 protected:
    std::list<Building*> _cards;

 public:
    Deck(){};
    virtual ~Deck();

    void shuffle();

    Building *getTop();

    void pushBack(Building *b);
    bool take(Building *b);

    //
    // list access
    //

    const std::list<Building*>& cards(void) const;

    const Building* search(const std::string &name) const;
    const Building* search(const Building *b) const;
};
} /* namespace Citadel */

#endif /* CITADELLIB_DECK */

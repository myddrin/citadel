
#ifndef CITADEL_PLAYER
#define CITADEL_PLAYER

#include <string>
#include <list>

#include "citadel/deck.hpp"

namespace Citadel {

#define DEF_TO_DRAW 2
#define DEF_TO_KEEP 1
#define DEF_TO_GOLD 2
#define DEF_TO_BUILD 1
#define PWR_TO_BUILD 3

#define INIT_GOLD 2
#define INIT_CARDS 4

struct Character;  // declared in citadel/character.hpp

class Player
{
 protected:
    const std::string _name;
    Deck _hand;
    Deck _built;
    int _treasure;

    const unsigned int _initNToDraw;
    const unsigned int _initNToKeep;
    const unsigned int _initNGoldDraw;
    const unsigned int _initNToBuild;
    const unsigned int _powerNToBuild;

    /** @brief number of cards to draw from stack */
    unsigned int _nToDraw;
    /** @brief number of cards to keep when drawing from stack */
    unsigned int _nToKeep;
    /** @brief number of gold to draw */
    unsigned int _nGoldDraw;
    /** @brief number of building that can be built this turn */
    unsigned int _nToBuild;
    /**
        * @brief describes if player is the 1st to have enough buildings to
        * win
        */
    bool _isFirst;

    Character *_character;

 public:
    Player(const std::string &name, int gold = INIT_GOLD,
        unsigned int toDraw = DEF_TO_DRAW,
        unsigned int toKeep = DEF_TO_KEEP,
        unsigned int toGold = DEF_TO_GOLD);

    void computePower();

    //
    // accessors
    //
    int treasure() const;

    /**
        * @param exchange operation to do on treasure (positive adds gold, 
        * negative takes gold)
        * @return new treasure value
        */
    int modifyTreasure(int exchange);

    const std::string& name() const;

    unsigned int nToDraw() const;

    unsigned int nToKeep() const;

    unsigned int nToGold() const;

    unsigned int nToBuild() const;

    bool isFirst() const;

    void setIsFirst(bool f = true);

    //
    // built access
    //
    bool canBuild(const Building *b) const;

    bool build(Building *b, int turn);

    bool takeFromBuilt(Building *b);

    const std::list<Building*>& built(void) const;

    //
    // hand access
    //

    bool takeFromHand(Building *b);
    void giveToHand(Building *b);

    const std::list<Building*>& hand(void) const;

    //
    // character access
    //
    Character *getCharacter();
    void setCharacter(Character *character);
};
} /* namespace Citadel */

#endif /* CITADEL_PLAYER */


#ifndef CITADEL_CHARACTER
#define CITADEL_CHARACTER

#include <string>

#include "citadel/building.hpp"

namespace Citadel {

struct Character
{
    /** @brief possible power for characters */
    enum Power {
        /** @brief Nothing special */
        POWER_NONE = 0,
        /** @brief Kill another character */
        POWER_KILL = 1,
        /**
            * @brief Steal from another character
            *
            * Cannot steal from the assassin (lower level of play)
            * or a dead character.
            */
        POWER_STEAL = POWER_KILL << 1,
        /** @brief exchange all cards with another player */
        POWER_CARDS_PLAYER = POWER_STEAL << 1,
        /** @brief exchange some cards with the deck */
        POWER_CARDS_DECK = POWER_CARDS_PLAYER << 1,
        /** @brief become the first player to choose a character */
        POWER_CROWN = POWER_CARDS_DECK << 1,
        /** @brief cannot be attacked by the condotiere */
        POWER_IMMUNITY = POWER_CROWN << 1,
        /** @brief additional gold each turn */
        POWER_RENT = POWER_IMMUNITY << 1,
        /** @brief additional cards each turn */
        POWER_CARDS = POWER_RENT << 1,
        /** @brief can build more buildings per turn */
        POWER_BUILD = POWER_CARDS << 1,
        /**
            * @brief can destroy another building
            *
            * Unless the character has immunity. Has to pay value-1.
            * Cannot attack the 1st player that has enough buildings to win.
            */
            POWER_DESTROY = POWER_BUILD << 1,
    };

    const std::string name;
    const std::string description;

    /** @brief turn of play - lower first */
    const unsigned int level;
    /**
        * @brief power held - constructed from values @ref enum Power
        */
    const unsigned int power;
    /** @brief what buildings are taxable */
    const enum Building::Type taxing;

    /** @brief if dead will not be able to play or be stolen from */
    bool dead;

    /** @brief at start of turn loose all its money to the thief */
    bool stolen;

    /**
        * @brief character is visible
        *
        * (i.e. was turned over or has played already)
        */
    bool visible;

    Character(const std::string &_name, unsigned int _level,
        unsigned int _power, enum Building::Type _taxing,
        const std::string &_description = "");

    void reset();
};
} /* namespace Citadel */

#endif /* CITADEL_CHARACTER */

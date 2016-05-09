/**
 * Copyright (c) 2016 Thomas Richard
 *
 * Following MIT license (see copying.txt)
 *
 * The software is provided "as is", without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement.
 */
#ifndef CITADEL_GAME
#define CITADEL_GAME

#include <vector>

#include "citadel/deck.hpp"
#include "citadel/building.hpp"
#include "citadel/player.hpp"

namespace Citadel {

struct Character;

struct Game
{
    std::vector<Player *> players;
    std::vector<Character *> characters;

    Deck deck;

    /** @brief number of buildings to win */
    unsigned int goal_building;

    /** @brief number of points if first to have goal_building built */
    unsigned int win_first;
    /**
        * @brief number of points if not first but goal_building have been
        * built
        */
    unsigned int win_number;
    /**
        * @brief number of points if player has all different building types
        */
    unsigned int win_colours;

    /** @brief minimum number of players */
    unsigned int min_players;
    /** @brief maximum number of players */
    unsigned int max_players;

    /**
        * @brief crowned player
        *
        * the player with the crown gets the 1st character
        */
    Player *crowned;

    /**
        * @brief number of hidden characters before the crown player
        * chooses a character
        */
    unsigned int choose_hidden_before;
    /**
        * @brief number of visible characters after every players chose a
        * character
        *
        * @note cannot be the King
        */
    unsigned int choose_visible_before;

// methods
    Game();
    virtual ~Game();

    /**
        * @brief once all players have been added set an initial Player for
        * the crown (random) and computes the choosing cards rules.
        */
    void initGame();

    bool isFinished() const;

    /**
        * @brief start of the turn - give a character to each player
        *
        * Also update's the player's powers
        *
        * @return number of given characters (0 on failure)
        */
    int giveCharacters() const;

    unsigned int computePoints(const Player &p);

    /**
        * @brief get the Player that should play (using it's Character level)
        *
        * @return Player to play
        * @return NULL if character's level is dead, visible or not available
        */
    Player *getPlayer(int level);

    /** @brief get index of player (for crown) */
    unsigned int getIndex(const Player *player) const;

    void resetCharacters();
};

class GameFactory
{
 public:
    static void populateDeck(Deck &d);

    static void populateCharacters(std::vector<Character *> &c);
};
} /* namespace Citadel */

#endif /* CITADEL_GAME */

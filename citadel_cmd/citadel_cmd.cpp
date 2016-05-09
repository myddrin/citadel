
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <list>
#include <vector>

#include "citadel/deck.hpp"
#include "citadel/building.hpp"
#include "citadel/player.hpp"
#include "citadel/character.hpp"
#include "citadel/game.hpp"

std::ostream & operator<<(std::ostream &os, enum Citadel::Building::Type t)
{
    switch (t)
    {
    case Citadel::Building::BUILD_TRADE:
        os << "Trade";
        break;
    case Citadel::Building::BUILD_MILITARY:
        os << "Military";
        break;
    case Citadel::Building::BUILD_RELIGIOUS:
        os << "Regligious";
        break;
    case Citadel::Building::BUILD_NOBLE:
        os << "Noble";
        break;
    case Citadel::Building::BUILD_WONDER:
        os << "Wonder";
        break;
    default:
        os << "Unknown";
    }

    return os;
}

std::ostream & operator<<(std::ostream &os, const Citadel::Building &b)
{
    os << b.name() << " (" << b.type() << ") " << b.value() << "g";
    if (b.description().size() > 0)
    {
        os << std::endl << b.description();
    }
    return os;
}

// different from operator because we want to print number to make a choice
void printDeck(const std::list<Citadel::Building*>& cards,
    bool nb = false, std::ostream &os = std::cout)
{
    std::list<Citadel::Building*>::const_iterator it = cards.begin();
    int i = 0;

    while (it != cards.end())
    {
        if (nb)
        {
            os << i << ": ";
        }
        os << *(*it);
        os << std::endl;
        it++;
        i++;
    }
}

std::ostream & operator<<(std::ostream &os, const Citadel::Deck &deck)
{
    printDeck(deck.cards(), false, os);
    return os;
}

std::ostream & operator<<(std::ostream &os, const Citadel::Player &p)
{
    os << p.name() << " " << p.treasure() << "g "
        << p.built().size() << " building and " << p.hand().size()
        << " in hand." << std::endl;
    if (p.built().size() > 0)
    {
        os << "City:" << std::endl;
        printDeck(p.built(), false, os);
    }

    return os;
}

void printHand(const Citadel::Player &p, bool nb = false,
    std::ostream &os = std::cout)
{
    std::list<Citadel::Building*>::const_iterator it = p.hand().begin();
    int i = 0;
    os << p.name() << "'s hand (" << p.treasure() << " gold):" << std::endl;
    while (it != p.hand().end())
    {
        bool can = p.canBuild(*it);

        // if nb we print numbers that will be used for choices
        if (nb)
        {
            os << i << ": ";
        }
        os << *(*it);
        if (!can)
        {
            os << " [cannot build]";
        }
        os << std::endl;

        it++;
        i++;
    }
}

#define MIN_PLAYERS 2
#define MAX_PLAYERS 8
struct Game: public Citadel::Game
{
    int createGame(unsigned int nPlayers = 4);

    int play();

    int findWinner();

    // returns the amount of gold to collect
    int collectTaxes(const Citadel::Player *current,
        const enum Citadel::Building::Type &taxing);

    int power_kill(Citadel::Player *current);
    int power_steal(Citadel::Player *current);
    int power_cards(Citadel::Player *current);
    int power_destroy(Citadel::Player *current);
};

int Game::createGame(unsigned int nPlayers)
{
    unsigned int i;
    std::vector<Citadel::Player *>::const_iterator p_it;

    if (nPlayers > MAX_PLAYERS)
    {
        std::cerr << "Too many players! Maximum is " << MAX_PLAYERS
            << std::endl;
        return EXIT_FAILURE;
    }
    if (nPlayers < MIN_PLAYERS)
    {
        std::cerr << "Not enough players! Minimum is " << MIN_PLAYERS
            << std::endl;
        return EXIT_FAILURE;
    }

    //
    // initialise the game
    //

    Citadel::GameFactory::populateDeck(deck);
    Citadel::GameFactory::populateCharacters(characters);

    std::cout << "shuffle cards..." << std::endl;
    deck.shuffle();

    std::ostringstream name;
    for (i = 0; i < nPlayers; i++)
    {
        name.str("");
        name << "player" << i;
        players.push_back(new Citadel::Player(name.str()));
        std::cout << players.back()->name() << " joined the game!"
            << std::endl;
    }

    for (i = 0; i < INIT_CARDS; i++)
    {
        p_it = players.begin();
        while (p_it != players.end())
        {
            (*p_it)->giveToHand(deck.getTop());
            p_it++;
        }
    }

    // initialise the rules according to number of players
    initGame();

    return EXIT_SUCCESS;
}

int Game::play()
{
    int ret = EXIT_SUCCESS;
    unsigned int i;
    std::vector<Citadel::Player *>::const_iterator p_it;
    int turn = 0;
    bool hasFirst = false;
    int crowned_index = getIndex(crowned);
    Citadel::Player *thief = 0;

    //
    // game turn
    //
    while (!isFinished())
    {
        unsigned int c;
        Citadel::Player *current = 0;
        Citadel::Character *character = 0;
        bool collectedTax = false;

        std::cout << std::endl << std::endl;
        std::cout << "===== ===== =====" << std::endl;
        std::cout << "Turn " << turn << " starts!" << std::endl;
        for (c = 0; c < players.size(); c++)
        {
            std::cout << players[c]->name() << " has "
                << players[c]->built().size() << "/" << goal_building
                << " buildings." << std::endl;
        }
        std::cout << "===== ===== =====" << std::endl;

        // shuffle characters and deal them
        // look for characters in order to find players

        c = giveCharacters();
        if (!c)
        {
            std::cerr << "failed to give character cards" << std::endl;
            return EXIT_FAILURE;
        }

        if (choose_visible_before > 0)
        {
            std::cout << std::endl;
            std::cout << "===== =====" << std::endl;
            std::cout << "The visible characters are: " << std::endl;

            for (c = 0; c < characters.size(); c++)
            {
                character = characters[c];

                if (character->visible)
                {
                    std::cout << character->level << " - "
                        << character->name << std::endl;
                }
            }
            std::cout << "===== =====" << std::endl;
        }

        std::cout << "Press enter to start the turn" << std::endl;
        getchar();

        for (c = 0; c < characters.size(); c++)
        {
            int choice = -1;
            int kept = 0;

            character = characters[c];

            if (!character->dead && !character->visible)
            {
                std::cout << std::endl;
                std::cout << "===== =====" << std::endl;
                std::cout << "It is the turn of the " << character->name
                    << " to play" << std::endl;

                current = getPlayer(c);
            }
            else
            {
                if (character->dead)
                {
                    std::cout << std::endl;
                    std::cout << "===== =====" << std::endl;
                    std::cout << "The " << character->name
                        << " is dead!" << std::endl;
                    std::cout << "===== =====" << std::endl;
                }
                continue;
            }

            if (!current)
            {
                std::cout << "Nobody has the " << character->name
                    << "." << std::endl;
                if (character->stolen)
                {
                    std::cout << "The " << thief->getCharacter()->name
                        << " goes back empty-handed to " << thief->name()
                        << "." << std::endl;
                }
                std::cout << "===== =====" << std::endl;
                continue;
            }
            else
            {
                std::cout << current->name() << " has the " << character->name
                    << "." << std::endl;
                std::cout << "Description: " << character->description
                    << std::endl;
                std::cout << "===== =====" << std::endl;
            }

            if (character->stolen)
            {
                int stolen = current->treasure();
                std::cout << "===== =====" << std::endl;
                if (stolen > 0)
                {
                    std::cout << "The " << thief->getCharacter()->name
                        << " steals " << stolen << " from " << current->name()
                        << " for " << thief->name() << "." << std::endl;

                    current->modifyTreasure(stolen*-1);
                    thief->modifyTreasure(stolen);
                }
                else
                {
                    std::cout << current->name() << " coffers are empty. "
                        << "The " << thief->getCharacter()->name
                        << " returns empty handed to "
                        << thief->name() << "." << std::endl;
                }
                std::cout << "===== =====" << std::endl;
            }

            if (character->power&Citadel::Character::POWER_CROWN)
            {
                if (crowned != current)
                {
                    std::cout << current->name() << " takes the crown from "
                        << crowned->name() << "." << std::endl;

                    crowned = current;
                    crowned_index = getIndex(crowned);
                }
                else
                {
                    std::cout << current->name() << " keeps the crown."
                        << std::endl;
                }
                std::cout << "===== =====" << std::endl;
            }
            if (character->power&Citadel::Character::POWER_CARDS)
            {
                std::cout << current->name() << " gets additional " << 2
                    << " cards." << std::endl;
                std::cout << "===== =====" << std::endl;

                for (i = 0; i < 2; i++)
                {
                    Citadel::Building *build = deck.getTop();
                    if (build)
                    {
                        current->giveToHand(build);
                    }
                }
            }
            if (character->power&Citadel::Character::POWER_RENT)
            {
                std::cout << current->name() << " gets " << 1
                    << " additional gold." << std::endl;
                std::cout << "===== =====" << std::endl;

                current->modifyTreasure(1);
            }

            std::cout << std::endl;
            std::cout << "Information about players:" << std::endl;

            for (i = 0; i < players.size(); i++)
            {
                std::cout << *(players[(crowned_index + i) % players.size()])
                    << std::endl;
            }

            // print current hand
            std::cout << std::endl;
            printHand(*current, false);

            choice = -1;
            while (-1 == choice)
            {
                std::list<Citadel::Building*>::const_iterator c_it;

                std::cout << std::endl;
                std::cout << "Choose action (0) get "
                    << current->nToGold() << " gold or (1) get "
                    << current->nToDraw() << " cards and keep "
                    << current->nToKeep() << "?" << std::endl;

                choice = -1;
                std::cout << "Choice: ";
                std::cin >> choice;
                getchar();

                if (choice < 0 || choice > 1)
                {
                    std::cout << "Invalid choice, try again!" << std::endl;
                    choice = -1;
                    continue;
                }
                if (choice)  // != 0 take card
                {
                    Citadel::Deck tmp;

                    i = 0;
                    while (i < current->nToDraw())
                    {
                        tmp.pushBack(deck.getTop());
                        i++;
                    }

                    // for each to keep
                    while (kept < static_cast<int>(current->nToKeep()))
                    {
                        std::cout << "Which card to keep? ("
                            << current->nToKeep() - kept
                            << " left to keep) " << std::endl;
                        printDeck(tmp.cards(), true);

                        choice = -1;
                        std::cout << "Choice: ";
                        std::cin >> choice;
                        getchar();

                        if (choice < 0)
                        {
                            choice = tmp.cards().size();
                        }

                        c_it = tmp.cards().begin();
                        i = 0;
                        while (c_it != tmp.cards().end())
                        {
                            if (i == choice)
                            {
                                current->giveToHand(*c_it);
                                tmp.take(*c_it);
                                kept++;
                                break;
                            }
                            c_it++;
                            i++;
                        }
                        if (i == tmp.cards().size())
                        {
                            std::cerr << "failed to find card " << choice
                                << " in list of " << tmp.cards().size()
                                << ". Try again." << std::endl;
                        }
                    }

                    // release the other ones
                    while (tmp.cards().size() > 0)
                    {
                        Citadel::Building *b = tmp.getTop();
                        std::cout << current->name() << " did not take "
                            << *b << "." << std::endl;
                        deck.pushBack(b);
                    }
                }
                else
                {
                    current->modifyTreasure(current->nToGold());
                    std::cout << "Getting " << current->nToGold()
                        << " gold, treasure is now "
                        << current->treasure() << "." << std::endl;
                }

                if (Citadel::Building::BUILD_N != character->taxing)
                {
                    std::cout << "===== =====" << std::endl;
                    std::cout << "The " << character->name
                        << " can collect taxes from the " << character->taxing
                        << " buildings. Should it be done now "
                        << "(otherwise done after building)? "
                        << "1 for now, other for later." << std::endl;

                    choice = 0;
                    std::cout << "Choice: ";
                    std::cin >> choice;
                    getchar();

                    if (1 == choice)
                    {
                        int collect = collectTaxes(current, character->taxing);

                        std::cout << "The " << character->name
                            << " collected " << collect << " gold from "
                            << character->taxing << " buildings for "
                            << current->name() << "." << std::endl;

                        collectedTax = true;
                        current->modifyTreasure(collect);
                    }
                    else
                    {
                        collectedTax = false;
                    }
                    std::cout << "===== =====" << std::endl;
                }

                kept = 0;
                while (kept < static_cast<int>(current->nToBuild()))
                {
                    choice = -1;
                    /*if (current->hand().size() == 0)
                    {
                        std::cout << current->name()
                            << " has no cards in hand. "
                            << "Building is not possible!"
                            << std::endl;
                        kept = (int)current->nToBuild();
                        continue;
                    }*/
                    while (-1 == choice)
                    {
                        std::cout << std::endl;
                        std::cout << current->name()
                            << " can still build "
                            << current->nToBuild() - kept
                            << " buildings (" << current->treasure()
                            << " gold left)!" << std::endl;
                        std::cout << "Select building to build (or >="
                            << current->hand().size()
                            << " to skip building)." << std::endl;
                        printHand(*current, true);

                        choice = -1;
                        std::cout << "Choice: ";
                        std::cin >> choice;
                        getchar();

                        if (choice < 0)
                        {
                            choice = current->hand().size();
                        }

                        c_it = current->hand().begin();
                        i = 0;
                        while (c_it != current->hand().end())
                        {
                            if (i == choice)
                            {
                                Citadel::Building *b = *c_it;
                                if (current->build(b, turn))
                                {
                                    std::cout << current->name()
                                        << " builds " << b->name()
                                        << " for " << b->price()
                                        << " gold." << std::endl;
                                }
                                else
                                {
                                    std::cout << "Cannot build " << *b << "."
                                        << std::endl;
                                    choice = -1;
                                }
                                break;
                            }
                            c_it++;
                            i++;
                        }
                        if (-1 != choice)
                        {
                            kept++;
                        }
                    }  // while choice build
                }  // while built it could

                current->setIsFirst(
                    current->built().size() >= goal_building
                    && !hasFirst);
            }  // while choice gold or cards

            if (Citadel::Building::BUILD_N != character->taxing
                && !collectedTax)
            {
                std::cout << "===== =====" << std::endl;
                std::cout << "The " << character->name
                    << " can collect taxes from the " << character->taxing
                    << " buildings. Should it be done now "
                    << "(otherwise taxes will not be collected)? "
                    << "1 for now, other will not collect taxes." << std::endl;

                choice = 0;
                std::cout << "Choice: ";
                std::cin >> choice;
                getchar();

                if (1 == choice)
                {
                    int collect = collectTaxes(current, character->taxing);

                    std::cout << "The " << character->name << " collected "
                        << collect << " gold from " << character->taxing
                        << " buildings for " << current->name() << "."
                        << std::endl;

                    collectedTax = true;
                    current->modifyTreasure(collect);
                }
                std::cout << "===== =====" << std::endl;
            }

            if (character->power&Citadel::Character::POWER_KILL)
            {
                power_kill(current);
            }

            if (character->power&Citadel::Character::POWER_STEAL)
            {
                thief = current;
                power_steal(current);
            }

            if (character->power&Citadel::Character::POWER_DESTROY)
            {
                power_destroy(current);
            }
        }  // while characters

        resetCharacters();
        thief = 0;

        turn++;
    }  // while !isFinished

    return EXIT_SUCCESS;
}

int Game::findWinner()
{
    unsigned int maxPoints = 0;
    int nMax = 1;
    unsigned points[MAX_PLAYERS];
    std::vector<Citadel::Player *>::const_iterator p_it;
    unsigned int i;

    std::cout << std::endl << std::endl;
    std::cout << "===== ===== =====" << std::endl;
    std::cout << "The game is over!" << std::endl;
    std::cout << "===== ===== =====" << std::endl;
    std::cout << std::endl;
    std::cout << "Here are the cities:" << std::endl;

    p_it = players.begin();
    i = 0;
    while (p_it != players.end())
    {
        points[i] = computePoints(*(*p_it));
        std::cout << *(*p_it)
            << "Points: " << points[i]
            << std::endl;

        if (points[i] == maxPoints)
        {
            nMax++;
        }
        else if (points[i] > maxPoints)
        {
            maxPoints = points[i];
            nMax = 1;
        }

        p_it++;
        i++;
        std::cout << std::endl;
    }

    std::cout << std::endl << std::endl;
    std::cout << "===== ===== =====" << std::endl;
    p_it = players.begin();
    i = 0;
    if (nMax > 1)
    {
        std::cout << "The winners are";
    }
    else
    {
        std::cout << "The winner is";
    }
    while (p_it != players.end())
    {
        if (static_cast<int>(points[i]) == maxPoints)
        {
            std::cout << " " << (*p_it)->name();
        }
        p_it++;
        i++;
    }
    std::cout << " with " << maxPoints << " points." << std::endl;
    std::cout << "===== ===== =====" << std::endl;
    std::cout << "Press enter to end the game" << std::endl;
    getchar();

    return EXIT_SUCCESS;
}

int Game::collectTaxes(const Citadel::Player *current,
    const enum Citadel::Building::Type &taxing)
{
    int collect = 0;
    std::list<Citadel::Building*>::const_iterator it =
        current->built().begin();

    while (it != current->built().end())
    {
        if ((*it)->type() == taxing)
        {
            collect++;
        }
        it++;
    }

    return collect;
}

int Game::power_kill(Citadel::Player *current)
{
    Citadel::Character *character = current->getCharacter();
    int choice;
    int killed = 0;

    std::cout << "===== =====" << std::endl;
    std::cout << "Kill a character? Enter character position ("
        << character->level + 1 << " to "
        << characters.size() - 1 << " - 0 or >="
        << characters.size() << " to cancel)." << std::endl;

    choice = character->level;
    std::cout << "Choice: ";
    std::cin >> choice;
    getchar();

    if (choice > static_cast<int>(character->level)
        && choice < static_cast<int>(characters.size()))
    {
        std::cout << "The " << character->name << " kills the "
            << characters[choice]->name << "." << std::endl;
        characters[choice]->dead = true;
        killed = 1;
    }
    else
    {
        std::cout << "The " << character->name
            << " does not kill anybody." << std::endl;
    }
    std::cout << "===== =====" << std::endl;
    return killed;
}

int Game::power_steal(Citadel::Player *current)
{
    Citadel::Character *character = current->getCharacter();
    int choice;
    int stolen = 0;

    std::cout << "===== =====" << std::endl;
    do
    {
        std::cout << "Steal from a character? "
            << "Enter character position ("
            << character->level + 1 << " to "
            << characters.size() - 1 << " - 0 or >="
            << characters.size() << " to cancel)." << std::endl;
        std::cout << "Cannot steal a dead or the assassin."
            << std::endl;

        choice = character->level;
        std::cout << "Choice: ";
        std::cin >> choice;
        getchar();

        if (choice > static_cast<int>(character->level)
            && choice < static_cast<int>(characters.size()))
        {
            if (!characters[choice]->dead)
            {
                std::cout << "The " << character->name
                    << " will steal from the "
                    << characters[choice]->name << "."
                    << std::endl;
                characters[choice]->stolen = true;
                choice = character->level;  // to stop the loop

                stolen = 1;
            }
            else
            {
                std::cout << "Cannot steal from the dead!"
                    << std::endl;
            }
        }
        else
        {
            std::cout << "The " << character->name
                << " does not steal from anybody." << std::endl;
            choice = character->level;  // to stop the loop
        }
    } while (choice != character->level);
    std::cout << "===== =====" << std::endl;

    return stolen;
}

int Game::power_cards(Citadel::Player *current)
{
    Citadel::Character *character = current->getCharacter();
    // int choice;

    return 0;
}

int Game::power_destroy(Citadel::Player *current)
{
    Citadel::Character *character = current->getCharacter();
    int choice;
    unsigned int c;
    int destroyed = 0;

    std::cout << "===== =====" << std::endl;
    std::cout << "Muster an army to destroy buildings? ";
    choice = -1;

    while (choice < 0)
    {
        std::cout << "Choose a player (Bishop is protected) or >= "
            << players.size() << " to cancel:" << std::endl;
        for (c = 0; c < players.size(); c++)
        {
            if (players[c] != current)
            {
                std::cout << c << " - " << players[c]->name()
                    << " - " << players[c]->getCharacter()->name
                    << std::endl;
            }
            else
            {
                std::cout << c << " - yourself" << std::endl;
            }
        }

        choice = -1;
        std::cout << "Choice: ";
        std::cin >> choice;
        getchar();

        if (choice >= static_cast<int>(players.size()) || choice < 0)
        {
            std::cout << "No army was mustered." << std::endl;
        }
        else
        {
            c = choice;
            if (players[c]->getCharacter()->power
                &Citadel::Character::POWER_IMMUNITY)
            {
                std::cout << players[choice]->name() << " has the "
                    << players[c]->getCharacter()->name
                    << " which grant immunity against the "
                    << character->name
                    << ". Choose another player." << std::endl;
                choice = -1;
            }
            else if (players[c]->isFirst())
            {
                std::cout << players[c]->name()
                    << " is the first to have reached "
                    << goal_building
                    << " buildings, he cannot be attacked. "
                    << "Choose another player." << std::endl;
                choice = -1;
            }
            else
            {
                std::list<Citadel::Building*>::const_iterator it;
                std::vector<Citadel::Building*> list;
                int muster;

                std::cout << "Choose a building from "
                    << players[c]->name()
                    << " to destroy from (>=" <<
                    players[c]->built().size()
                    << " to cancel):" << std::endl;

                choice = 0;
                it = players[c]->built().begin();
                while (it != players[c]->built().end())
                {
                    muster = (*it)->value() - 1;
                    /* todo handle the special building that
                    * cannot be destroyed */
                    /* todo handle the power that makes the normal
                    * value for destruction */
                    std::cout << choice << " - " << (*it)->name()
                        << " to destroy: " << muster;
                    if (muster > current->treasure())
                    {
                        std::cout << " [cannot destroy]";
                    }
                    list.push_back(*it);
                    std::cout << std::endl;
                    choice++;
                    it++;
                }

                choice = players[c]->built().size();
                std::cout << "Choice: ";
                std::cin >> choice;
                getchar();

                if (choice < static_cast<int>(players[c]->built().size()))
                {
                    muster = list[choice]->value() - 1;
                    if (muster > current->treasure())
                    {
                        std::cout << "Cannot destroy the "
                            << list[choice]->name() << " from "
                            << players[c]->name() << "'s city."
                            << " Choose again." << std::endl;
                        choice = -1;
                    }
                    else
                    {
                        if (muster > 0)
                        {
                            current->modifyTreasure(-1 * muster);
                        }
                        players[c]->takeFromBuilt(list[choice]);

                        if (current != players[c])
                        {
                            std::cout << current->name()
                                << " pays the " << character->name
                                << " " << muster
                                << " gold to destroys the "
                                << list[choice]->name()
                                << " from " << players[c]->name()
                                << "'s city!" << std::endl;
                        }
                        else
                        {
                            std::cout << current->name()
                                << " pays the " << character->name
                                << " " << muster
                                << " gold to destroys the "
                                << list[choice]->name()
                                << " from his/her own city!"
                                << std::endl;
                        }
                        destroyed = 1;
                    }
                }
                else
                {
                    std::cout << "No city was attacked by the "
                        << character->name << "." << std::endl;
                }
                list.clear();
            }
        }
    }
    std::cout << "===== =====" << std::endl;
    return destroyed;
}

int main(int argc, const char *argv[])
{
    int ret;
    unsigned int seed = (unsigned int)time(0);
    Game game;

    srand(seed);

#ifndef N_DEBUG
    std::cout << "random seed " << seed << std::endl;
#endif

    ret = game.createGame(MIN_PLAYERS);

    if (EXIT_SUCCESS != ret)
    {
        std::cerr << "failed to create game" << std::endl;
        return ret;
    }

    ret = game.play();
    if (EXIT_SUCCESS != ret)
    {
        std::cerr << "failed when playing game" << std::endl;
        return ret;
    }

    ret = game.findWinner();
    if (EXIT_SUCCESS != ret)
    {
        std::cerr << "failed when searching for winner" << std::endl;
    }

    return ret;
}

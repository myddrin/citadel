
#include "citadel/player.hpp"
#include "citadel/building.hpp"

#include "citadel/character.hpp"

Citadel::Player::Player(const std::string &name, int gold,
	unsigned int toDraw, unsigned int toKeep, unsigned int toGold)
	: _name(name), _treasure(gold),
	_initNToDraw(toDraw), _initNToKeep(toKeep), _initNGoldDraw(toGold), 
    _initNToBuild(DEF_TO_BUILD), _powerNToBuild(PWR_TO_BUILD),
    _nToDraw(toDraw), _nToKeep(toKeep), 
	_nGoldDraw(toGold), _nToBuild(DEF_TO_BUILD), _isFirst(false)
{
    _character = 0;
}

void Citadel::Player::computePower()
{
	_nToDraw = _initNToDraw;
	_nToKeep = _initNToKeep;
	_nGoldDraw = _initNGoldDraw;
	_nToBuild = _initNToBuild;

	if (_character)
	{
        if (_character->power&Character::POWER_BUILD)
        {
            _nToBuild = _powerNToBuild;
        }
	}

    // check for wonders for special powers
}

//
// accessors
//

int Citadel::Player::treasure() const
{
	return _treasure;
}

int Citadel::Player::modifyTreasure(int exchange)
{
	if (_treasure + exchange >= 0)
	{
		_treasure += exchange;
	}
	return _treasure;
}

const std::string& Citadel::Player::name() const
{
	return _name;
}

unsigned int Citadel::Player::nToDraw() const
{
	return _nToDraw;
}

unsigned int Citadel::Player::nToKeep() const
{
	return _nToKeep;
}

unsigned int Citadel::Player::nToGold() const
{
	return _nGoldDraw;
}

unsigned int Citadel::Player::nToBuild() const
{
	return _nToBuild;
}

bool Citadel::Player::isFirst() const
{
	return _isFirst;
}

void Citadel::Player::setIsFirst(bool f)
{
	_isFirst = f;
}

//
// built access
//

bool Citadel::Player::canBuild(const Citadel::Building *b) const
{
	bool can = false;
	if (!b)
	{
		return can;
	}

	// todo if player has properties take them into account
	can = (_treasure >= (int)b->price()) && (_hand.search(b)) 
		&& (!_built.search(b->name()));

	return can;
}

bool Citadel::Player::build(Citadel::Building *b, int turn)
{
	int oldTreasure = _treasure;
	if (!canBuild(b))
	{
		return false;
	}
	
	modifyTreasure(b->price()*-1);
	_hand.take(b);
	_built.pushBack(b);

	b->setValue(b->price());
	b->setBuiltAtTurn(turn);

	return true;
}

bool Citadel::Player::takeFromBuilt(Building *b)
{
	if (!b || !_built.search(b))
	{
		return false;
	}
	
	_built.take(b);

	// todo recompute player's properties

	return true;
}

const std::list<Citadel::Building*>& Citadel::Player::built(void) const
{
	return _built.cards();
}

//
// hand access
//

bool Citadel::Player::takeFromHand(Building *b)
{
	if (!b || !_hand.search(b))
	{
		return false;
	}
	_hand.take(b);
	return true;
}

void Citadel::Player::giveToHand(Building *b)
{
	_hand.pushBack(b);
}

const std::list<Citadel::Building*>& Citadel::Player::hand(void) const
{
	return _hand.cards();
}

//
// character access
//

Citadel::Character* Citadel::Player::getCharacter()
{
    return _character;
}

void Citadel::Player::setCharacter(Citadel::Character *character)
{
    this->_character = character;
}

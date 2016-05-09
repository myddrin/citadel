/**
 * Copyright (c) 2016 Thomas Richard
 *
 * Following MIT license (see copying.txt)
 *
 * The software is provided "as is", without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement.
 */
#include "citadel/building.hpp"

#include <string>

Citadel::Building::Building(const std::string &name, unsigned int price,
    enum Type t, const std::string &desc)
        :_name(name), _description(desc), _price(price), _type(t),
        _value(price), _builtAtTurn(-1)
{
}

//
// accessors
//

std::string Citadel::Building::name() const
{
    return _name;
}

std::string Citadel::Building::description() const
{
    return _description;
}

unsigned int Citadel::Building::price() const
{
    return _price;
}

unsigned int Citadel::Building::value() const
{
    return _value;
}

void Citadel::Building::setValue(unsigned int v)
{
    _value = v;
}

enum Citadel::Building::Type Citadel::Building::type() const
{
    return _type;
}

int Citadel::Building::builtAtTurn() const
{
    return _builtAtTurn;
}

void Citadel::Building::setBuiltAtTurn(int turn)
{
    _builtAtTurn = turn;
}

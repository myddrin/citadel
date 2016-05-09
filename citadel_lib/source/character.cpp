/**
 * Copyright (c) 2016 Thomas Richard
 *
 * Following MIT license (see copying.txt)
 *
 * The software is provided "as is", without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement.
 */
#include "citadel/character.hpp"

#include <string>

Citadel::Character::Character(const std::string &_name, unsigned int _level,
    unsigned int _power, enum Building::Type _taxing,
    const std::string &_description)
    : name(_name), description(_description), level(_level), power(_power),
    taxing(_taxing), dead(false), visible(false), stolen(false)
{
    // reset();
}

void Citadel::Character::reset()
{
    dead = false;
    visible = false;
    stolen = false;
}

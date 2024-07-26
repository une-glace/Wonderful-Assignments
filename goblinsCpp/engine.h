#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <iostream>
#include "field.h"

extern bool debugMode;

// Load map
Field* loadMap(std::istream& ise, Vector<Unit*>& units);

// Main loop for playing the game
void play(Field& field, std::istream& is, std::ostream& os,Vector<Unit*>& units);

#endif // ENGINE_H_INCLUDED

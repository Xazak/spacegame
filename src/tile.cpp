/*   tile.cpp
AUTH xazak
DATE Feb 11 2020
DESC Contains the implementation of the Tile class.
*/

#include "tile.hpp"
#include <string>

using namespace std;

// CTOR DEFAULT
Tile::Tile() :
sigil(0x0470),
color("magenta"),
explored(true)
{	}
// CTOR SPECIFIC
Tile::Tile(int inputSigil, string inputColor, bool isExplored) :
sigil(inputSigil),
color(inputColor),
explored(isExplored)
{	}


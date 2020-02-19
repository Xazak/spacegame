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
color(0x55FF00FF),
bkcolor(0x99FF00FF),
name("TILE_PROTOTYPE"),
explored(true),
obstructs(false)
{	}
// CTOR SPECIFIC
//Tile::Tile(int inputSigil, string inputColor, bool isExplored, bool blocksMovement) :
Tile::Tile(int inputSigil, int inputColor, int inputBKColor, string inputName, bool isExplored, bool blocksMovement) :
sigil(inputSigil),
color(inputColor),
bkcolor(inputBKColor),
name(inputName),
explored(isExplored),
obstructs(blocksMovement)
{	}

// **** SPECIFIC TILE TYPES
// **** Wall
Wall::Wall() :
Tile(0x002B, 0xAAAAAAAA, 0xFF777777, "wall", true, true)
{	}
Floor::Floor() :
Tile(0x00AC, 0xAA333333, 0x55777777, "floor", true, false)
{	}
Vacuum::Vacuum() :
Tile(0x00B7, 0x00000000, 0x33333333, "vacuum", true, false)
{	}

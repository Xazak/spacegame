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
obstructs(false),
opaque(false)
{	}
// CTOR SPECIFIC
//Tile::Tile(int inputSigil, string inputColor, bool isExplored, bool blocksMovement) :
Tile::Tile(int inputSigil, int inputColor, int inputBKColor, string inputName, bool isExplored, bool blocksMovement, bool blocksVision) :
sigil(inputSigil),
color(inputColor),
bkcolor(inputBKColor),
name(inputName),
explored(isExplored),
obstructs(blocksMovement),
opaque(blocksVision)
{	}

// **** SPECIFIC TILE TYPES
// Five basic types, ranging in mass:

//   sigil,    fg color,   bg color,   name,   expl. obst.
Vacuum::Vacuum() : // Empty vacuum
Tile(0x2022, 0x22FFFFFF, 0xFF000000, "vacuum", true, false, false)
{	}
Tunnel::Tunnel() : // A crawlspace or tunnel, big enough for a prone human
Tile(0x00A4, 0xFF999999, 0xFF111111, "tunnel", true, false, false)
{	}
Floor::Floor() : // A floor, such as would have carpet or steel grating
Tile(0x00AC, 0xFF333333, 0xFF111111, "floor", true, false, false)
{	}
Wall::Wall() : // A wall, like a partition or a bulkhead
Tile(0x002B, 0xFFAAAAAA, 0xFF111111, "wall", true, true, true)
{	}
Solid::Solid() : // A solid tile of something, such as might fill ship bulkheads
Tile(0x2588, 0xFFDDDDDD, 0xFF111111, "tunnel", true, false, true)
{	}

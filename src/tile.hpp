/*   tile.hpp
AUTH xazak
DATE Feb 11 2020
DESC Contains the definitions and methods of the Tile class.
*/

#ifndef SPACEGAME_TILE
#define SPACEGAME_TILE

#include <string>

struct Tile {
	int			sigil;
	std::string color;
	bool		explored;
//	bool		obstructs;
//	bool		opaque;
//	Actor*		occupant;
//	Actor*		itemList;
	Tile();
	Tile(int inputSigil, std::string inputColor, bool isExplored);
};

#endif //SPACEGAME_TILE

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
//	std::string	color;
	int			color;
	int			bkcolor;
	std::string	name;
	bool		explored;
	bool		obstructs;
//	bool		opaque;
//	Actor*		occupant;
//	Actor*		itemList;
	Tile();
//	Tile(int inputSigil, std::string inputColor, bool isExplored, bool blocksMovement);
	Tile(int inputSigil, int inputColor, int inputBKColor, std::string inputName, bool isExplored, bool blocksMovement);
};

struct Wall : public Tile {
	Wall();
};
struct Floor : public Tile {
	Floor();
};
struct Vacuum : public Tile {
	Vacuum();
};
#endif //SPACEGAME_TILE

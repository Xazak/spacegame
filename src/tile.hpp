/*   tile.hpp
AUTH xazak
DATE Feb 11 2020
DESC Contains the definitions and methods of the Tile class.
*/

#ifndef SPACEGAME_TILE
#define SPACEGAME_TILE

#include <string>
#include <list>

class Actor;
class Container;
struct Tile {
	int			sigil;
//	std::string	color;
	int			color;
	int			bkcolor;
	std::string	name;
	bool		explored;
	bool		obstructs;
	bool		opaque;
	Actor*		occupant;
	Actor*		contents; // only one item allowed per tile for now
	Tile();
	Tile(int inputSigil, int inputColor, int inputBKColor, std::string inputName, bool isExplored, bool blocksMovement, bool blocksVision);
};

struct Vacuum : public Tile {
	Vacuum();
};
struct Tunnel : public Tile {
	Tunnel();
};
struct Floor : public Tile {
	Floor();
};
struct Wall : public Tile {
	Wall();
};
struct Solid : public Tile {
	Solid();
};
#endif //SPACEGAME_TILE

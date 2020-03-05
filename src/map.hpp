/*  map.hpp
AUTH xazak
DATE Jan 30 2020
DESC Contains the definitions and methods of the GameMap class.
*/

#ifndef SPACEGAME_MAP
#define SPACEGAME_MAP

#include "tile.hpp"
#include "main.hpp"
#include <list>

class Actor; // fwd-decl
class GameMap {
	public:
		struct MapIterator {
			MapIterator();
			MapIterator(GameMap* inputSource);
//			MapIterator(Tile** inputSource);
		//	~GameMapIterator();
			GameMap* mapSource;
			Tile* target;
			uint index;
			void linkTo(GameMap* inputSource);
			void reset();
			void next();
			void previous();
			bool hasNext();
		} mapIndexIter;
		GameMap();
		GameMap(uint inputWidth, uint inputHeight);
		~GameMap();
		void generateMap();
		void generateMap(uint newWidth, uint newHeight);
		// GETS
		uint getWidth() { return width; }
		uint getHeight() { return height; }
		uint getSize() { return (width * height); }
		int getTileSigil(uint xPos, uint yPos);
		int getTileColor(uint xPos, uint yPos);
		int getTileBkcolor(uint xPos, uint yPos);
		std::string getTileName(uint xPos, uint yPos);
		bool isBlocked(uint xPos, uint yPos);
		bool isBlocked(cpair target);

		Actor* getOccupant(uint xPos, uint yPos);
		Actor* getOccupant(cpair inputLocation);
		Actor* getContents(uint xPos, uint yPos);
		Actor* getContents(cpair inputLocation);
		void setOccupant(Actor *occupier); // occupancy is a reflexive relation
		void unsetOccupant(uint xPos, uint yPos);
		void setObstruction(uint xPos, uint yPos, bool inputValue);
		void setObstruction(cpair inputLocation, bool inputValue);
		void registerItem(Actor* newItem, uint xPos, uint yPos);
		void registerItem(Actor* newItem, cpair inputLocation);
		void removeItem(Actor *target);

		std::list<Actor*> allActors; // List of pointers to ALL actors in this

	private:
		Tile** mapArray;
		uint width;
		uint height;
};

#endif //SPACEGAME_MAP

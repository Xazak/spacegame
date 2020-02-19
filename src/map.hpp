/*  map.hpp
AUTH xazak
DATE Jan 30 2020
DESC Contains the definitions and methods of the GameMap class.
*/

#ifndef SPACEGAME_MAP
#define SPACEGAME_MAP

#include "tile.hpp"
#include "main.hpp"

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

	private:
		Tile** mapArray;
		uint width;
		uint height;
};

#endif //SPACEGAME_MAP

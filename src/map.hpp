/*  map.hpp
AUTH xazak
DATE Jan 30 2020
DESC Contains the definitions and methods of the GameMap class.
*/

#ifndef SPACEGAME_MAP
#define SPACEGAME_MAP

#include "tile.hpp"

class GameMap {
	private:
		struct GameMapIterator {
			GameMapIterator();
			GameMapIterator(GameMap* inputSource);
		//	~GameMapIterator();
			GameMap* mapSource;
			uint index;
			void linkToMap(Tile* targetMap);
			void moveToStart();
			void forward();
			void backward();
			bool isAtEnd();
		};
	public:
		GameMap();
		GameMap(uint inputWidth, uint inputHeight);
		~GameMap();
		void generateMap();

		// GETS
		uint getWidth() { return width; }
		uint getHeight() { return height; }
		uint getSize() { return (width * height); }
		std::string getTileName(uint xPos, uint yPos);
		bool isBlocked(uint xPos, uint yPos);

		GameMapIterator mapIterator;
		Tile* mapArray;

	private:
		uint width;
		uint height;
		uint size;
};

#endif //SPACEGAME_MAP

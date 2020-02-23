/*  map.hpp
AUTH xazak
DATE Jan 30 2020
DESC Contains the definitions and methods of the Map and Tile classes.
*/

#include "map.hpp"
#include "main.hpp"

using namespace std;

// **** GAMEMAP METHODS
// CTOR Default
GameMap::GameMap() :
width(0), height(0)
{	}
// CTOR SPECIFIC
GameMap::GameMap(uint inputWidth, uint inputHeight) :
width(inputWidth), height(inputHeight)
{	generateMap(inputWidth, inputHeight); }
// DTOR
GameMap::~GameMap()
{	if (*mapArray) {
		for (uint index = 0; index < this->getSize(); index++) {
			if (mapArray[index] != nullptr) delete mapArray[index];
		}
	}
	delete mapArray;
}
// **** HELPERS
string GameMap::getTileName(uint xPos, uint yPos) {
	return mapArray[xPos + yPos * width]->name;
}
bool GameMap::isBlocked(uint xPos, uint yPos) {
//	LOGMSG("Checking for blockage at " << xPos << ", " << yPos);
	if (xPos >= width || yPos >= height) {
		return false;
	} else {
		return mapArray[xPos + yPos * width]->obstructs;
	}
}
bool GameMap::isBlocked(cpair target) {
//	LOGMSG("called");
	return isBlocked(target.x, target.y);
}
// **** UTILITIES
void GameMap::generateMap() {
	if (this->width <= 0 || this->height <= 0) {
		this->generateMap(width, height);
	}
}
void GameMap::generateMap(uint newWidth, uint newHeight) {
	width = newWidth;
	height = newHeight;
	if (width == 0 || height == 0) {
		ERRMSG("Invalid map dimensions specified!");
		return;
	}
	// Generates rooms and places them into the map array
	mapArray = new Tile*[width * height]; // Create an empty pointer map
	mapIndexIter.linkTo(this); // Init the public iterator
	// Create the actual tile objects
	// mapArray[x + y * width]
	/* Build a test room
	for (uint xOffset = 0; xOffset < width; xOffset++) {
		for (uint yOffset = 0; yOffset < height; yOffset++) {
			if (xOffset == 0 || xOffset == width - 1 ) {
				mapArray[xOffset * width + yOffset] = new Wall();
			} else if (yOffset == 0 || yOffset == height - 1) {
				mapArray[xOffset * width + yOffset] = new Wall();
			} else {
				mapArray[xOffset * width + yOffset] = new Floor();
			}
		}
	}
	*/
	
/*	void add(int x, int y, int w, int h) {
        for (int i = x; i < x + w; i++) {
            set(i, y, 1);
            set(i, y + h - 1, 1);
        }
        for (int i = y + 1; i < y + h - 1; i++) {
            set(x, i, 1);
            set(x + w - 1, i, 1);
        }
    }*/

	// Build a test structure
	uint arrWidth = 28;
	uint tempWidth = arrWidth;
	uint arrHeight = 12;
	uint tempHeight = arrHeight;
	uint mapTemplate[arrWidth * arrHeight] = {0}; // 0 == vacuum (see Tile class)
	uint xOffset = 0;
	uint yOffset = 0;
	uint index = 0;
	for (index = xOffset; index < (xOffset + tempWidth); index++) {
		mapTemplate[index + yOffset * tempWidth] = 5;
		mapTemplate[index + (yOffset + tempHeight - 1) * arrWidth] = 5;
	}
	for (index = yOffset; index < (yOffset + tempHeight); index++) {
		mapTemplate[xOffset + index * tempWidth] = 5;
		mapTemplate[(xOffset + tempWidth - 1) + index * arrWidth] = 5;
	}
//	xOffset++;
//	yOffset++;
//	tempWidth--;
//	tempHeight--;
	for (index = xOffset; index < (xOffset + tempWidth); index++) {
		mapTemplate[index + yOffset * tempWidth] = 4;
		mapTemplate[index + (yOffset + tempHeight - 2) * arrWidth] = 4;
	}
	for (index = yOffset; index < (yOffset + tempHeight); index++) {
		mapTemplate[xOffset + index * tempWidth] = 4;
		mapTemplate[(xOffset + tempWidth - 2) + index * arrWidth] = 4;
	}
	/*
	xOffset++;
	yOffset++;
	tempWidth--;
	tempHeight--;
	for ( ; xOffset < tempWidth; xOffset++) {
		for ( ; yOffset < tempHeight; yOffset++) {
			mapTemplate[xOffset + yOffset * arrWidth] = 3;
		}
	}
	*/
	// FIXME: fill in some internal walls
	// Use the template to build the map
	uint tileType = 0;
	for (xOffset = 0; xOffset < width; xOffset++) {
		for (yOffset = 0; yOffset < height; yOffset++) {
//			mapArray[xOffset + yOffset * width] = new Vacuum();
			if (xOffset > 0 && xOffset <= arrWidth) {
				if (yOffset > 0 && yOffset <= arrHeight) {
					// call into the template
					mapArray[xOffset + yOffset * width] = new Floor();
					tileType = mapTemplate[(xOffset - 1) + (yOffset - 1) * arrWidth];
					clog << tileType;
					/*
					switch(tileType) {
						case 1:
							mapArray[xOffset + yOffset * arrWidth] = new Vacuum();
							break;
						case 2:
							mapArray[xOffset + yOffset * arrWidth] = new Tunnel();
							break;
						case 3:
							mapArray[xOffset + yOffset * arrWidth] = new Floor();
							break;
						case 4:
							mapArray[xOffset + yOffset * arrWidth] = new Wall();
							break;
						case 5:
							mapArray[xOffset + yOffset * arrWidth] = new Solid();
							break;
						default:
							mapArray[xOffset + yOffset * arrWidth] = new Vacuum();
							break;
					}
					*/
				} else {
					mapArray[xOffset + yOffset * width] = new Vacuum();
				}
			} else {
				mapArray[xOffset + yOffset * width] = new Vacuum();
			}
			/*
			if (xOffset == 0 || xOffset >= arrWidth) {
				mapArray[xOffset * width + yOffset] = new Vacuum();
			} else if (yOffset == 0 || yOffset >= arrHeight) {
				mapArray[xOffset * width + yOffset] = new Vacuum();
			} else {
				tileType = mapTemplate[(xOffset - 1) + (yOffset - 1) * arrWidth];
				switch(tileType) {
					case 1:
						mapArray[xOffset * arrWidth + yOffset] = new Vacuum();
						break;
					case 2:
						mapArray[xOffset * arrWidth + yOffset] = new Tunnel();
						break;
					case 3:
						mapArray[xOffset * arrWidth + yOffset] = new Floor();
						break;
					case 4:
						mapArray[xOffset * arrWidth + yOffset] = new Wall();
						break;
					case 5:
						mapArray[xOffset * arrWidth + yOffset] = new Solid();
						break;
					default:
						mapArray[xOffset * arrWidth + yOffset] = new Vacuum();
						break;
				}
			}
			*/
		}
		clog << endl;
	}
	LOGMSG("Tilemap created: " << width << "x" << height);
}
int GameMap::getTileSigil(uint xPos, uint yPos) {
	return this->mapArray[xPos + yPos * width]->sigil;
}
int GameMap::getTileColor(uint xPos, uint yPos) {
	return this->mapArray[xPos + yPos * width]->color;
}
int GameMap::getTileBkcolor(uint xPos, uint yPos) {
	return this->mapArray[xPos + yPos * width]->bkcolor;
}

// *************************
// **** MAP ITERATOR METHODS
GameMap::MapIterator::MapIterator() :
mapSource(nullptr),
target(nullptr),
index(0)
{	}
GameMap::MapIterator::MapIterator(GameMap* inputSource) :
mapSource(inputSource),
index(0)
{	target = mapSource->mapArray[0]; }
void GameMap::MapIterator::linkTo(GameMap* targetMap) {
	mapSource = targetMap;
	target = mapSource->mapArray[0];
	index = 0;
}
void GameMap::MapIterator::reset() {
	index = 0;
	target = mapSource->mapArray[0];
}
void GameMap::MapIterator::next() {
	if (index + 1 < mapSource->getSize()) index++;
	target = mapSource->mapArray[index];
}
void GameMap::MapIterator::previous() {
	if (index != 0) index--;
	target = mapSource->mapArray[index];
}
bool GameMap::MapIterator::hasNext() {
	return ( index < (mapSource->getSize() - 1) );
}

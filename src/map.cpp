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
	return mapArray[xPos * width + yPos]->name;
}
bool GameMap::isBlocked(uint xPos, uint yPos) {
//	LOGMSG("Checking for blockage at " << xPos << ", " << yPos);
	if (xPos >= width || yPos >= height) {
		return false;
	} else {
		return mapArray[xPos * width + yPos]->obstructs;
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
	// mapArray[x * width + y]
	// Build a test room
	for (uint xOffset = 0; xOffset < width; xOffset++) {
		for (uint yOffset = 0; yOffset < height; yOffset++) {
			if (xOffset == 3 || xOffset == 13) {
				mapArray[xOffset * width + yOffset] = new Wall();
			} else if (yOffset == 3 || yOffset == 13) {
				mapArray[xOffset * width + yOffset] = new Wall();
			} else {
				mapArray[xOffset * width + yOffset] = new Floor();
			}
		}
	}
//	LOGMSG("Tilemap created: " << width << "x" << height);
}
int GameMap::getTileSigil(uint xPos, uint yPos) {
	return this->mapArray[xPos * width + yPos]->sigil;
}
int GameMap::getTileColor(uint xPos, uint yPos) {
	return this->mapArray[xPos * width + yPos]->color;
}
int GameMap::getTileBkcolor(uint xPos, uint yPos) {
	return this->mapArray[xPos * width + yPos]->bkcolor;
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

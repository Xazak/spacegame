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
width(20), height(25), size((width * height))
{	generateMap(); }
// CTOR SPECIFIC
GameMap::GameMap(uint inputWidth, uint inputHeight) :
width(inputWidth), height(inputHeight), size((width * height))
{	generateMap(); }
// DTOR
GameMap::~GameMap()
{	delete [] mapArray; }
// **** UTILITIES
void GameMap::generateMap() {
	// Generates rooms and places them into the map array
	mapArray = new Tile[width * height]; // Create a map of default tiles
	mapIterator.linkToMap(mapArray);
	LOGMSG("Tilemap created: " << width << "x" << height);
}
// **** MAP ITERATOR METHODS (UNDER CONSTRUCTION)
// CTOR DEFAULT
GameMap::GameMapIterator::GameMapIterator() :
mapSource(nullptr),
index(0)
{	}
// CTOR SPECIFIC
GameMap::GameMapIterator::GameMapIterator(GameMap* inputSource) :
mapSource(inputSource),
index(0)
{	}
void GameMap::GameMapIterator::linkToMap(Tile* targetMap) {
	// Ensures that the iterator is pointing to a map
//	mapSource = &targetMap[0];
	index = 0;
}
void GameMap::GameMapIterator::moveToStart() {
//	mapSource = &mapSource[0];
	index = 0;
}
void GameMap::GameMapIterator::forward() {
	index++;
//	mapSource = &mapSource[index];
}
void GameMap::GameMapIterator::backward() {
	index--;
//	mapSource = &mapSource[index];
}
bool GameMap::GameMapIterator::isAtEnd() {
	return (index == mapSource->size - 1);
//	return true;
}

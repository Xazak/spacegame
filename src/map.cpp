/*  map.hpp
AUTH xazak
DATE Jan 30 2020
DESC Contains the definitions and methods of the Map and Tile classes.
*/

#include "actor.hpp"
#include "map.hpp"
#include "main.hpp"
#include "item.hpp"
#include "container.hpp"

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
//		LOGMSG("Target is out of bounds");
		return false;
	} else {
//		LOGMSG("Using map tile obstruction value: " << mapArray[xPos + yPos * width]->obstructs);
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
	/* DISABLED: Build a test room
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
	/* note: RAINCOMPLEX BOXDRAW METHOD
	void add(int x, int y, int w, int h) {
			for (int i = x; i < x + w; i++) {
				set(i, y, 1);
				set(i, y + h - 1, 1);
			}
			for (int i = y + 1; i < y + h - 1; i++) {
				set(x, i, 1);
				set(x + w - 1, i, 1);
			}
		}*/

	// *** TEST STRUCTURE
	// Start by generating a template
	uint arrWidth = 28;
	uint tempWidth = arrWidth;
	uint arrHeight = 12;
	uint tempHeight = arrHeight;
	uint mapTemplate[arrWidth * arrHeight] = {0}; // 0 == vacuum (see Tile class)
	uint xOffset = 0;
	uint yOffset = 0;
	uint index = 0;
	// Draw the 'solid' hull around the outside
	for (index = xOffset; index < (xOffset + tempWidth); index++) {
		mapTemplate[index + yOffset * tempWidth] = 5;
		mapTemplate[index + (yOffset + tempHeight - 1) * arrWidth] = 5;
	}
	for (index = yOffset; index < (yOffset + tempHeight); index++) {
		mapTemplate[xOffset + index * tempWidth] = 5;
		mapTemplate[(xOffset + tempWidth - 1) + index * arrWidth] = 5;
	}
	// Draw the walls around the inside of the hull
	xOffset++;
	yOffset++;
	tempWidth--;
	tempHeight--;
	for (index = xOffset; index < (xOffset + tempWidth - 1); index++) {
		mapTemplate[index + yOffset * arrWidth] = 4;
		mapTemplate[index + (yOffset + tempHeight - 2) * arrWidth] = 4;
	}
	for (index = yOffset; index < (yOffset + tempHeight - 1); index++) {
		mapTemplate[xOffset + index * arrWidth] = 4;
		mapTemplate[(xOffset + tempWidth - 2) + index * arrWidth] = 4;
	}
	// Fill in the remaining space with flooring
	xOffset++;
	yOffset++;
	tempWidth--;
	tempHeight--;
	for (uint foo = xOffset ; foo < tempWidth; foo++) {
		for (uint bar = yOffset ; bar < tempHeight; bar++) {
			mapTemplate[foo + bar * arrWidth] = 3;
		}
	}
	// Fill in some internal walls
	for (index = 2; index < 10; index++) {
		if (index != 8) {
			mapTemplate[11 + index * arrWidth] = 4;
		}
	}
	for (index = 2; index < 10; index++) {
		if (index == 5) {
			mapTemplate[22 + index * arrWidth] = 2;
			mapTemplate[23 + index * arrWidth] = 2;
			mapTemplate[24 + index * arrWidth] = 2;
		} else {
			mapTemplate[22 + index * arrWidth] = 4;
			mapTemplate[23 + index * arrWidth] = 4;
			mapTemplate[24 + index * arrWidth] = 4;
		}
	}
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
//					LOGMSG("Placing " << tileType << " at " << xOffset << ", " << yOffset);
					switch(tileType) {
						case 1:
							mapArray[xOffset + yOffset * width] = new Vacuum();
							break;
						case 2:
							mapArray[xOffset + yOffset * width] = new Tunnel();
							break;
						case 3:
							mapArray[xOffset + yOffset * width] = new Floor();
							break;
						case 4:
							mapArray[xOffset + yOffset * width] = new Wall();
							break;
						case 5:
							mapArray[xOffset + yOffset * width] = new Solid();
							break;
						default:
							mapArray[xOffset + yOffset * width] = new Vacuum();
							break;
					}
				} else {
					mapArray[xOffset + yOffset * width] = new Vacuum();
				}
			} else {
				mapArray[xOffset + yOffset * width] = new Vacuum();
			}
		}
	}
	// Add some furniture
	Door *localDoor = new Door(12, 9, this);
	registerFurniture(localDoor, 12, 9);
	// *** END TEST STRUCTURE
//	LOGMSG("Tilemap created: " << width << "x" << height);
	/*
	clog << "TEMPLATE DUMP:" << endl;
	xOffset = 1;
	yOffset = 1;
	for (uint foo = 0; foo < arrHeight; foo++) {
		for (uint bar = 0; bar < arrWidth; bar++) {
			clog << mapTemplate[bar + foo * arrWidth];
		}
		clog << endl;
	}
	*/
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
Actor* GameMap::getOccupant(uint xPos, uint yPos) {
	return this->mapArray[xPos + yPos * width]->occupant;
}
Actor* GameMap::getOccupant(cpair inputLocation) {
	return getOccupant(inputLocation.x, inputLocation.y);
}
Actor* GameMap::getContents(uint xPos, uint yPos) {
	if (this->mapArray[xPos + yPos * width]->contents == nullptr) return nullptr;
	return this->mapArray[xPos + yPos * width]->contents->itemList.front();
}
Actor* GameMap::getContents(cpair inputLocation) {
	return getContents(inputLocation.x, inputLocation.y);
}
Actor* GameMap::getFurnitureAt(uint xPos, uint yPos) {
	return this->mapArray[xPos + yPos * width]->furniture;
}
Actor* GameMap::getFurnitureAt(cpair inputLocation) {
	return getFurnitureAt(inputLocation.x, inputLocation.y);
}
void GameMap::setOccupant(Actor *occupier) {
	this->mapArray[occupier->getLocation().x + occupier->getLocation().y * width]->occupant = occupier;
	if (occupier->obstructs) this->setObstruction(occupier->getLocation(), occupier->obstructs);
//	LOGMSG("Occupier set at " << occupier->getLocation() << " to " << occupier->getName() << ":" << this);
}
void GameMap::unsetOccupant(uint xPos, uint yPos) {
	this->mapArray[xPos + yPos * width]->occupant = nullptr;
	this->setObstruction(xPos, yPos, false);
}
void GameMap::unsetOccupant(cpair inputLocation) {
	this->unsetOccupant(inputLocation.x, inputLocation.y);
}
void GameMap::setObstruction(uint xPos, uint yPos, bool inputValue) {
//	LOGMSG("Setting obstruction at " << xPos << ", " << yPos << " to " << inputValue);
	this->mapArray[xPos + yPos * width]->obstructs = inputValue;
}
void GameMap::setObstruction(cpair inputLocation, bool inputValue) {
	this->setObstruction(inputLocation.x, inputLocation.y, inputValue);
}
void GameMap::registerFurniture(Actor* newItem, uint xPos, uint yPos) {
	this->mapArray[xPos + yPos * width]->furniture = newItem;
	newItem->setLocality(this);
	newItem->setAbsLocation(xPos, yPos);
	setObstruction(xPos, yPos, newItem->obstructs);
	this->furnishings.push_back(newItem);
//	LOGMSG(newItem->getName() << " (" << newItem << ") added to furniture of Tile at " << xPos << ", " << yPos);
}
void GameMap::registerItem(Actor* newItem, uint xPos, uint yPos) {
	this->mapArray[xPos + yPos * width]->receiveItem(newItem);
	newItem->setLocality(this);
	newItem->setAbsLocation(xPos, yPos);
	// FIXME: Change/move the line below, so that the GUI method checks a tile
	// for any contents and if so, draws the topmost item as the pile
	this->allActors.push_back(newItem);
//	LOGMSG(newItem->getName() << " (" << newItem << ") added to contents of Tile at " << xPos << ", " << yPos);
}
void GameMap::registerItem(Actor* newItem, cpair inputLocation) {
	this->registerItem(newItem, inputLocation.x, inputLocation.y);
}
void GameMap::removeItem(Actor *target) {
	mapArray[target->location.x + target->location.y * width]->releaseItem(target);
	target->setLocality(nullptr);
	allActors.remove(target);
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

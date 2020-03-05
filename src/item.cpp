/*   item.cpp
AUTH xazak
DATE Feb 27 2020
DESC Describes the GameItem class, which inherits from the Actor class to
	 provide a generic prototype for any non-sentient prop or device
*/

#include "map.hpp"
#include "item.hpp"
#include "portable.hpp"
#include "container.hpp"
#include "openable.hpp"
#include <string>

using namespace std;

Item::Item(string inputName, int inputSigil, int inputColor, int xPos, int yPos, GameMap* inputMap, bool obstructFlag, bool occludesFlag) :
Actor(inputName, inputSigil, inputColor, xPos, yPos, inputMap, obstructFlag, occludesFlag)
{	}
Item::Item(string inputName, int inputSigil, int inputColor, cpair inputLocation, GameMap* inputMap, bool obstructFlag, bool occludesFlag) :
Actor(inputName, inputSigil, inputColor, inputLocation, inputMap, obstructFlag, occludesFlag)
{	}

// **** SPECIFIC ITEMS
// **** Large Items
Door::Door(int xPos, int yPos, GameMap* inputMap) :
	Item("door", 0x03A6, 0xFF666699, xPos, yPos, inputMap, true, true),
	isOpen(false),
	closedSigil(0x03A6),
	openSigil(0x00D4)
{	this->aperture = new Openable(openSigil, closedSigil);
	this->aperture->close(*this);

}
/*
int Door::getSigil() {
	// Returns the correct sigil value based on its internal state
	return (this->aperture->isOpen() ? openSigil : closedSigil);
}
*/
// **** Small Items
Wrench::Wrench(int xPos, int yPos, GameMap* inputMap) :
Item("wrench", 0x00A5, 0xFF666699, xPos, yPos, inputMap, false, true)
{	this->portable = new Portable();
	this->aperture = new Openable();

}

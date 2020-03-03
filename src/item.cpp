/*   item.cpp
AUTH xazak
DATE Feb 27 2020
DESC Describes the GameItem class, which inherits from the Actor class to
	 provide a generic prototype for any non-sentient prop or device
*/

#include "item.hpp"
#include "portable.hpp"
#include "container.hpp"
#include "openable.hpp"
#include <string>

using namespace std;

Item::Item(string inputName, int inputSigil, int inputColor, int xPos, int yPos, bool obstructFlag, bool occludesFlag) :
Actor(inputName, inputSigil, inputColor, xPos, yPos, obstructFlag, occludesFlag)
{	}
Item::Item(string inputName, int inputSigil, int inputColor, cpair inputLocation, bool obstructFlag, bool occludesFlag) :
Actor(inputName, inputSigil, inputColor, inputLocation, obstructFlag, occludesFlag)
{	}

// **** SPECIFIC ITEMS
// **** Large Items
Door::Door(int xPos, int yPos) :
	Item("door", 0x03A6, 0xFF666699, xPos, yPos, true, true),
	isOpen(false),
	closedSigil(0x03AD),
	openSigil(0x00D4)
{	this->aperture = new Openable();

}
/*
int Door::getSigil() {
	// Returns the correct sigil value based on its internal state
	return (this->aperture->isOpen() ? openSigil : closedSigil);
}
*/
// **** Small Items
Wrench::Wrench(int xPos, int yPos) :
Item("wrench", 0x00A5, 0xFF666699, xPos, yPos, false, true)
{	this->portable = new Portable();

}

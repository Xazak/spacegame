/*   item.cpp
AUTH xazak
DATE Feb 27 2020
DESC Describes the GameItem class, which inherits from the Actor class to
	 provide a generic prototype for any non-sentient prop or device
*/

#include "item.hpp"
#include <string>

using namespace std;

Item::Item(string inputName, int inputSigil, int inputColor, int xPos, int yPos, bool obstructFlag, bool visibleFlag) :
Actor(inputName, inputSigil, inputColor, xPos, yPos, obstructFlag, visibleFlag)
{	}
Item::Item(string inputName, int inputSigil, int inputColor, cpair inputLocation, bool obstructFlag, bool visibleFlag) :
Actor(inputName, inputSigil, inputColor, inputLocation, obstructFlag, visibleFlag)
{	}

// **** SPECIFIC ITEMS
Wrench::Wrench(int xPos, int yPos) :
Item("wrench", 0x00A5, 0xFF666699, xPos, yPos, false, true)
{	//this->portable = new Portable();

}

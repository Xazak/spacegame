/*   actor.cpp
AUTH xazak
DATE Jan 29 2020
DESC Contains the definitions of the Actor class, which handles the player
	object as well as any NPCs and any usable items and objects
*/

#include "actor.hpp"
#include "map.hpp"
#include "sentience.hpp"
#include "container.hpp"

using namespace std;

// *** ACTOR METHODS

// The default ctor
Actor::Actor() :
obstructs(true),
occludes(true),
location(0, 0),
locality(nullptr),
intent(nullptr),
contents(nullptr),
portable(nullptr),
name("n0b0dY"),
sigil(0x2112),
color(0xFF345678)
{	}
// The specific ctor
Actor::Actor(bool obstructFlag, bool occludesFlag, string inputName, cpair inputPosition, GameMap* inputArea, int inputSigil, int inputColor) :
obstructs(obstructFlag),
occludes(occludesFlag),
location(inputPosition),
locality(inputArea),
intent(nullptr),
contents(nullptr),
portable(nullptr),
name(inputName),
sigil(inputSigil),
color(inputColor)
{	}
// A shorthand ctor: name, sigil, color, x = 0, y = 0
Actor::Actor(string inputName, int inputSigil, int inputColor, int xPos, int yPos, GameMap* inputArea, bool obstructFlag, bool occludesFlag) :
obstructs(obstructFlag),
occludes(occludesFlag),
location(xPos, yPos),
locality(inputArea),
intent(nullptr),
contents(nullptr),
portable(nullptr),
name(inputName),
sigil(inputSigil),
color(inputColor)
{	}
Actor::Actor(string inputName, int inputSigil, int inputColor, cpair inputLocation, GameMap* inputArea, bool obstructFlag, bool occludesFlag) :
obstructs(obstructFlag),
occludes(occludesFlag),
location(inputLocation),
locality(inputArea),
intent(nullptr),
contents(nullptr),
portable(nullptr),
name(inputName),
sigil(inputSigil),
color(inputColor)
{	}
// Need some cpair overrides for these two fxns
string Actor::getLocString() {
	string actorLocString;
	actorLocString.push_back((char)this->location.x);
	actorLocString.append(", ");
	actorLocString.push_back((char)this->location.y);
	return actorLocString;
}
void Actor::setAbsLocation(int newXPos, int newYPos) {
	// Sets the actor's location to the exact specified coordinates
	this->locality->unsetOccupant(this->location.x, this->location.y);
	this->location.x = newXPos;
	this->location.y = newYPos;
	this->locality->setOccupant(this);
}
void Actor::setAbsLocation(cpair inputPosition) {
	this->setAbsLocation(inputPosition.x, inputPosition.y);
}
void Actor::setRelLocation(int xOffset, int yOffset) {
	// Sets the actor's position by a relative offset
	this->location.x += xOffset;
	this->location.y += yOffset;
}
void Actor::setRelLocation(cpair inputPosition) {
	this->setRelLocation(inputPosition.x, inputPosition.y);
}
void Actor::setLocality(GameMap *inputArea) {
	// FIXME: does NOT remove actors from localities!
	this->locality = inputArea;
//	LOGMSG(this->getName() << "(" << this << ") locality set to " << locality);
}

// *** PLAYER METHODS
Player::Player() :
Actor("Jenaryk", 0x2135, 0xFF996600) {
	intent = new PlayerSentience();
	contents = new Container(2);
//	LOGMSG("Player object initialized.");
}
void Player::update() {
	// Examines the player's action queue and does the most recent thing
//	LOGMSG("Checking for updates on " << this->getName());
	if (this->intent->hasActions()) this->intent->doNextAction();
}

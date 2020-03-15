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
obstructs(true), occludes(true), location(0, 0), locality(nullptr),
intent(nullptr), contents(nullptr), portable(nullptr),
name("n0b0dY"), sigil(0x2112), color(0xFF345678)
{	}
// The specific ctor
Actor::Actor(bool obstructFlag, bool occludesFlag, string inputName, cpair inputPosition, GameMap* inputArea, int inputSigil, int inputColor) :
obstructs(obstructFlag), occludes(occludesFlag), location(inputPosition), locality(inputArea),
intent(nullptr), contents(nullptr), portable(nullptr),
name(inputName), sigil(inputSigil), color(inputColor)
{	}
// A shorthand ctor: name, sigil, color, x = 0, y = 0
Actor::Actor(string inputName, int inputSigil, int inputColor, int xPos, int yPos, GameMap* inputArea, bool obstructFlag, bool occludesFlag) :
obstructs(obstructFlag), occludes(occludesFlag), location(xPos, yPos), locality(inputArea),
intent(nullptr), contents(nullptr), portable(nullptr),
name(inputName), sigil(inputSigil), color(inputColor)
{	}
// A shorthand ctor: name, sigil, color, location
Actor::Actor(string inputName, int inputSigil, int inputColor, cpair inputLocation, GameMap* inputArea, bool obstructFlag, bool occludesFlag) :
obstructs(obstructFlag), occludes(occludesFlag), location(inputLocation), locality(inputArea),
intent(nullptr), contents(nullptr), portable(nullptr),
name(inputName), sigil(inputSigil), color(inputColor)
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
	this->location.x = newXPos;
	this->location.y = newYPos;
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
void Actor::moveByAbs(int inputX, int inputY) {
	// move and set occupancy
	this->locality->unsetOccupant(this->location.x, this->location.y);
	this->location.x = inputX;
	this->location.y = inputY;
	this->locality->setOccupant(this);
}
void Actor::moveByAbs(cpair inputPosition) {
	this->moveByAbs(inputPosition.x, inputPosition.y);
}
void Actor::moveByRel(int xOffset, int yOffset) {
	// move and set occupancy
	this->locality->unsetOccupant(this->location.x, this->location.y);
	this->location.x += xOffset;
	this->location.y += yOffset;
	this->locality->setOccupant(this);
}
void Actor::moveByRel(cpair inputOffset) {
	this->moveByRel(inputOffset.x, inputOffset.y);
}

// *** PLAYER METHODS
Player::Player() :
	Actor("Jenaryk", 0x263A, 0xFF996600)
{	intent = new PlayerSentience();
	contents = new Container(2);
	obstructs = true;
//	LOGMSG("Player object initialized.");
}
void Player::update() {
	// Examines the player's action queue and does the most recent thing
//	LOGMSG("Checking for updates on " << this->getName());
	if (this->intent->hasActions()) this->intent->doNextAction();
}

Drone::Drone() :
	Actor("DRONE", 0x03B6, 0xFF666699)
{	intent = new DroneSentience(this);
	contents = new Container(1);
	obstructs = true;
//	LOGMSG("Drone object initialized.");
}
void Drone::update() {
	// Asks the sentience module to do the next action
	// (Sentience decides what to do next and then does it)
	if (this->intent->isFocused()) {
		// continue the in-progress action
//		LOGMSG(this->getName() << " continuing its action");
		this->intent->continueWorking();
	} else {
//		LOGMSG(this->getName() << " choosing new action");
		this->intent->doNextAction();
	}
}

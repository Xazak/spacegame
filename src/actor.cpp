/*   actor.cpp
AUTH xazak
DATE Jan 29 2020
DESC Contains the definitions of the Actor class, which handles the player
	object as well as any NPCs and any usable items and objects
*/

#include "actor.hpp"
#include "sentience.hpp"
#include "container.hpp"

using namespace std;

// *** ACTOR METHODS

// The default ctor
Actor::Actor() :
obstructs(true),
visible(true),
location(0, 0),
locality(nullptr),
intent(nullptr),
container(nullptr),
portable(nullptr),
name("n0b0dY"),
sigil(0x2112),
color("white")
{	}
// The specific ctor
Actor::Actor(bool obstructFlag, bool visibleFlag, string inputName, cpair inputPosition, GameMap* inputArea, int inputSigil, string inputColor) :
obstructs(obstructFlag),
visible(visibleFlag),
location(inputPosition),
locality(inputArea),
intent(nullptr),
container(nullptr),
portable(nullptr),
name(inputName),
sigil(inputSigil),
color(inputColor)
{	}
// A shorthand ctor: name, sigil, color, x = 0, y = 0
Actor::Actor(string inputName, int inputSigil, string inputColor, int xPos, int yPos) :
obstructs(true),
visible(true),
location(xPos, yPos),
locality(nullptr),
intent(nullptr),
container(nullptr),
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
	this->locality = inputArea;
//	LOGMSG(this->getName() <<" locality set to " << locality);
}

// *** PLAYER METHODS
Player::Player() :
Actor("Jenaryk", 0x2135, "light green") {
	intent = new PlayerSentience();
	container = new Container(2);
//	LOGMSG("Player object initialized.");
}
void Player::update() {
	// Examines the player's action queue and does the most recent thing
//	LOGMSG("Checking for updates on " << this->getName());
	if (this->intent->hasActions()) this->intent->doNextAction();
}

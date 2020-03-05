/*   openable.cpp
AUTH xazak
DATE Mar 02 2020
DESC Describes the Openable class, which allows Actors to possess doors, lids,
	 and other such coverings that must be OPENed or CLOSEd.
*/

#include "map.hpp"
#include "openable.hpp"
#include "actor.hpp"

Openable::Openable() :
	openSigil(0),
	shutSigil(0),
	apertureState(false)
{	}
Openable::Openable(int sigilOne, int sigilTwo) :
	openSigil(sigilOne),
	shutSigil(sigilTwo),
	apertureState(false)
{	}
bool Openable::open(Actor &parent) {
	// Returns TRUE if it was successful
	this->apertureState = true;
	parent.setSigil(openSigil);
	parent.locality->setObstruction(parent.location, false);
	return apertureState;
}
bool Openable::close(Actor &parent) {
	// Returns TRUE if it was successful
	this->apertureState = false;
	parent.setSigil(shutSigil);
	parent.locality->setObstruction(parent.location, true);
	return (!apertureState);
}
void Openable::toggle(Actor &parent) {
	if (this->apertureState == true) this->close(parent);
	else this->open(parent);
}

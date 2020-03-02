/*   openable.cpp
AUTH xazak
DATE Mar 02 2020
DESC Describes the Openable class, which allows Actors to possess doors, lids,
	 and other such coverings that must be OPENed or CLOSEd.
*/

#include "openable.hpp"

bool Openable::open() {
	// Returns TRUE if it was successful
	this->apertureState = true;
	return apertureState;
}
bool Openable::close() {
	// Returns TRUE if it was successful
	this->apertureState = false;
	return (!apertureState);
}
void Openable::toggle() {
	if (this->apertureState == true) this->close();
	else this->open();
}

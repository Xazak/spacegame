/*   portable.cpp
AUTH xazak
DATE Feb 26 2020
DESC Describes the Portable class property: when attached to an Actor, allows
	 it to be picked up and moved by some other Actor
*/

#include "portable.hpp"
#include "container.hpp"
#include "actor.hpp"
#include "map.hpp"

bool Portable::take(Actor *subject, Actor *object) {
	// Does the subject HAVE A container and does that container HAVE room?
	if (subject->contents && subject->contents->hasRoom()) {
		// FIXME: Remove the object from the global actor list
		subject->contents->add(object);
//		object->locality->removeFromContents(object->location);
		object->locality->removeItem(object);
		return true;
	}
	return false;
}
void Portable::drop(Actor *subject, Actor *object) {
	// Drop the object at the subject's feet
	if (subject->contents) {
		subject->contents->remove(object);
		subject->locality->addItem(object, subject->location);
		// FIXME: Add the object to the global actor list
//		object->setAbsLocation(subject->getLocation());
		// FIXME: should the result msg be created here?
	}
}

/*   portable.cpp
AUTH xazak
DATE Feb 26 2020
DESC Describes the Portable class property: when attached to an Actor, allows
	 it to be picked up and moved by some other Actor
*/

#include "portable.hpp"
#include "container.hpp"
#include "actor.hpp"

bool Portable::take(Actor *subject, Actor *object) {
	if (subject->container && subject->container->add(object)) {
		// FIXME: Remove the object from the global actor list
		return true;
	}
	return false;
}
void Portable::drop(Actor *subject, Actor *object) {
	if (subject->container) {
		subject->container->remove(object);
		// FIXME: Add the object to the global actor list
		object->setAbsLocation(subject->getLocation());
		// FIXME: should the result msg be created here?
	}
}

/*   container.cpp
AUTH xazak
DATE Feb 26 2020
DESC Describes the Container class: when attached to a valid Actor, allows the
	 Actor to pick up and hold a collection of (Portable) objects
*/

#include "container.hpp"

Container::Container() : capacity(0) {	}
Container::Container(uint inputSize) : capacity(inputSize) {	}
Container::~Container() {
	// FIXME: Be sure to delete everything in this->contents!

}
bool Container::add(Actor *object) {
	if (this->isFull()) return false;
	contents.push_front(object);
	return true;
}
void Container::remove(Actor *object) {
	// iterate across the contents of the actor to find a match
	// if not found, fail silently

}
void Container::setCapacity(uint inputSize) {
	// Throws an error if the inventory is not empty
	if (contents.size() > 0) {
		ERRMSG("The capacity of a non-empty container cannot be changed.");
	} else {
		this->capacity = inputSize;
	}
}
bool Container::hasRoom() {
	return (this->capacity == 0 || this->capacity > contents.size());
}
bool Container::isFull() {
	return (!this->hasRoom());
}

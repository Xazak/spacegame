/*   container.cpp
AUTH xazak
DATE Feb 26 2020
DESC Describes the Container class: when attached to a valid Actor, allows the
	 Actor to pick up and hold a collection of (Portable) objects
*/

#include "container.hpp"
#include "actor.hpp"
#include <list>

using namespace std;

Container::Container() : capacity(0) {	/*LOGMSG("Created new unlim. container");*/ }
Container::Container(uint inputSize) : capacity(inputSize) {	}
Container::~Container() {
	// FIXME: Be sure to delete everything in this->itemList!
//	LOGMSG(" called ");
}
bool Container::add(Actor *object) {
	if (this->isFull()) return false;
	itemList.push_front(object);
	return true;
}
void Container::remove(Actor *object) {
	// iterate across the itemList of the actor to find a match
	// if not found, fail silently
	// check for emptiness? or just fail?
	itemList.remove(object);
}
void Container::setCapacity(uint inputSize) {
	// Throws an error if the inventory is not empty
	if (itemList.size() > 0) {
		ERRMSG("The capacity of a non-empty container cannot be changed.");
	} else {
		this->capacity = inputSize;
	}
}
bool Container::hasRoom() {
	return (this->capacity == 0 || this->capacity > itemList.size());
}
bool Container::isEmpty() {
	return (itemList.size() == 0);
}
bool Container::isFull() {
	return (!this->hasRoom());
}
void Container::dump() {
	// Prints the list itemList to stdlog
	LOGMSG("Dumping itemList of " << this);
	if (itemList.empty()) {
		clog << "- empty -" << endl;
	} else {
		for (list<Actor*>::iterator listIter = itemList.begin();
			listIter != itemList.end(); listIter++) {
			clog << "    " << *listIter << " " << (*listIter)->getName() << endl;
		}
	}
}

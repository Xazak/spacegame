/*   container.hpp
AUTH xazak
DATE Feb 26 2020
DESC Describes the Container class: when attached to a valid Actor, allows the
	 Actor to pick up and hold a collection of (Portable) objects
*/

#ifndef SPACEGAME_ACTOR_CONTAINER
#define SPACEGAME_ACTOR_CONTAINER

#include "main.hpp"
#include <list>

class Actor; // fwd-decl
class Container {
	public:
		Container(); // defaults to 0 (unlimited)
		Container(uint inputSize);
		~Container();
		bool add(Actor *object);
		void remove(Actor *object);
		void setCapacity(uint inputSize);
		uint getCapacity() { return capacity; }
		uint getSize() { return itemList.size(); }
		bool hasRoom();
		bool isEmpty();
		bool isFull();
		void dump();
		uint capacity;
		std::list<Actor*> itemList;
};

#endif // SPACEGAME_ACTOR_CONTAINER

/*   portable.hpp
AUTH xazak
DATE Feb 26 2020
DESC Describes the Portable class property: when attached to an Actor, allows
	 it to be picked up and moved by some other Actor
*/

#ifndef SPACEGAME_ACTOR_PORTABLE
#define SPACEGAME_ACTOR_PORTABLE

#include "actor.hpp"

class Portable {
	public:
		bool take(Actor *subject, Actor *object);
		void drop(Actor *subject, Actor *object);
};
#endif // SPACEGAME_ACTOR_PORTABLE

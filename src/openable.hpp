/*   openable.hpp
AUTH xazak
DATE Mar 02 2020
DESC Describes the Openable class, which allows Actors to possess doors, lids,
	 and other such coverings that must be OPENed or CLOSEd.
*/

#ifndef SPACEGAME_ACTOR_OPENABLE
#define SPACEGAME_ACTOR_OPENABLE

class Actor; //fwd-decl

class Openable {
	public:
		bool open();
		bool close();
		void toggle();
		bool isOpen() { return this->apertureState; }
	
	private:
		bool apertureState; // TRUE if open, FALSE if closed
};

#endif // SPACEGAME_ACTOR_OPENABLE

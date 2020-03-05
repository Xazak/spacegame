/*   item.hpp
AUTH xazak
DATE Feb 27 2020
DESC Describes the GameItem class, which inherits from the Actor class to
	 provide a generic prototype for any non-sentient prop or device
*/

#ifndef SPACEGAME_ITEM
#define SPACEGAME_ITEM

#include "actor.hpp"
#include "main.hpp"
class GameMap;

class Item : public Actor {
	// As inheritors of Actor, all Items may implement:
	//	virtual ~Actor() {}
	//	virtual void update() {}
	//	virtual int getSigil() { return this->sigil; }
	// Available modules:
	//	Sentience*	intent; 	// Goal-seeking and action-taking
	//	Container*	contents;	// Allows an Actor to contain objects
	//	Portable*	portable;	// Allows an Actor to be picked up
	//	Openable*	aperture;	// Allows an Actor to be opened/closed
	public:
		Item();
		Item(std::string inputName, int inputSigil, int inputColor,
			int xPos, int yPos, GameMap* inputMap,
			bool obstructFlag = false,
			bool occludesFlag = true);
		Item(std::string inputName, int inputSigil, int inputColor,
			cpair inputLocation, GameMap* inputMap,
			bool obstructFlag = false,
			bool occludesFlag = true);
//		~Item();
		// needs a name, sigil, color, position, [obstructs], [occludes]
		// ASSUME occludes (inoccludes objects are probably very special)
		// Will need to set up the Container, Portable, etc modules according
		// to its particular setup
	private:
		
};

// **** SPECIFIC ITEMS AND TYPES
// **** Large Items (NEVER portable, USUALLY obstructs)
struct Door : public Item {
	Door(int xPos, int yPos, GameMap* inputMap);
	int getSigil();
//	void update();
	bool isOpen;
	int closedSigil;
	int openSigil;
};

// **** Small Items (ALWAYS portable, NEVER obstructs)
struct Wrench : public Item {
	Wrench(int xPos, int yPos, GameMap* inputMap);
};

#endif // SPACEGAME_ITEM

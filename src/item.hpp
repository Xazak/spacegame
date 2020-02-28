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

class Item : public Actor {
	public:
		Item();
		Item(std::string inputName, int inputSigil, int inputColor,
			int xPos, int yPos, bool obstructFlag = false,
			bool visibleFlag = true);
		Item(std::string inputName, int inputSigil, int inputColor,
			cpair inputLocation, bool obstructFlag = false,
			bool visibleFlag = true);
//		~Item();
		// needs a name, sigil, color, position, [obstructs], [visible]
		// ASSUME visible (invisible objects are probably very special)
		// Will need to set up the Container, Portable, etc modules according
		// to its particular setup
	private:
		
};

// **** SPECIFIC ITEMS AND TYPES
struct Wrench : public Item {
	Wrench(int xPos, int yPos);
};

#endif // SPACEGAME_ITEM

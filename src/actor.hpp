/*   actor.hpp
AUTH xazak
DATE Jan 29 2020
DESC Contains the definitions of the Actor class, which handles the player
	object as well as any NPCs and any usable items and objects
*/

#ifndef SPACEGAME_ACTOR
#define SPACEGAME_ACTOR

#include "sentience.hpp"
#include "main.hpp"
#include <string>

class Container; // fwd-decl
class Actor {
	// Defines the Actor prototype, which (should not) be used on its own, but
	// as a base class from which characters and items can be derived
	public:
	//  METHODS
		// Default ctor
		Actor();
		// Specific ctor
		Actor(bool obstructFlag, bool visibleFlag, std::string inputName,
			cpair inputPosition, GameMap* inputArea, int inputSigil,
			std::string inputColor);
		// Shorthand ctors
		// name, sigil, color, x=0, y=0
		Actor(std::string inputName, int inputSigil, std::string inputColor,
			int xPos = 0, int yPos = 0);
		virtual ~Actor() {}
		virtual void update() {}
		// GETS
		std::string	getName() { return this->name; }
		cpair		getLocation() { return this->location; }
		std::string	getLocString();
		GameMap*	getLocality() { return this->locality; }
		int			getSigil() { return this->sigil; }
		std::string	getColor() { return this->color; }
		// SETS
		//setName
		void		setAbsLocation(int newXPos, int newYPos);
		void		setRelLocation(int xOffset, int yOffset);
		void		setLocality(GameMap *inputArea);
		//setSigil
		//setColor
	//  PROPERTIES - Guaranteed
		bool		obstructs;
		bool		visible;
		cpair		location;	// (x, y) coordinates on map
		GameMap*	locality;
	//  PROPERTIES - Optional
		Sentience*	intent; 	// Goal-seeking and action-taking
		Container*	contents;	// Allows an Actor to hold objects
		Portable*	portable;	// Allows an Actor to be picked up
		/*
		Violence*	// Damage-causing methods
		Consumable* // Allows an actor to be consumed by another
		Mortality*	// Allows an actor to be damaged and killed
		Vitality*	// Provides access to core stats and skill checks
		? device	// Allows access to an Actor's 'internal mechanisms'
		? barrier	// Allows an Actor to change its obstruction?
		*/

	private:
		std::string	name;		// Contains actor's (visible) name
		int			sigil;		// Contains Unicode codepoint of display character
		std::string	color;		// Contains the color name of the actor sigil
};
class Player : public Actor {
	// Defines the Player object
	public:
		Player();
		Player(std::string inputName, int inputSigil, std::string inputColor);
//		~Player();
		void update();

	private:
		
};

#endif //SPACEGAME_ACTOR

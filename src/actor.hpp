/*   actor.hpp
AUTH xazak
DATE Jan 29 2020
DESC Contains the definitions of the Actor class, which handles the player
	object as well as any NPCs and any usable items and objects
*/

#ifndef SPACEGAME_ACTOR
#define SPACEGAME_ACTOR

#include "main.hpp"
#include <string>

// **** FORWARD DECLARES
class Sentience;
class Container;
class Portable;
class Openable;
class GameMap;

class Actor {
	// Defines the Actor prototype, which (should not) be used on its own, but
	// as a base class from which characters and items can be derived
	public:
	//  METHODS
		// Default ctor
		Actor();
		// Specific ctor
		Actor(bool obstructFlag, bool occludesFlag, std::string inputName,
			cpair inputPosition, GameMap* inputArea, int inputSigil,
			int inputColor);
		// Shorthand ctors
		// name, sigil, color, x=0, y=0
		Actor(std::string inputName, int inputSigil, int inputColor,
			int xPos = 0, int yPos = 0, GameMap* inputArea = nullptr,
			bool obstructFlag = false, bool occludesFlag = true);
		Actor(std::string inputName, int inputSigil, int inputColor,
			cpair inputLocation, GameMap* inputArea,
			bool obstructFlag = false, bool occludesFlag = true);
		// ***
		virtual ~Actor() {}
		virtual void update() {}
		// GETS
		std::string	getName() { return this->name; }
		cpair		getLocation() { return this->location; }
		std::string	getLocString();
		GameMap*	getLocality() { return this->locality; }
//		virtual int	getSigil() { return this->sigil; }
		int			getSigil() { return this->sigil; }
		int			getColor() { return this->color; }
		// SETS
		//setName
		void		setAbsLocation(int newXPos, int newYPos);
		void		setAbsLocation(cpair inputPosition);
		void		setRelLocation(int xOffset, int yOffset);
		void		setRelLocation(cpair inputPosition);
		void		setLocality(GameMap *inputArea);
		void		setSigil(int newSigil) { this->sigil = newSigil; }
		//setSigil
		//setColor
	//  PROPERTIES - Guaranteed
		bool		obstructs;
		bool		occludes;
		cpair		location;	// (x, y) coordinates on map
		GameMap*	locality;
	//  PROPERTIES - Optional
		Sentience*	intent; 	// Goal-seeking and action-taking
		Container*	contents;	// Allows an Actor to hold objects
		Portable*	portable;	// Allows an Actor to be picked up
		Openable*	aperture;	// Allows an Actor to be opened/closed
		/*
		Violence*	// Damage-causing methods
		Consumable* // Allows an actor to be consumed by another
		Mortality*	// Allows an actor to be damaged and killed
		Vitality*	// Provides access to core stats and skill checks
		? device	// Allows access to an Actor's 'internal mechanisms'
		? barrier	// Allows an Actor to change its obstruction?
		*/

	private:
		std::string	name;		// Contains actor's (occludes) name
		int			sigil;		// Contains Unicode codepoint of display character
		int			color;		// Contains the color name of the actor sigil
};

class Player : public Actor {
	// Defines the Player object
	public:
		Player();
//		Player(std::string inputName, int inputSigil, std::string inputColor);
//		~Player();
		void update();

	private:
		
};
class Drone : public Actor {
	// Defines the Drone object, which is a prototype for the LMR
	public:
		Drone();
//		Drone(std::string inputName, int inputSigil, std::string inputColor);
//		~Drone();
		void update();
	private:
		// pointer to AURITA?
};

#endif //SPACEGAME_ACTOR

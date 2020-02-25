/*   action.cpp
AUTH xazak
DATE Feb 13 2020
DESC Describes the Action class, which represents the set of possible 'verbs'
	 available to the Actors who possess a working Sentience module.
*/

#ifndef SPACEGAME_ACTIONS
#define SPACEGAME_ACTIONS

#include "main.hpp"

enum class ActionType : uint {
	IDLE =0,// 00: parser is awaiting input ("null" action)
	WAIT,	// 01: to do nothing
	MOVE,	// 02: to move by walking
	JUMP,	// 03: to move by jumping multiple tiles
	GET,	// 04: to pick up something
	DROP,	// 05: to remove something from inventory
	CONSUME,// 06: to ingest something for its caloric value
	OPEN,	// 07: to open a container or door
	CLOSE,	// 08: to close a container or door
	TOGGLE, // 09: to change a device's setting to its other position
	OPERATE,// 10: to open a device's interface
	PUSH,	// 11: to change something's position
	FARLOOK,// 12: to examine something distance
	EXAMINE,// 13: to examine something in-hand
	WEAR,	// 14: to don something wearable
	REMOVE,	// 15: to remove something that is already worn
	INVENTORY //16: to list the contents of an actor's inventory
};
class Actor; // fwd-decl
class GameMap; // fwd-decl
struct ActionContext; // fwd-decl
struct Action {
	Action();
	Action(const Action& inputAction);
	virtual ~Action() {}
	virtual bool isPlausible() { return true; } // Allow any action by default
	virtual void execute() = 0;
	virtual bool result() { return true; } // All actions succeed by default
	virtual Action* clone() const = 0;
	ActionContext* context;
};
// *** ACTION CLASSES
struct IdleAction : public Action {
	IdleAction();
	void execute();
	IdleAction* clone() const { return new IdleAction(*this); }
};
struct MoveAction : public Action {
	MoveAction();
	MoveAction(const ActionContext& inputContext);
	MoveAction(Actor *inputTarget, GameMap *inputArea, int targetX, int targetY);
	~MoveAction();
	bool isPlausible();
	void execute();
	MoveAction* clone() const { return new MoveAction(*this); }
	cpair moveIncrement;
};
struct GetAction : public Action {
	GetAction();
	GetAction(Actor *inputTarget, GameMap *inputArea); // take specific object
//	GetAction(GameMap *inputArea, int xPos, int yPos); // take by location
	bool isPlausible();
	void execute();
	GetAction* clone() const { return new GetAction(*this); }
};
struct DropAction : public Action {
	DropAction();
	DropAction(Actor *dropTarget); // drop an object from carried inventory
	bool isPlausible();
	void execute();
	DropAction* clone() const { return new DropAction(*this); }
};
struct OpenAction : public Action {
	OpenAction();
	OpenAction(Actor *openTarget, GameMap *inputArea); // open target
	bool isPlausible();
	void execute();
	OpenAction* clone() const { return new OpenAction(*this); }
};
struct CloseAction : public Action {
	CloseAction();
	CloseAction(Actor *closeTarget, GameMap *inputArea); // open target
	bool isPlausible();
	void execute();
	CloseAction* clone() const { return new CloseAction(*this); }
};

#endif //SPACEGAME_ACTIONS

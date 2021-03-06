/* context.hpp
AUTH xazak
DATE Feb 14 2020
DESC Describes the ActionContext object, which provides details as needed to
	 the Actors on the game map.
*/
#ifndef SPACEGAME_CONTEXT
#define SPACEGAME_CONTEXT

#include "main.hpp"
#include "action.hpp"

class Actor; // fwd decl
class GameMap; // fwd decl
struct ActionContext {
	// Collects and contains the context details for an actor's action
	// All actors obtain one of these with their Sentience instantiation
	// CONTROLS
	ActionContext(); // gotta leave this in until a ctor chain is worked out
	ActionContext(const ActionContext& inputContext);
	ActionContext(ActionType inputType, GameMap *inputArea, Actor *inputTarget, Actor *inputSubject, int inputX = 0, int inputY = 0, int inputZ = 0, bool inputFlag = true);
	ActionContext& operator=(const ActionContext& rhs);
	ActionContext& operator*();
	friend void swap(ActionContext& lhs, ActionContext& rhs);
	void clear(); // Sets ALL components of this back to defaults
	void reset(); // Sets ONLY the x/y/z and successflag to defaults
	void setSuccess(bool newValue = true) { success = newValue; }
	bool isSuccessful() { return success; }
	void setTarget(Actor *inputPtr) { target = inputPtr; }
	void dump(); // Write contents of object to stdlog (DEBUG)

	// CONTENTS
//	Sentience::Action previousAction; // the last action that was successful
	// v This v may be replaced with a queue later on.... ?
//	Sentience::Action currentAction; // the next action to be performed
	ActionType type;
	Actor *target; // the target (or object) of the action
	Actor *subject; // the entity performing the action
	GameMap *vicinity; // the local area where the action will take place
	int echs, whye, zhee; // generic value containers for extra information
	bool success; // = FALSE if an action is attempted and BLOCKED
};

#endif //SPACEGAME_CONTEXT

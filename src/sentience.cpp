/*   sentience.cpp
AUTH xazak
DATE Feb 13 2020
DESC Describes the Sentience module for Actors, which enables access to the
	Action system and includes packages for decision making.
*/

#include "sentience.hpp"
#include "context.hpp"
#include "gui.hpp"
#include "actor.hpp"
#include <string>
#include <stack>

using namespace std;

/* xxx DISABLED
void Sentience::pushAction(Action* nextAction) {
	// Adds an Action to the actor's action queue to be executed later, during
	// the engine's update phase
	// Returns FALSE if there was a problem adding the action
//	actionStack.push(nextAction);
//	Action newAction(nextAction);
	actionStack.emplace(nextAction);
}
void Sentience::doNextAction() {
	// Performs the next action on the stack
	actionStack.top()->execute();
	actionStack.pop();
}
bool Sentience::hasActions() {
	return (!actionStack.empty());
}
void Sentience::pushAction(const ActionContext& input) {
	switch(input.type) {
		case ActionType::MOVE:
		break;
	}
}
Action* Sentience::getAction() {
	return actionStack.top();
}

*/
PlayerSentience::PlayerSentience() {
//	NOTE: This line below occurs too early to obtain a valid ptr!
//	FIXME: Consider moving this line to someplace 'later' so as to prevent the
//	pushAction() from constantly reacquiring the service pointer?
//	this->msgOutput = ServiceLocator::getMsgLog();
//	LOGMSG("Player sentience initialized");
}
void PlayerSentience::pushAction(Action* inputAction) {
	if (this->msgOutput == nullptr) this->msgOutput = ServiceLocator::getMsgLog();
/*	xxx DISABLED
	LOGMSG("Pushing action #" << (uint)inputAction->context->type);
	delete prevAction;
	if (nextAction == nullptr) {
		prevAction = nullptr;
	} else {
		prevAction = nextAction->clone();
	}
	if (inputAction == nullptr) {
		nextAction = nullptr;
	} else {
		nextAction = inputAction->clone();
	}
	actionReady = true;
	xxx */
//	LOGMSG("Testing action of type #" << (uint)inputAction->context->type);
//	inputAction->context->dump();
	if (inputAction->isPlausible()) {
		// Should probably throw some kind of warning message if NOT plausible
//		LOGMSG("Action #" << (uint)inputAction->context->type << " is valid");
		inputAction->execute();
		//reportSuccess(inputAction);
	} else {
//		LOGMSG("Action #" << (uint)inputAction->context->type << " is INVALID");
//		inputAction->context->dump();
		// FIXME: Throw an error message describing the implausibility
		//reportFailure(inputAction);
		msgOutput->add("Your action failed.");
	}
}
void PlayerSentience::doNextAction() {
//	LOGMSG("Performing action of type " << (uint)nextAction->context->type);
	actionReady = false;
	nextAction->execute();
}
DroneSentience::DroneSentience(Actor* owner) {
	localContext.subject = owner;
	localContext.vicinity = owner->getLocality();
}
void DroneSentience::pushAction(Action* inputAction) {
	// add the action to the stack for processing
	// set the flag to notify for more actions
	actionStack.push(inputAction);
}
void DroneSentience::doNextAction() {
	// pull the action on the top of the stack and do it
	// make sure the action is removed when done
	this->consider();
	this->actionStack.top()->execute();
	this->actionStack.pop();
}
void DroneSentience::consider() {
	// Examines current goals and picks actions to achieve it
	Action* newAction = nullptr;
	if (!localContext.subject->location.isEqual(cpair(4, 8))) {
		LOGMSG("moving");
		localContext.echs = 10;
		localContext.whye = 10;
		localContext.target = localContext.subject;
		newAction = new MoveAction(localContext);
	} else {
		LOGMSG("idling");
		newAction = new IdleAction();
	}
	this->pushAction(newAction);
}
Actor* DroneSentience::findPlayer() {
	// Returns nullptr if the player is not w/in five tiles
	int radius = 5;
	
}

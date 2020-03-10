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

// **** CORE SENTIENCE METHODS
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
Sentience::Sentience() :
	remainingTime(0),
	actionReady(false)
{	}
bool Sentience::isFocused() {
	return (remainingTime > 0.0 ? true : false);
}

// **** PLAYER SENTIENCE
PlayerSentience::PlayerSentience() {
//	NOTE: This line below occurs too early to obtain a valid ptr!
//	FIXME: Consider moving this line to someplace 'later' so as to prevent the
//	pushAction() from constantly reacquiring the service pointer?
//	this->msgOutput = ServiceLocator::getMsgLog();
//	LOGMSG("Player sentience initialized");
}
void PlayerSentience::doNextAction() {
//	LOGMSG("Performing action of type " << (uint)nextAction->context->type);
	actionReady = false;
	nextAction->execute();
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
// **** DRONE SENTIENCE
DroneSentience::DroneSentience(Actor* owner) {
	localContext.subject = owner;
	localContext.vicinity = owner->getLocality();
}
bool DroneSentience::hasActions() {
	return (actionStack.empty() ? false : true);
}
void DroneSentience::doNextAction() {
	// pull the action on the top of the stack and do it
	// make sure the action is removed when done
	if (!this->hasActions()) this->consider(); // Consider whether we should change the action
	else this->continueWorking();
}
void DroneSentience::consider() {
	// Examines current goals and picks actions to achieve it
	Action* newAction = nullptr;
	localContext.target = localContext.subject;
	localContext.vicinity = localContext.target->getLocality();
	if (localContext.subject->location.isEqual(cpair(7, 7))) {
//		LOGMSG("going to #2");
		localContext.echs = 7;
		localContext.whye = 8;
		localContext.type = ActionType::MOVE;
		newAction = new MoveAction(localContext);
	} else if (localContext.subject->location.isEqual(cpair(7, 8))) {
//		LOGMSG("going to #1");
		localContext.echs = 7;
		localContext.whye = 7;
		localContext.type = ActionType::MOVE;
		newAction = new MoveAction(localContext);
	} else {
//		LOGMSG("idling");
		newAction = new IdleAction();
	}
//	newAction = new IdleAction();
	this->pushAction(newAction);
}
void DroneSentience::pushAction(Action* inputAction) {
	// add the action to the stack for processing
	// set the flag to notify for more actions
	// NOTE: pretty sure this is where intrinsic speed should be calc'ed in...
	if (inputAction->isPlausible()) {
		actionStack.push(inputAction);
		remainingTime = actionStack.top()->duration;
//		LOGMSG("New action duration: " << remainingTime);
	} else {
		ERRMSG("FIXME: bad action context checking");
	}
}
void DroneSentience::continueWorking() {
	remainingTime -= MS_PER_UPDATE;
	if (remainingTime < 0.0) {
		this->actionStack.top()->execute();
		this->actionStack.pop();
	}
}
Actor* DroneSentience::findPlayer() {
	// Returns nullptr if it can't find the player
	return nullptr;
}

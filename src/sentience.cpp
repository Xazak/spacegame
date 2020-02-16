/*   sentience.cpp
AUTH xazak
DATE Feb 13 2020
DESC Describes the Sentience module for Actors, which enables access to the
	Action system and includes packages for decision making.
*/

#include "sentience.hpp"
#include "context.hpp"

using namespace std;

PlayerSentience::PlayerSentience() {
//	LOGMSG("Player sentience initialized.");
}
void PlayerSentience::pushAction(Action* inputAction) {
/*	LOGMSG("Pushing action #" << (uint)inputAction->context->type);
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
	actionReady = true;*/
	inputAction->execute();
}
void PlayerSentience::doNextAction() {
	LOGMSG("Performing action of type " << (uint)nextAction->context->type);
	actionReady = false;
	nextAction->execute();
}
/*
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

/*   action.cpp
AUTH xazak
DATE Feb 13 2020
DESC Describes the Action class, which represents the set of possible 'verbs'
	 available to the Actors who possess a working Sentience module.
*/

#include "action.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "context.hpp"

using namespace std;

// **** ACTION Prototype
Action::Action() :
context(nullptr)
{	
//	LOGMSG("Action() ctor");
}
Action::Action(const Action& inputAction) :
context(inputAction.context)
{	
//	LOGMSG("Action(Action) ctor");
//	this->context->dump();
}

// **** IDLE Action
void IdleAction::execute() {
	// Do nothing
	LOGMSG("Idling.");
}
// **** MOVE Action
MoveAction::MoveAction() {
	context = new ActionContext();
}
MoveAction::MoveAction(const ActionContext& inputContext) {
	context = new ActionContext(inputContext);
}
MoveAction::MoveAction(Actor* inputTarget, GameMap* inputArea, int targetX, int targetY) {
	context = new ActionContext(ActionType::MOVE, inputTarget, inputArea, targetX, targetY);
}
MoveAction::~MoveAction() {
	delete context;
}
bool MoveAction::isPlausible() {
	// Clamp the target position to the actor's movement speed
	// Actor doesn't have a movement speed yet, assume 1 tile at a time
	// context->target->location : ORIGIN
	// targetN : DESTINATION
	moveIncrement.set(this->context->echs, this->context->whye);
	if (moveIncrement.x > 1) moveIncrement.x = 1;
	else if (moveIncrement.x < -1) moveIncrement.x = -1;
	if (moveIncrement.y > 1) moveIncrement.y = 1;
	else if (moveIncrement.y < -1) moveIncrement.y = -1;
//	if ((this->context->target->location.x - target.x) > 1) {
//		target.x = 1;
//	} else if ((this->context->target->location.x
	/*
	if (this->context->echs >= 1) {
		targetX += 1;
	} else if (this->context->echs <= -1 ) {
		targetX += -1;
	}
	if (this->context->whye >= 1) {
		targetY += 1;
	} else if (this->context->whye <= -1 ) {
		targetY += -1;
	}
	*/
	this->context->dump();
	cpair moveTarget(this->context->target->location);
	moveTarget = moveTarget + moveIncrement;
//	if (this->context->vicinity->isBlocked(moveIncrement + this->context->target->location)) {
	if (this->context->vicinity->isBlocked(moveTarget)) {
		this->context->success = false;
	}
//	LOGMSG("? Is moving into " << moveTarget << " allowed? " << this->context->success);
	return this->context->success;
}
void MoveAction::execute() {
	// Perform the movement action
	context->target->setRelLocation(context->echs, context->whye);
//	LOGMSG(context->target->getName() << " now at " << context->target->location);
}
/* DISABLED: void Move: undo()
void MoveAction::undo() {
	context->echs *= -1;
	context->whye *= -1;
	context->target->setRelLocation(context->echs, context->whye);
}*/

/*   action.cpp
AUTH xazak
DATE Feb 13 2020
DESC Describes the Action class, which represents the set of possible 'verbs'
	 available to the Actors who possess a working Sentience module.
*/

#include "action.hpp"
#include "portable.hpp"
#include "container.hpp"
#include "openable.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "context.hpp"

using namespace std;

// **** ACTION Prototype
Action::Action() :
	context(nullptr),
	subject(nullptr),
	duration(0.0)
{	
//	LOGMSG("Action() ctor");
}
Action::Action(const Action& inputAction) :
	context(inputAction.context),
	subject(inputAction.subject),
	duration(inputAction.duration)
{//	LOGMSG("Action(Action) ctor");
//		this->context->dump();
}
Action::Action(const ActionContext& inputContext) {
	context = new ActionContext(inputContext);
	// FIXME: Missing subject and duration assignments!
}
Action::~Action() {
	delete context;
}
// ****************
// **** IDLE Action
IdleAction::IdleAction() {
	this->duration = 3;
}
void IdleAction::execute() {
	// Do nothing
	LOGMSG("Idling.");
}
// ****************
// **** MOVE Action
MoveAction::MoveAction(const ActionContext& inputContext) :
	Action(inputContext)
{	}
MoveAction::MoveAction(Actor* inputTarget, GameMap* inputArea, int targetX, int targetY) {
	context = new ActionContext(ActionType::MOVE, inputArea, inputTarget, inputTarget, targetX, targetY);
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
//	this->context->dump();
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
// ****************
// ****  GET Action
GetAction::GetAction(const ActionContext& inputContext) :
	Action(inputContext)
{	}
bool GetAction::isPlausible() {
	// tests against the action context
	// Does the target exist...?
	//  AND is the target portable...?
	//  AND does the subject have pockets...?
	//  AND does the subject have room in his pockets?
	return (context->target && context->target->portable && context->subject->contents && context->subject->contents->hasRoom());
}
void GetAction::execute() {
	context->target->portable->take(context->subject, context->target);
//	context->subject->contents->dump();
}
// ****************
// **** DROP Action
DropAction::DropAction(const ActionContext& inputContext) :
	Action(inputContext)
{	}
bool DropAction::isPlausible() {
	return (context->subject->contents->getSize());
}
void DropAction::execute() {
	context->target->portable->drop(context->subject, context->target);
}
// ****************
// **** OPEN Action
OpenAction::OpenAction(const ActionContext& inputContext) :
	Action(inputContext)
{	}
bool OpenAction::isPlausible() {
	// Does the target exist...?
	//  AND does the target have an aperture to be opened?
	//  AND is that aperture currently closed?
	return (context->target && context->target->aperture && !context->target->aperture->isOpen());
}
void OpenAction::execute() {
	context->target->aperture->open(*context->target);
}
// *****************
// **** CLOSE Action
CloseAction::CloseAction(const ActionContext& inputContext) :
	Action(inputContext)
{	}
bool CloseAction::isPlausible() {
	// Does the target exist...?
	//  AND does the target have an aperture to be closed?
	return (context->target && context->target->aperture && context->target->aperture->isOpen());
}
void CloseAction::execute() {
	context->target->aperture->close(*context->target);
}

// *****************
// **** META ACTIONS
// ****************
// **** QUIT Action
void QuitMetaAction::execute() {
	if (this) LOGMSG("Not sure how to QUIT from the parser yet...");
}
// ****************
// **** SAVE Action
void SaveMetaAction::execute() {
	if (this) LOGMSG("Not sure how to SAVE from the parser yet...");
}
// ****************
// **** RESTART Action
void RestartMetaAction::execute() {
	if (this) LOGMSG("Not sure how to RESTART from the parser yet...");
}

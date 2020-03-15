/*   action.cpp
AUTH xazak
DATE Feb 13 2020
DESC Describes the Action class, which represents the set of possible 'verbs'
	 available to the Actors who possess a working Sentience module.
*/

#include "BearLibTerminal.h"
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
Action::Action(const ActionContext& inputContext, const Actor* inputSubject, double inputDuration) :
	duration(inputDuration)
{	context = new ActionContext(inputContext);
	// FIXME: Missing subject and duration assignments!
}
Action::Action(const ActionContext& inputContext, double inputDuration) :
	duration(inputDuration)
{	}
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
	Action(inputContext, inputContext.subject, 0.25)
{	}
MoveAction::MoveAction(Actor* inputTarget, GameMap* inputArea, int targetX, int targetY) {
	context = new ActionContext(ActionType::MOVE, inputArea, inputTarget, inputTarget, targetX, targetY);
}
bool MoveAction::isPlausible() {
	// Clamp the target position to the actor's movement speed
	// Actor doesn't have a movement speed yet, assume 1 tile at a time
	// context->target->location : ORIGIN
	// targetN : DESTINATION
//	moveIncrement.set(this->context->echs, this->context->whye);
	int xIncrement = 0;
	int yIncrement = 0;
	if (context->target->location.x < this->context->echs) xIncrement = 1;
	if (context->target->location.x > this->context->echs) xIncrement = -1;
	if (context->target->location.y < this->context->whye) yIncrement = 1;
	if (context->target->location.y > this->context->whye) yIncrement = -1;
	moveIncrement.set(xIncrement, yIncrement);
	cpair moveTarget(context->target->location);
	moveTarget = moveTarget + moveIncrement;
	if (this->context->vicinity->isBlocked(moveTarget)) {
		this->context->success = false;
	} else {
		this->context->success = true;
	}
//	LOGMSG("? Is moving into " << moveTarget << " allowed? " << this->context->success);
//	this->context->dump(); // DEBUG
	return this->context->success;
}
void MoveAction::execute() {
	// Perform the movement action
	normalizeMoveIncrement();
	context->target->moveByRel(context->echs, context->whye);
//	LOGMSG(context->target->getName() << " moved to " << context->target->location << " by " << context->echs << ", " << context->whye);
}
void MoveAction::normalizeMoveIncrement() {
//	LOGMSG("Normalizing MOVE action");
	if (context->target->location.x < this->context->echs) context->echs = 1;
	else if (context->target->location.x > this->context->echs) context->echs = -1;
	else context->echs = 0;
	if (context->target->location.y < this->context->whye) context->whye = 1;
	else if (context->target->location.y > this->context->whye) context->whye = -1;
	else context->whye = 0;
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
	Action(inputContext, inputContext.subject, 0.0)
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
	Action(inputContext, inputContext.subject, 0.0)
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
	Action(inputContext, inputContext.subject, 0.0)
{	}
bool OpenAction::isPlausible() {
	// Does the target exist...?
	//  AND does the target have an aperture to be opened?
	//  AND is that aperture currently closed?
	if (context->target->location == context->subject->location) return false;
	return (context->target && context->target->aperture && !context->target->aperture->isOpen());
}
void OpenAction::execute() {
	context->target->aperture->open(*context->target);
}
// *****************
// **** CLOSE Action
CloseAction::CloseAction(const ActionContext& inputContext) :
	Action(inputContext, inputContext.subject, 0.0)
{	}
bool CloseAction::isPlausible() {
	// Does the target exist...?
	//  AND does the target have an aperture to be closed?
	// FIXME: check for same location AND obstruction (door vs barrel)...
	if (context->target->location == context->subject->location) return false;
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
	terminal_close();
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

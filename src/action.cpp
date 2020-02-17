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
{	}
Action::Action(const Action& inputAction) :
context(inputAction.context)
{	}

// **** IDLE Action
void IdleAction::execute() {
	// Do nothing
	LOGMSG("Idling.");
}
void IdleAction::undo() {
	// Do nothing
	LOGMSG(".gnidlI");
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
bool MoveAction::precondition() {
	// Clamp the target position to the actor's movement speed
	// Actor doesn't have a movement speed yet, assume 1 tile at a time
	if (context->echs > 1) context->echs = 1;
	if (context->echs < -1 ) context->echs = -1;
	if (context->whye > 1) context->whye = 1;
	if (context->whye < -1 ) context->whye = -1;
	if (context->vicinity->isBlocked(context->echs, context->whye)) {
		context->success = false;
	}
	return context->success;
}
void MoveAction::execute() {
	// Perform the movement action
	context->target->setRelLocation(context->echs, context->whye);
//	LOGMSG(context->target->getName() << " now at " << context->target->location);
}
void MoveAction::undo() {
	context->echs *= -1;
	context->whye *= -1;
	context->target->setRelLocation(context->echs, context->whye);
}

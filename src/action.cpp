/*   action.cpp
AUTH xazak
DATE Feb 13 2020
DESC Describes the Action class, which represents the set of possible 'verbs'
	 available to the Actors who possess a working Sentience module.
*/

#include "action.hpp"
#include "actor.hpp"
#include "context.hpp"

using namespace std;

Action::Action() :
context(nullptr)
{	}
Action::Action(const Action& inputAction) :
context(inputAction.context)
{	}

void IdleAction::execute() {
	// Do nothing
	LOGMSG("Idling.");
}
void IdleAction::undo() {
	// Do nothing
	LOGMSG(".gnidlI");
}

MoveAction::MoveAction() {
	context = new ActionContext();
}
MoveAction::MoveAction(const ActionContext& inputContext) {
	context = new ActionContext(inputContext);
}
MoveAction::MoveAction(Actor* inputTarget, int targetX, int targetY) {
	context = new ActionContext(ActionType::MOVE, inputTarget, targetX, targetY);
}
MoveAction::~MoveAction() {
	delete context;
}
void MoveAction::execute() {
	// Clamp the target position to the actor's movement speed
	// Actor doesn't have a movement speed yet, assume 1 tile at a time
	if (context->echs > 1) context->echs = 1;
	if (context->echs < -1 ) context->echs = -1;
	if (context->whye > 1) context->whye = 1;
	if (context->whye < -1 ) context->whye = -1;
	context->target->setRelLocation(context->echs, context->whye);
//	LOGMSG(context->target->getName() << " now at " << context->target->location);
}
void MoveAction::undo() {
	context->echs *= -1;
	context->whye *= -1;
	context->target->setRelLocation(context->echs, context->whye);
}

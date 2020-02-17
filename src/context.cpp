/*   context.cpp
AUTH xazak
DATE Feb 14 2020
DESC Describes the ActionContext object, which provides details as needed to
	 the Actors on the game map.
*/

#include "context.hpp"

using namespace std;

ActionContext::ActionContext() :
type(ActionType::IDLE),
target(nullptr),
vicinity(nullptr),
echs(0),
whye(0),
zhee(0),
success(true)
{	}
ActionContext::ActionContext(const ActionContext& inputContext) :
type(inputContext.type),
target(inputContext.target),
vicinity(inputContext.vicinity),
echs(inputContext.echs),
whye(inputContext.whye),
zhee(inputContext.zhee),
success(inputContext.success)
{	}
ActionContext::ActionContext(ActionType inputType, Actor *inputTarget, GameMap *inputArea, int inputX, int inputY, int inputZ, bool inputFlag) :
type(inputType),
target(inputTarget),
vicinity(inputArea),
echs(inputX),
whye(inputY),
zhee(inputZ),
success(inputFlag)
{	}
ActionContext& ActionContext::operator= (const ActionContext& rhs) {
	LOGMSG("Setting this->type to " << (uint)rhs.type);
	this->type = rhs.type;
	this->target = rhs.target;
	this->echs = rhs.echs;
	this->whye = rhs.whye;
	this->zhee = rhs.zhee;
	this->success = rhs.success;
	return *this;
}
void ActionContext::clear() {
	this->type = ActionType::IDLE;
	this->target = nullptr;
	this->echs = 0;
	this->whye = 0;
	this->zhee = 0;
	this->success = true;
}
void ActionContext::set(Actor *inputTarget, int inputX, int inputY, int inputZ, bool inputFlag) {
	// Sets a context object to the specified values
	this->target = inputTarget;
	this->echs = inputX;
	this->whye = inputY;
	this->zhee = inputZ;
	this->success = inputFlag;
}

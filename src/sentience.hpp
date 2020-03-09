/*   sentience.hpp
AUTH xazak
DATE Feb 13 2020
DESC Describes the Sentience module for Actors, which enables access to the
	Action system and includes packages for decision making.
*/

#ifndef SPACEGAME_SENTIENCE
#define SPACEGAME_SENTIENCE

//#include "action.hpp"
#include "context.hpp"
#include <stack>

class Action;
class Drone;
class MessageLog;

class Sentience {
	public:
		Sentience();
		//~Sentience();
//		void pushAction(const ActionContext& context);
		virtual void pushAction(Action* nextAction) = 0;
//		void pushAction(const ActionContext& input);
		virtual void doNextAction() = 0;
		virtual bool hasActions() { return actionReady; }
//		Action* getAction();
//		Action popAction();
//		ActionType peekAction();
		bool isFocused();
		virtual void continueWorking() {}

	protected:
		// Note that Action objects contain their own context!
		MessageLog* msgOutput;
		double remainingTime; // time left until the action is complete
		bool actionReady;
};

class PlayerSentience : public Sentience {
	public:
		PlayerSentience();
		void doNextAction();
		void pushAction(Action* nextAction);
	private:
		Action* nextAction;
		Action* prevAction;
};
class DroneSentience : public Sentience {
	public:
		DroneSentience(Actor* owner);
		void doNextAction();
		void pushAction(Action* nextAction);
		bool hasActions();
		void continueWorking();
	private:
		std::stack<Action*> actionStack;
		ActionContext localContext;
		void consider();
		Actor* findPlayer();
};
#endif //SPACEGAME_SENTIENCE

/*   sentience.hpp
AUTH xazak
DATE Feb 13 2020
DESC Describes the Sentience module for Actors, which enables access to the
	Action system and includes packages for decision making.
*/

#ifndef SPACEGAME_SENTIENCE
#define SPACEGAME_SENTIENCE

#include "action.hpp"
//#include "context.hpp"
#include <stack>

class Sentience {
	public:
//		Sentience();
		//~Sentience();
//		void pushAction(const ActionContext& context);
		virtual void pushAction(Action* nextAction) = 0;
//		void pushAction(const ActionContext& input);
		virtual void doNextAction() = 0;
		virtual bool hasActions() = 0;
//		Action* getAction();
//		Action popAction();
//		ActionType peekAction();
	private:
		// Note that Action objects contain their own context!
};

class PlayerSentience : public Sentience {
	public:
		PlayerSentience();
		void pushAction(Action* nextAction);
		void doNextAction();
		bool hasActions() { return actionReady; }
	private:
		Action* nextAction;
		Action* prevAction;
		bool actionReady;
};
#endif //SPACEGAME_SENTIENCE

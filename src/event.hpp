/*   event.hpp
AUTH xazak
DATE Mar 15 2020
DESC Implements the GameEvent class, which handles things like countdown timers
     as well as conditional triggers and scripted events
*/

#ifndef SPACEGAME_EVENT
#define SPACEGAME_EVENT

#include "main.hpp"

class GameEngine;
class GameEvent {
	public:
		GameEvent();
		virtual void update() = 0; // Check activation state vs world state
		// trigger activation
		virtual void execute() = 0; // Carry out the event consequences
		static void registerEngine(GameEngine *enginePtr);
		bool isActive() { return activeState; }
		bool isResolved() { return resolvedState; }

	protected:
		static GameEngine *engine;
		bool activeState; // FALSE = inactive
		bool resolvedState; // FALSE = has not been carried out
		// carry out consequences
		// provide feedback for listeners

};

class CountdownTimer : public GameEvent {
	// Counts down to zero (0) in milliseconds as called by update()
	public:
		CountdownTimer(uint remainingTime);
		void update(); // add or remove time from the timer
		void execute();
		int getRemainingTime() { return remainder; }

	private:
		int remainder; // in milliseconds
};


#endif // SPACEGAME_EVENT

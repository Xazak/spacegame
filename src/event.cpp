/*   event.cpp
AUTH xazak
DATE Mar 15 2020
DESC Implements the GameEvent class, which handles things like countdown timers
     as well as conditional triggers and scripted events
*/

#include "event.hpp"
#include "engine.hpp"

GameEngine* GameEvent::engine = nullptr;

GameEvent::GameEvent() :
	activeState(true),
	resolvedState(false)
{	}
void GameEvent::registerEngine(GameEngine *enginePtr) {
	GameEvent::engine = enginePtr;
}

CountdownTimer::CountdownTimer(uint remainingTime) :
	remainder(remainingTime)
{	//LOGMSG("Timer created: " << remainder);
}
void CountdownTimer::update() {
	if (remainder <= 0) {
		this->execute();
	} else {
		int timeOffset = engine->worldClock.getLastDelta();
		remainder -= (timeOffset % 10);
//		LOGMSG("Time remaining: " << remainder << " / delta: " << timeOffset);
	}
}
void CountdownTimer::execute() {
	// FIXME: place a gameover message somewhere?
	// set the engine state to DEFEAT
	engine->getGUIPtr()->addMessage("You ran out of time!");
	GameEngine::switchMode(GameEngine::EngineState::DEFEAT);
	// This is a global event, therefore never really inactive..
	this->resolvedState = true;
}

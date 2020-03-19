/*   event.cpp
AUTH xazak
DATE Mar 15 2020
DESC Implements the GameEvent class, which handles things like countdown timers
     as well as conditional triggers and scripted events
*/

#include "event.hpp"
#include "engine.hpp"
#include <string>
#include <chrono>

using namespace std;

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
	// Timers are advanced alongside the world clock
	if (remainder <= 0) {
		this->execute();
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
void CountdownTimer::advanceTime(double increment) {
	remainder -= increment;
	if (remainder <= 0) remainder = 0;
}
string CountdownTimer::getRemainingTimeString() {
	// Formats the remainder as a string before returning it
	return Chrono::timeToString(remainder);
}

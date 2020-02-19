/*   main.cpp
AUTH xazak
DATE Jan 27 2020
DESC Contains the implementation of main()
*/


// *** INCLUDES
#include "actor.hpp"
#include "engine.hpp"
#include "gui.hpp"
#include "main.hpp"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	// Load the configuration and any command line options
	string configFileName = "config.txt";
	if (argc >= 2) {
		// Parse the command line options
		// need to invent some command line options!
		ERRMSG("No command line options are available at this time.");
		return 1; // Exit the program by throwing a generic error code
	}
	// Start loading up the various game modules and initializing them
//	LOGMSG("Creating and initializing engine.");
	GameEngine engine;
	if (!engine.initialize(configFileName)) { // Try initializing the engine
		// If it didn't work for some reason, say so and exit
		ERRMSG("There was a problem loading the configuration.");
		ERRMSG("The game will now exit.");
		return 5; // Exit the program and throw a (different) error code
	}
//	LOGMSG("Engine started successfully.");
	// Invoke the game loop
//	LOGMSG("Starting the game...");
	engine.execGameLoop();
	// WHEN the player has closed the game:
//	LOGMSG("The engine will now halt and terminate.");
	engine.terminate();
}

// **** CoordinatePair METHODS
CoordinatePair::CoordinatePair() : x(0), y(0) { } // The default ctor
CoordinatePair::CoordinatePair(int inputX, int inputY) :
x(inputX), y(inputY) { } // The specific ctor
ostream& operator<< (ostream &output, const CoordinatePair& inputPair) {
	output << inputPair.x << ", " << inputPair.y;
	return output;
}
CoordinatePair CoordinatePair::operator+(const CoordinatePair& rhs) {
	cpair lhs;
	lhs.x = this->x + rhs.x;
	lhs.y = this->y + rhs.y;
//	LOGMSG(*this << " + " << rhs << " + " << lhs);
	return lhs;
}

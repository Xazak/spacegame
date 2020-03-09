/*   engine.cpp
DATE Jan 27 2020
AUTH xazak
DESC Contains implementation of game engine
*/

#include "BearLibTerminal.h"
#include "engine.hpp"
#include "item.hpp"
#include "main.hpp"
#include <iostream>		// Provides access to stdin/stdout (cout, cerr, etc)
#include <sstream>		// Object for conversion from std::string to input stream
#include <fstream>		// Simple file input/output
#include <chrono>		// steady_clock, maybe others? system time?

using namespace std;

GameEngine::GameEngine() :
	currMode(STARTUP),
	prevMode(STARTUP),
	screenWidth(80),
	screenHeight(50),
	cliMode(false)
{
//	LOGMSG("Default ctor called.");
	// The default constructor
}

// *** CORE FUNCTIONS
bool GameEngine::initialize(std::string configFile) {
	// Sets up the initial game state; this is NOT in the constructor because
	// we want to keep track of whether an error has arisen from the GameEngine
	// class or some other sub-module of the system
	if (!loadConfiguration(configFile)) { // Try loading the config file
		// If it didn't work, do not continue!
		return false;
	}
	if (!terminal_open()) { // Try creating a BearLibTerminal instance
		// If it didn't work, send an error message to stderr
		ERRMSG("*!* There was a problem starting BearLibTerminal.");
		return false;
	}
	// Set up BearLibTerminal
	string bltConfigString = generateBLTConfigString();
//	LOGMSG("Generated BLT configuration:\n " << bltConfigString);
	terminal_set(bltConfigString.c_str()); // Get BLT set up to its default state
	// Perform any remaining setup and module creation
	// parser, gui, meatspace, player objects are already created!
	meatspace.generateMap(30, 14);
	// FIXME: Need to wrap some of this up in a player init method...
	// Establish the player in meatspace
	player.setLocality(&meatspace); // Set the player's map pointer
	player.setAbsLocation(5, 5); // Set a valid location
	meatspace.allActors.push_back(&player); // Add to the visibility list
	sentientActors.push_back(&player); // Add to the sentients list
	parser.setPlayer(&player); // Initialize the parser-to-player linkage
	// Establish the LMR
	lemur.setLocality(&meatspace);
	lemur.setAbsLocation(7, 7);
	meatspace.allActors.push_back(&lemur);
	sentientActors.push_back(&lemur);
	// Initialize the GUI
	GameEngine* tempPtr = this;
	gui.initialize(screenWidth, screenHeight, tempPtr, &player, &meatspace);
	ServiceLocator::provide(gui.getMessageLog());
	return true;
}
void GameEngine::execGameLoop() {
	// Handler function called by main() that coordinates the game loop and
	// some setup methods
	// BLT display explicitly requires an initial call to _refresh() prior to
	// displaying anything onscreen
	using namespace chrono;
//	LOGMSG("Calling...");
	terminal_refresh();
	steady_clock::time_point prevTime = steady_clock::now();
	steady_clock::time_point currTime = prevTime;
	duration<double> timeSpan = duration_cast<duration<double>>(currTime - prevTime);
	duration<double> lagTime = timeSpan;
	duration<double> updateTimeStep = duration<double>(MS_PER_UPDATE);

	// TK_CLOSE == true when the terminal window is closed
	// _peek does not block if false (unlike _read)
	while (terminal_peek() != TK_CLOSE) {
		currTime = steady_clock::now(); // get snapshot of current timepoint
		timeSpan = currTime - prevTime; // get the diff since last snapshot
		prevTime = currTime;			// update the snapshots forward
		lagTime += timeSpan;			// add the diff to the accumulator
		// Handle player inputs
		if (terminal_has_input()) { // Is there control input waiting?
			// Parse the command input by reading it from terminal_
			// Need to trap meta and debug calls before passing to the parser
			int inputValue = terminal_read(); // Find out what key was struck
			char inputKey = char(terminal_state(TK_CHAR));
//			LOGMSG("@@@ Keypress: " << inputKey);
			if (inputValue == TK_Q) { // Press Q to quit
				break;
			} else if (inputValue == TK_ESCAPE) { // (test) pause
//				LOGMSG("Testing pause functions...");
				this->togglePause();
			} else if (terminal_check(TK_SHIFT) && inputValue == TK_SEMICOLON) { // raise the CLI
//				LOGMSG("Entering CLI mode");
				this->cliMode = true;
				gui.raiseCLI();
			} else if (inputValue == TK_P) { // do some in-game testing
				LOGMSG("Dropping a wrench...");
				// drop a wrench for the player to pick up
				Wrench *myWrench = new Wrench(7, 7, &meatspace);
//				myWrench->setLocality(&meatspace);
				meatspace.registerItem(myWrench, 7, 7);
//				myWrench->setAbsLocation(7, 7);
//				meatspace.allActors.push_back(myWrench);
			} else if (inputValue >= TK_A && inputValue <= TK_Z) { // don't pass meta keys
//				LOGMSG("@@@ Keypress: " << inputKey);
				if (this->currMode != PAUSED) parser.interpret(inputKey);
			}
		}
		// Set flag HERE for detecting whether the player's input has created a
		// VICTORY or DEFEAT engine state

//		while (timeSpan >= duration_cast<duration<double>>(MS_PER_UPDATE)) {
		while (lagTime >= updateTimeStep) {
			update(); // Perform game update routines based on engine state
			lagTime -= updateTimeStep;
		}
		// Push an update to the screen: the input value is the fractional
		// amount of time that we have progressed into the 'next' frame
		// The value is normalized by updateTimeStep before it is passed
//		LOGMSG("Requesting GUI render with lagTime == " << lagTime.count());
		gui.render(lagTime / updateTimeStep);
	};
}
void GameEngine::update() {
	// Polls various game modules to see if they would like to make some kind
	// of change to the game state by way of their available actions
	switch(currMode) {
		case STARTUP:
			// Things that occur only once, when the game loop is started
//			LOGMSG("Engine is doing STARTUP");
			this->switchMode(ONGOING);
			break;
		case ONGOING:
			// The 'default' running game state
			// EZ flags for testing engine state changes
//			LOGMSG("Engine is ONGOING");
			if (player.location.isEqual(26, 10)) this->switchMode(VICTORY);
			if (player.location.isEqual(26, 3)) this->switchMode(DEFEAT);
			// ask for Actor updates from the list
			break;
		case PAUSED:
			// Halts all in-game action, but allows metagame functions
//			LOGMSG("Engine is PAUSED");
			break;
		case VICTORY:
			// Displays a GOOD END banner and halts the game
			LOGMSG("VICTORY state attained");
			break;
		case DEFEAT:
			// Displays a BAD END banner and halts the game
			LOGMSG("DEFEAT state attained");
			break;
		default:
			ERRMSG("An invalid engine state was encountered!");
			this->terminate();
			break;
	}
	if (cliMode) {
		gui.raiseCLI();
	}
//	LOGMSG("Updating game state.");
	for (auto actorIter = sentientActors.begin(); actorIter != sentientActors.end(); actorIter++) {
//		LOGMSG("Update request: " << (*actorIter)->getName());
		if (*actorIter == &player) {
			// do some player-specific stuff?
			// the parser already invokes the inputted command, rather than
			// having it invoked here
		} else {
			(*actorIter)->update();
		}
	}
//	LOGMSG("Player position: " << player.location);
}
void GameEngine::terminate() {
	// Performs closing-of-the-game methods before the engine itself shuts down
	// If we wanted to save the game automatically, we could do so here
	terminal_close(); // Halt the BearLibTerminal instance
}
void GameEngine::switchMode(EngineState newMode) {
	prevMode = currMode;
	currMode = newMode;
/*xxx DEBUG: display state switch messages
	switch (currMode) {
		case STARTUP:
			LOGMSG("mode switch: " << currMode << ": STARTUP");
			break;
		case IDLE:
			LOGMSG("mode switch: " << currMode << ": IDLE");
			break;
		case ONGOING:
			LOGMSG("mode switch: " << currMode << ": ONGOING");
			break;
		case NEWTURN:
			LOGMSG("mode switch: " << currMode << ": NEWTURN");
			break;
		case VICTORY:
			LOGMSG("mode switch: " << currMode << ": VICTORY");
			break;
		case DEFEAT:
			LOGMSG("mode switch: " << currMode << ": DEFEAT");
			break;
		default:
			break;
	}
	*/
}
void GameEngine::pauseOn() {
	if (this->currMode != PAUSED) this->switchMode(PAUSED);
}
void GameEngine::pauseOff() {
	if (this->currMode == PAUSED) this->switchMode(prevMode);
}
void GameEngine::togglePause() {
	if (this->currMode != PAUSED) this->switchMode(PAUSED);
	else this->switchMode(this->prevMode);
}
void GameEngine::interpretLongCommand(const char * inputBuffer) {
	// Takes a long command from the GUI and sends it for parsing
	this->cliMode = false;
	parser.interpret(inputBuffer);
	gui.hideCLI();
}
// *** UTILITIES
bool GameEngine::loadConfiguration(std::string inputFile) {
	std::ifstream config(inputFile); // Open the configuration file
	if (!config) { // Was the config file opened successfully?
		// If not, display an error and exit
		cerr << "The configuration file could not be opened." << std::endl;
		return false;
	}
	stringstream lineStream; // Allows parsing single lines by chars
	string configLine; // Contains a whole line
	string configKey; // Contains the configuration property to be set
	string configValue; // Contains the new value for the config property
	while (!config.eof()) { // Parse all lines in the config file
		getline(config, configLine); // Obtain a full line
		lineStream.clear(); // Make sure the stream is empty
		lineStream << configLine; // Put the config line into the stream
		// Break the stream into a key and its value at the '='
		getline(lineStream, configKey, '=');
		getline(lineStream, configValue);
		// Find the matching configuration property and set its value
		// FIXME: Include sanity checks on input values
		// FIXME: Set up some kind of value defaults if anything's not set
		if (configKey != "") { // Prevent trying blank lines (ie trailing lines)
			if (configKey == "screenWidth") {
				screenWidth = std::stoul(configValue, nullptr, 0);
			} else if (configKey == "screenHeight") {
				screenHeight = std::stoul(configValue, nullptr, 0);
			} else if (configKey == "font") {
				terminalFontPath = configValue;
			} else if (configKey == "fontSize") {
				terminalFontSize = std::stoul(configValue, nullptr, 0);
			} else { // No matching config key was found!
				std::cerr << "*** Configuration key " << configKey << " is not recognized by the game." << std::endl;
			}
		}
	}
	config.close();
	return true;
}
string GameEngine::generateBLTConfigString() {
	// Generates a valid BearLibTerminal configuration string from our config
	// Set the window title
	string windowTitleOption = "title='SPACEGAME'";
	// Set the window dimensions
	string windowSizeOption  = "size=";
	windowSizeOption.append(to_string(screenWidth));
	windowSizeOption.append("x");
	windowSizeOption.append(to_string(screenHeight));
	// If we want to set up mouse input, it needs to be enabled here
	// If we want to specify custom color names, we can do that here too
	// ***
	// Finally, generate the string to be returned
	string fullOptionString;
	// The terminal window title and dimensions will always be set
	fullOptionString.append("window: ");
	fullOptionString.append(windowTitleOption);
	fullOptionString.append(", ");
	fullOptionString.append(windowSizeOption);
	// Only add the font option if it has been specified
	if (terminalFontPath != "") {
		// If we specify a font, we MUST also specify a size
		fullOptionString.append("; font: ");
		fullOptionString.append(terminalFontPath);
		fullOptionString.append(", size=");
		fullOptionString.append(to_string(terminalFontSize));
	}
	fullOptionString.append(";"); // Terminating character
	return fullOptionString;
}

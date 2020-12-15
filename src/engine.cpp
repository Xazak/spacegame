/*   engine.cpp
DATE Jan 27 2020
AUTH xazak
DESC Contains implementation of game engine
*/

#include "BearLibTerminal.h"
#include "engine.hpp"
#include "item.hpp"
#include "main.hpp"
#include "chrono.hpp"	// In-game time functions
#include <iostream>		// Provides access to stdin/stdout (cout, cerr, etc)
#include <sstream>		// Object for conversion from std::string to input stream
#include <fstream>		// Simple file input/output
#include <chrono>		// steady_clock, maybe others? system time?

using namespace std;

GameEngine::EngineState GameEngine::currMode = EngineState::STARTUP;
GameEngine::EngineState GameEngine::prevMode = GameEngine::currMode;

GameEngine::GameEngine() :
//	currMode(STARTUP),	// public
//	prevMode(STARTUP),
	lagTime(0.0),
	screenWidth(80),	// private
	screenHeight(50),
	cliMode(false)
{
//	LOGMSG("Default ctor called.");
	// The default constructor
}

// *** CORE FUNCTIONS
bool GameEngine::initialize(string configFile) {
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
	// Initialize the starting Event list
	GameEvent::registerEngine(this);
//	GameEvent *gravityWell = new CountdownTimer(30000);
	gravityWell = new CountdownTimer(1000);
	eventList.push_back(gravityWell);
	timerList.push_back(gravityWell);

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
	update(); // Call once to resolve STARTUP methods
	steady_clock::time_point prevTime = steady_clock::now();
	steady_clock::time_point currTime = prevTime;
	duration<double> timeSpan = duration_cast<duration<double>>(currTime - prevTime);
//	duration<double> lagTime = timeSpan;
	lagTime = timeSpan;
	duration<double> updateTimeStep = duration<double>(SECONDS_PER_UPDATE);

	// *** THE CORE GAME LOOP
	// TK_CLOSE == true WHEN the terminal window is closed
	// terminal_peek does not block if false (unlike terminal_read)
	while (terminal_peek() != TK_CLOSE) {
		currTime = steady_clock::now(); // get snapshot of current timepoint
		timeSpan = currTime - prevTime; // get the diff since last snapshot
		prevTime = currTime;			// update the snapshots forward
		lagTime += timeSpan;			// add the diff to the accumulator
		if (currMode == ONGOING) {
			worldClock.update(timeSpan.count()); // increment the world clock
			// update any running game timers
			for (auto timerIter = timerList.begin(); timerIter != timerList.end(); timerIter++) {
				(*timerIter)->advanceTime(timeSpan.count());
			}
		}
		// **** Handle player inputs
		if (terminal_has_input()) { // Is there control input waiting?
			// Parse the command input by reading it from terminal_
			// Need to trap meta and debug calls before passing to the parser
			int inputValue = terminal_read(); // Find out what key was struck
			char inputKey = char(terminal_state(TK_CHAR));
//			LOGMSG("@@@ Keypress: " << inputKey);
			// NEW METHOD:
			// ? Is the game currently paused?
			// > Y: use the MetaParser (do meta commands, move thru menus)
			// > N: use the GameParser (do player actions)
			// **** NEW METHOD
			if (currMode == PAUSED) {
				// make sure there's a window raised or something
				// interpret command as meta input: menu move, save, quit, &c
				if (inputValue == TK_Q) { // quit is a special case right now
					gui.addMessage("Press q again to quit the game.");
					break; // force the while loop to halt
					// FIXME: Creating an actual QUIT signal here will allow
					// the engine to exit correctly from any depth rather than
					// relying on a feature of the C++ loop to work...
				//} else { // pass it to the metaparser
				} else if (inputValue == TK_S) {
					// save the game
					gui.addMessage("DEBUG: saving game");
					saveGame("testSave.txt");
				} else if (inputValue == TK_P) {
					gui.addMessage("Wrench dropping temporarily disabled.");
				}
			} else {
				// check for any special debugging keys
				// interpret command as game input, CLI raise, etc (old method)
				if (inputValue == TK_ESCAPE) {
					togglePause();
				} else if (terminal_check(TK_SHIFT) &&
							inputValue == TK_SEMICOLON) { // raise the CLI
//					LOGMSG("Entering CLI mode");
					this->cliMode = true;
					gui.raiseCLI();
				} else if (inputValue >= TK_A &&
							inputValue <= TK_Z) { // don't pass meta keys
//					LOGMSG("@@@ Keypress: " << inputKey);
					// reject input if game is paused
					if (currMode != PAUSED) parser.interpret(inputKey);
				}
			}
			// **** OLD METHOD
			/*
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
				if (currMode != PAUSED) parser.interpret(inputKey); // reject input if game is paused
			}
			*/
		}
		// Set flag HERE for detecting whether the player's input has created a
		// VICTORY or DEFEAT engine state

//		while (timeSpan >= duration_cast<duration<double>>(SECONDS_PER_UPDATE)) {
//		LOGMSG("lag: " << lagTime.count());
		while (lagTime >= updateTimeStep) {
			update(); // Perform game update routines based on engine state
			lagTime -= updateTimeStep;
		}
		// Push an update to the screen: the input value is the fractional
		// amount of time that we have progressed into the 'next' frame
		// The value is normalized by updateTimeStep before it is passed
//		LOGMSG("Requesting GUI render with lagTime == " << lagTime.count());
		gui.render(lagTime / updateTimeStep);
		if (currMode == DEFEAT || currMode == VICTORY) break;
		else if (currMode == PAUSED) {
			// draw the main menu on the screen
		}
	};
	// if we made it here, the game stopped but the window is still open
	if (currMode == DEFEAT) {
		// print a defeat banner
		gui.addMessage("You have been defeated!");
	} else if (currMode == VICTORY) {
		// print a victory banner
		gui.addMessage("You have won the game!");
	}
	gui.render(lagTime / updateTimeStep);
	while (terminal_read() != TK_Q) {
		gui.render(lagTime / updateTimeStep);
		if (terminal_peek() == TK_CLOSE) break;
	}
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
			for (auto actorIter = sentientActors.begin(); actorIter != sentientActors.end(); actorIter++) {
//				LOGMSG("Update request: " << (*actorIter)->getName());
				if (*actorIter == &player) {
					// do some player-specific stuff?
					// the parser already invokes the inputted command, rather than
					// having it invoked here
				} else {
					(*actorIter)->update();
				}
			}
			for (auto eventIter = eventList.begin(); eventIter != eventList.end(); eventIter++) {
				if ((*eventIter)->isActive()) (*eventIter)->update();
			}
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
//	LOGMSG("Player position: " << player.location);
}
void GameEngine::terminate() {
	// Performs closing-of-the-game methods before the engine itself shuts down
	// If we wanted to save the game automatically, we could do so here
	delete gravityWell;
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
bool GameEngine::loadConfiguration(string inputFile) {
	ifstream config(inputFile); // Open the configuration file
	if (!config) { // Was the config file opened successfully?
		// If not, display an error and exit
		cerr << "The configuration file could not be opened." << endl;
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
				screenWidth = stoul(configValue, nullptr, 0);
			} else if (configKey == "screenHeight") {
				screenHeight = stoul(configValue, nullptr, 0);
			} else if (configKey == "font") {
				terminalFontPath = configValue;
			} else if (configKey == "fontSize") {
				terminalFontSize = stoul(configValue, nullptr, 0);
			} else { // No matching config key was found!
				cerr << "*** Configuration key " << configKey << " is not recognized by the game." << endl;
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
bool GameEngine::saveGame(string fileName) {
	// Creates a game save at the specified file name.
	// Returns true ONLY if it was successful.
	gui.addMessage("Saving game to " + fileName);
	// Object List, in order of initialization
	// Parser and GUI do not need persistence
	// Meatspace: a GameMap, containing a 2d array of Tiles and two uints
	// Player
	// Lemur
	// [lists] (sentients, events, timers)
	// Gravity well
	ofstream output(fileName); // open the save game file for writing
	if (!output) {
		cerr << filename << " could not be opened for saving the game!" << endl;
		return false;
	}
	output << "# * MEATSPACE - GameMap" << endl;
	output << meatspace.getWidth() << ", " << meatspace.getHeight() << endl;

	return true;
}
// functions that write primitives to the specified output
ostream& operator<< (ostream &output, const Tile &inputTile) {
	// The explicit Tile ctor takes the following values in this order
	// output << inputFile.tileType << ", "; // FIXME
	output << inputTile.sigil << ", ";		// int
	output << inputTile.color << ", ";		// int
	output << inputTile.bkcolor << ", ";	// int
	output << inputTile.name << ", ";		// string
	// The four flags that set the Tile's behavior
	// If the tile contains anything (flag 4) then the next line will be the
	// list of items that are contained in the tile
	output << inputTile.explored;
	output << inputTile.obstructs;
	output << inputTile.opaque;
	// - END Tile ctor values, only add the Contents if the last flag is true
	if (inputTile.contents) output << true << endl << inputTile.contents;
	else output << false << endl;
	// Note that the tile's Occupant and Furniture will be added via their list
	return output;
}
bool GameEngine::loadGame(string fileName) {
	// Loads the specified game save.
	// Returns true ONLY if it was successful.
//	ERRMSG("Game loading has not been implemented!");
	ifstream input(fileName);
	if (!input) {
		cerr << fileName << " could not be opened for loading!" << endl;
		return false;
	}

	return true;
}


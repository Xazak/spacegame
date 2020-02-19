/*   engine.cpp
DATE Jan 27 2020
AUTH xazak
DESC Contains implementation of game engine
*/

#include "BearLibTerminal.h"
#include "engine.hpp"
#include "main.hpp"
#include <iostream>		// Provides access to stdin/stdout (cout, cerr, etc)
#include <sstream>		// Object for conversion from std::string to input stream
#include <fstream>		// Simple file input/output

using namespace std;

GameEngine::GameEngine() :
screenWidth(80),
screenHeight(50)
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
	meatspace.generateMap(20, 20);
	player.setLocality(&meatspace);
	player.setAbsLocation(5, 5);
//	actorList.push_front(&player);
	parser.setPlayer(&player); // Initialize the parser-to-player linkage
	// Initialize the GUI
	gui.initialize(screenWidth, screenHeight, &player, &meatspace);
	return true;
}
void GameEngine::execGameLoop() {
	// BLT display explicitly requires an initial call to _refresh() prior to
	// displaying anything onscreen
	terminal_refresh();
	// TK_CLOSE == true when the terminal window is closed
	// _peek does not block if false (unlike _read)
	while (terminal_peek() != TK_CLOSE) {
		if (terminal_has_input()) { // Is there control input waiting?
			// Parse the command input by reading it from terminal_
			// Need to trap meta and debug calls before passing to the parser
			int inputValue = terminal_read(); // Find out what key was struck
			char inputKey = char(terminal_state(TK_CHAR));
//			LOGMSG("@@@ Keypress: " << inputKey);
			if (inputValue == TK_Q) { // Press Q to quit
				break;
			} else if (inputValue == TK_M) { // test message log
//				LOGMSG("Testing message log.");
				gui.testMessageLog();
			} else {
				parser.interpret(inputKey);
			}
		}
		update();
//		gui.update(); // Make sure the GUI catches any state changes
		gui.render(); // Update the game screen
	};
}
void GameEngine::update() {
	// Polls various game modules to see if they would like to make some kind
	// of change to the game state by way of their available actions
//	LOGMSG("Updating game state.");
	for (auto actorIter = actorList.begin(); actorIter != actorList.end(); actorIter++) {
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
// *** UTILITIES
void GameEngine::sendMessage(string messageText) {
//	LOGMSG(messageText);
//	this->gui.addMessage(messageText);

}
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

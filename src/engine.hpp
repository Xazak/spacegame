/*   engine.cpp
DATE Jan 27 2020
AUTH xazak
DESC Contains definitions of the GameEngine class
*/
#ifndef SPACEGAME_ENGINE
#define SPACEGAME_ENGINE

#include "parser.hpp"
#include "map.hpp"
#include "actor.hpp"
#include "gui.hpp"
#include <string>
#include <list>
//#include <random>

class GameEngine {
	public:
		GameEngine();
//		~GameEngine();
		bool initialize(std::string configFile); // Loads state from config
		void execGameLoop(); // Performs the game itself and handles BLT
		void update(); // Requests game module updates
		void terminate(); // Performs cleanup when the game is closed
		void sendMessage(std::string messageText);
		
		// GET
		uint getScreenWidth()	{ return screenWidth; }
		uint getScreenHeight()	{ return screenHeight; }

	private:
		bool loadConfiguration(std::string configFile); // Loads external config
		std::string generateBLTConfigString(); // Generates a BearLibTerminal configuration string for terminal_set()
		// Game objects
		GameParser parser;
		GameGUI gui;
		GameMap meatspace;
		Player player;
		std::list<Actor*> actorList;
		// Properties
		uint screenWidth; // Width of terminal in # of monospace chars
		uint screenHeight; // Height of terminal in # of monospace chars
		std::string terminalFontPath; // Contains relative path to the terminal font
		uint terminalFontSize; // Size of the terminal font
};

#endif //SPACEGAME_ENGINE

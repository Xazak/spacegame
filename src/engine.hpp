/*   engine.hpp
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
		enum EngineState {
			STARTUP,	//0
			ONGOING,	//1
			PAUSED,		//2
			VICTORY,	//3
			DEFEAT		//4
		} currMode, prevMode;
		GameEngine();
//		~GameEngine();
		bool initialize(std::string configFile); // Loads state from config
		void execGameLoop(); // Performs the game itself and handles BLT
		void update(); // Requests game module updates
		void terminate(); // Performs cleanup when the game is closed
		void switchMode(EngineState newMode); // handles engine state changes
		void pauseOn();
		void pauseOff();
		void togglePause();
		
		// GET
		uint getScreenWidth()	{ return screenWidth; }
		uint getScreenHeight()	{ return screenHeight; }
		Actor* getPlayerPtr() { return (&player); }
		GameMap* getMeatspacePtr() { return (&meatspace); }

	private:
		bool loadConfiguration(std::string configFile); // Loads external config
		std::string generateBLTConfigString(); // Generates a BearLibTerminal configuration string for terminal_set()
		// Game objects
		GameParser parser;
		GameGUI gui;
		GameMap meatspace;
		Player player;
		std::list<Actor*> sentientActors; // Actors who will be taking actions
		// Properties
		uint screenWidth; // Width of terminal in # of monospace chars
		uint screenHeight; // Height of terminal in # of monospace chars
		std::string terminalFontPath; // Contains relative path to the terminal font
		uint terminalFontSize; // Size of the terminal font
};

#endif //SPACEGAME_ENGINE

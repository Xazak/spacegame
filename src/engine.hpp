/*   engine.hpp
DATE Jan 27 2020
AUTH xazak
DESC Contains definitions of the GameEngine class
*/
#ifndef SPACEGAME_ENGINE
#define SPACEGAME_ENGINE

#include "parser.hpp"
#include "map.hpp"
#include "tile.hpp"
#include "actor.hpp"
#include "chrono.hpp"
#include "gui.hpp"
#include "event.hpp"
#include <string>
#include <list>
#include <chrono>
//#include <random>

class GameEngine {
	public:
		enum EngineState {
			STARTUP,	//0
			ONGOING,	//1
			PAUSED,		//2
			VICTORY,	//3
			DEFEAT		//4
		};
		static EngineState currMode;
		static EngineState prevMode;
		GameEngine();
//		~GameEngine();
		bool initialize(std::string configFile); // Loads state from config
		void execGameLoop(); // Performs the game itself and handles BLT
		void update(); // Requests game module updates
		void terminate(); // Performs cleanup when the game is closed
		static void switchMode(EngineState newMode); // handles engine state changes
		void pauseOn();
		void pauseOff();
		void togglePause();
		void interpretLongCommand(const char * inputBuffer);

		// GET
		uint getScreenWidth()	{ return screenWidth; }
		uint getScreenHeight()	{ return screenHeight; }
		Actor* getPlayerPtr() { return (&player); }
		GameMap* getMeatspacePtr() { return (&meatspace); }
		GameGUI* getGUIPtr() { return (&gui); }
		CountdownTimer* getGravityWell() { return gravityWell; }

		Chrono worldClock;
		std::chrono::duration<double> lagTime;
//		int getLagTime() { return (int)lagTime.rep; }

	private:
		bool loadConfiguration(std::string configFile); // Loads external config
		std::string generateBLTConfigString(); // Generates a BearLibTerminal configuration string for terminal_set()
		bool saveGame(std::string fileName); // creates a game save file
		bool loadGame(std::string fileName); // loads a game save file
		// Game objects
		GameParser parser;
		GameGUI gui;
		GameMap meatspace;
		Player player;
		Drone lemur;
		std::list<Actor*> sentientActors; // Actors who will be taking actions
		std::list<GameEvent*> eventList; // event registry
		std::list<CountdownTimer*> timerList; // timer registry
		CountdownTimer *gravityWell;
		// Properties
		uint screenWidth; // Width of terminal in # of monospace chars
		uint screenHeight; // Height of terminal in # of monospace chars
		std::string terminalFontPath; // Contains relative path to the terminal font
		uint terminalFontSize; // Size of the terminal font
		bool cliMode;
};

#endif //SPACEGAME_ENGINE

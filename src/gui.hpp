/*   gui.hpp
DATE Jan 16 2020
AUTH xazak
DESC Implements the GameGUI class, which handles the system input/output and the
     interface, including menu drawing and selection
*/
#ifndef SPACEGAME_GUI
#define SPACEGAME_GUI

#include "main.hpp"
#include "panel.hpp"
#include <string>
#include <vector>
#include <queue>

class GameEngine;
class Actor;
class GameMap;
/* NOTES ON THE GUI SPECIFICATION
The GUI has two main layers: a base layer, where all the windows within it are
assumed to be in the same z-position, and will be tiled together, and a floating
layer, where any windows that appear will be drawn independently of any others.

The base layer is where the game interface will appear; the floating layer is
reserved for event prompts, menus, dialogue boxes, and suchlike.

The user is not allowed to change the window dimensions while the program is
running, though in special cases they may be allowed to rearrange their order.

METHOD
ABSTRACT
-	The programmer outlines which windows shall appear in the base layer
	-	Windows in the base layer have min/(max) dimensional stipulations
-	The programmer may display windows in the floating layer at-will
	-	Windows in the floating layer are much simpler, more often menus

DISPLAY
Given a list of windows which contain min/max dimensions and a list of edges,
	figure out which window is in the upper left,
	calculate its corner positions,
	calculate the corner positions of all the other windows,
	draw the edges of all the windows,
	draw any unshared window corners,
	draw the remaining (shared) window corners.
(Drawing the contents of a window is subordinate to the frame-drawing.)

WINDOW TYPES
BASE
	viewport
		Displays a section of a map object, given a centerpoint and w/h
	messageLog
		Displays the contents of a message log
	readout
		Displays a list of abstracted stat meters, markers, notes, etc
	commandline
		Displays a command line interface prompt

FLOATING
	menu
		Displays a menu with a set of arbitrary options
	dialog
		Displays a prompt with a set of dialog controls (Y/N, meter, etc)
*/

class GameGUI {
	public:
		GameGUI();
		~GameGUI();
		// Initializes a GameGUI to initial state
		void initialize(uint maxWidth, uint maxHeight, GameEngine* enginePtr, Actor* playerPtr, GameMap* meatspacePtr);
		void update(); // Polls the game state for changes in displayed info
		void render(double timeDelta); // Draws the interface onto the screen
		void testBLT(); // BearLibTerminal debugging/test function
		void testMessageLog();
		void testMenu();
		void addMessage(std::string messageText);
		void displayPauseBanner();
		MessageLog* getMessageLog() { return &globalMsgLog; }
		void raiseCLI();
		void hideCLI();
		void toggleCLI();

	protected:
		friend struct Splitter;
		friend struct Viewport;
		friend struct MessageReadout;
		friend struct CommandPrompt;
		friend struct DataDisplay;
		friend struct Vitals;
		
		// METHODS
		void drawFullLayoutTree();
		static void drawHorizontalLine(uint x, uint y, uint length);
		static void drawVerticalLine(uint x, uint y, uint length);
		void drawGUIFrame();
		void drawPlanqIcon();
		static uint getFrameBufferValue(uint x, uint y);
		static void setFrameBufferValue(uint x, uint y, uint value);
		void dump();

		// MEMBERS
		CommandPrompt *cmdPrompt;
		static GameEngine* engine;
		static Actor* avatar;
		static GameMap* worldMap;
		MessageLog globalMsgLog;
		// GUI DIMENSIONS
		// Note that the map display panel consumes any remaining room once the
		// dimensions of the statPanel and msgPanel are deducted from the max
		static uint windowWidth;
		static uint windowHeight;
		uint statPanelWidth;
		uint statPanelHeight;
		uint msgPanelWidth;
		uint msgPanelHeight;
		// Internal geometry specifications, (re-)calculated at init()
		// Stored as a BST, comes with iterators and such (?)
		GUIPanel *layoutRoot;
		DataDisplay *vitals;
		DataDisplay *auxiliary;
		DataDisplay *monitor;
		// Used as a mask for generating the correct GUI frame by taking into
		// account the touching edges of the drawn lines
		// FIXME: add options for other line types
		static char *frameBuffer;
};
#endif //SPACEGAME_GUI

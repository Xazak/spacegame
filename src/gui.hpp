/*   gui.hpp
DATE Jan 16 2020
AUTH xazak
DESC Implements the GameGUI class, which handles the system input/output and the
     interface, including menu drawing and selection
*/
#ifndef SPACEGAME_GUI
#define SPACEGAME_GUI

#include "map.hpp"
#include "actor.hpp"
#include <string>
#include <vector>
#include <queue>

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
struct GUIPanel {
	GUIPanel(uint newID, uint xOrigin = 0, uint yOrigin = 0,
		uint inputMinWidth = 3, uint inputMinHeight = 3,
		uint inputMaxWidth = 0, uint inputMaxHeight = 0,
		GUIPanel *inputLeft = nullptr, GUIPanel *inputRight = nullptr);
	GUIPanel(uint newID, cpair newOrigin,
		uint inputMinWidth = 3, uint inputMinHeight = 3,
		uint inputMaxWidth = 0, uint inputMaxHeight = 0,
		GUIPanel *inputLeft = nullptr, GUIPanel *inputRight = nullptr);
	virtual void display() = 0;
	void setOrigin(uint inputX, uint inputY) { origin.set(inputX, inputY); }
	uint id; // assigned at creation, in order of panel creation
	cpair origin; // Panel's upper-left corner coords w/in terminal
	virtual cpair leftPanelOrigin() { return cpair(0, 0); }
	virtual cpair rightPanelOrigin() { return cpair(0, 0); }
	virtual cpair upPanelOrigin() { return cpair(0, 0); }
	virtual cpair downPanelOrigin() { return cpair(0, 0); }
	uint minWidth;
	uint minHeight;
	// Setting the below values to 0 when initializing the panels will
	// cause the GUI logic to determine a "fill" value for them based
	// on the terminal window dimensions. That is, they do not remain
	// at zero, but will be calculated at runtime.
	// Resizing is NOT supported!
	uint maxWidth;
	uint maxHeight;
	GUIPanel* left;
	GUIPanel* right;
};
struct MessageLog { // semantic wrapper around the message log object
	// An advanced Message object might look like:
	// message text
	// message timestamp (using internal game time)
	// message origin
	// message recipient
	// delivery status
	// Pointers for data retrieval
	std::vector<std::string> messageList;
	int add(std::string newMessage); // Returns # of messages in log
	uint size() { return messageList.size(); }
	// probably need some kind of log culling function(s)
	// NOTE: the message log is structured as a STACK, ie LIFO
};
class GameGUI {
	// FIXME: Do these need to be here? Can specific access be set up?
	friend struct Splitter;
	friend struct Viewport;
	friend struct MessageReadout;
	public:
		GameGUI();
		~GameGUI();
		// Initializes a GameGUI to initial state
		void initialize(uint maxWidth, uint maxHeight, Actor* playerObject, GameMap* mapObject);
		void update(); // Polls the game state for changes in displayed info
		void render(); // Draws the interface onto the screen
		void testBLT(); // BearLibTerminal debugging/test function
		void testMessageLog();
		void addMessage(std::string messageText);
		MessageLog* getMessageLog() { return &globalMsgLog; }

	private:
		struct Splitter : public GUIPanel {
		//struct Splitter {
			// want to specify a percentage OR a character width
			Splitter(uint inputID, uint xOrigin, uint yOrigin, uint inputWidth, uint inputHeight, bool inputType, uint ratioPercent);
			Splitter(uint inputID, cpair inputOrigin, uint inputWidth, uint inputHeight, bool inputType, uint ratioPercent);
			void display();
			cpair leftPanelOrigin();
			cpair rightPanelOrigin();
			cpair upPanelOrigin();
			cpair downPanelOrigin();
			bool verticalSplit; // FALSE = it is a horizontal split
			uint splitRatio; // Defines sizes of split as L:r % ratio
		};
		struct Viewport : public GUIPanel {
			// pointer to map object
			Viewport(uint inputID, cpair inputOrigin, uint inputWidth, uint inputHeight, GameMap* inputSource, cpair mapCenterpoint, Actor* playerPtr);
			GameMap* mapSource;
			cpair focusPoint; // Sets the camera's centerpoint
			Actor* playerObj;
			void display();
		};
		struct MessageReadout : public GUIPanel {
			// displays the contents of a linked message log
			// pointer to message log source
			MessageReadout(uint inputID, cpair inputOrigin, uint inputWidth, uint inputHeight, MessageLog *inputSource);
			MessageLog* logObject;
			void display();
		};
		/*
		struct DataDisplay : public GUIPanel {
			// misc/catch-all category for data visualizations such as stat
			// bars, meters, buttons/switches, etc
			void display();
		};
		*/
		/*
		struct CommandPrompt : public GUIPanel {
			// might elect to make this a floating panel so as to autohide it
		};
		*/
		void drawFullLayoutTree();
		static void drawHorizontalLine(uint x, uint y, int length);
		static void drawVerticalLine(uint x, uint y, int length);
		Actor* avatar;
		GameMap* worldMap;
		MessageLog globalMsgLog;
		uint windowWidth;
		uint windowHeight;
		uint statPanelWidthMinimum;
		uint statPanelHeightMinimum;
		uint msgPanelWidthMinimum;
		uint msgPanelHeightMinimum;
		// Internal geometry specifications, (re-)calculated at init()
		// Stored as a BST, comes with iterators and such
		GUIPanel *layoutRoot;
};
#endif //SPACEGAME_GUI

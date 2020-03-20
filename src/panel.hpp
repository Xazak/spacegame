/*   panel.hpp
DATE Mar 11 2020
AUTH xazak
DESC Implements the GUIPanel class, which describes the set of components that a
	 GameGUI can potentially draw on the screen, including the map viewport,
	 message log, and stat displays
*/

#ifndef SPACEGAME_GUI_PANEL
#define SPACEGAME_GUI_PANEL

#include "main.hpp"
#include <vector>
#include <string>
#include <list>

class GameGUI;
class GameEngine;
class Actor;
class GameMap;
class CountdownTimer;
struct MessageLog { // Message log object
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
struct GUIPanel {
	GUIPanel(uint newID, uint xOrigin = 0, uint yOrigin = 0,
		uint inputWidth = 3, uint inputHeight = 3,
		GUIPanel *inputLeft = nullptr, GUIPanel *inputRight = nullptr);
	GUIPanel(uint newID, cpair newOrigin,
		uint inputWidth = 3, uint inputHeight = 3,
		GUIPanel *inputLeft = nullptr, GUIPanel *inputRight = nullptr);
	virtual void display() = 0;
	void setOrigin(uint inputX, uint inputY) { origin.set(inputX, inputY); }
	uint id; // assigned at creation, in order of panel creation
	cpair origin; // Panel's upper-left corner coords w/in terminal
	virtual cpair leftPanelOrigin() { return cpair(0, 0); }
	virtual cpair rightPanelOrigin() { return cpair(0, 0); }
	virtual cpair upPanelOrigin() { return cpair(0, 0); }
	virtual cpair downPanelOrigin() { return cpair(0, 0); }
	uint width;
	uint height;
	// Setting the below values to 0 when initializing the panels will
	// cause the GUI logic to determine a "fill" value for them based
	// on the terminal window dimensions. That is, they do not remain
	// at zero, but will be calculated at runtime.
	// Resizing is NOT supported!
	GUIPanel* left;
	GUIPanel* right;
	GUIPanel* up;
	GUIPanel* down;
};
struct Splitter : public GUIPanel {
//struct Splitter {
	// want to specify a percentage OR a character width
	Splitter(uint inputID, uint xOrigin, uint yOrigin, uint inputWidth, uint inputHeight, bool inputType);
	Splitter(uint inputID, cpair inputOrigin, uint inputWidth, uint inputHeight, bool inputType);
	void display();
	cpair leftPanelOrigin();
	cpair rightPanelOrigin();
	cpair upPanelOrigin(); // fxnlly equiv to LEFT panel origin
	cpair downPanelOrigin(); // fxnlly equiv to RIGHT panel origin
	bool verticalSplit; // FALSE = this is a horizontal split
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
struct CommandPrompt : public GUIPanel {
	CommandPrompt(uint inputID, cpair inputOrigin, uint inputWidth);
	~CommandPrompt();
	void display();
	std::string promptPrefix;
	bool visible;
	char * inputBuffer;
};
struct DataDisplay : public GUIPanel {
	// misc/catch-all category for data visualizations such as stat
	// bars, meters, buttons/switches, etc
	DataDisplay(uint inputID, cpair inputOrigin, uint inputWidth, uint inputHeight, Actor* inputActor);
	~DataDisplay();
	void addVitals(Actor* playerPtr);
	void addTimer(CountdownTimer* timerPtr);
	Actor* targetActor;
	void display();
	struct DataObject;
	std::list<DataObject*> displayList;
};
struct DataDisplay::DataObject {
	// defines a single data readout method within a DataDisplay
	DataObject();
	DataObject(uint inputHeight);
	DataObject(std::string inputTitle, uint inputHeight = 1);
	std::string titleString; // Leave blank to produce an unbroken ruled line
	uint width; // is set by the parent panel
	uint height; // = 1 by default
	virtual uint display(uint xPos, uint yPos) = 0;
	void displayTitle(uint xPos, uint yPos);
};
struct Vitals : public DataDisplay::DataObject {
	Vitals();
	uint display(uint xPos, uint yPos);
	Actor *target;
};
struct Clock : public DataDisplay::DataObject {
	Clock(CountdownTimer *timerPtr);
	uint display(uint xPos, uint yPos);
	uint drawReadout(uint input[], uint xPos, uint yPos);
	CountdownTimer* localClock;
	// need to define the set of character display methods
	void drawClockDigits(uint value, uint length, uint xPos, uint yPos);
	void drawColon(uint xPos, uint yPos);
	void drawPeriod(uint xPos, uint yPos);
	int* getDigitArray(uint value);
	int digitZero[9];
	int digitOne[9];
	int digitTwo[9];
	int digitThree[9];
	int digitFour[9];
	int digitFive[9];
	int digitSix[9];
	int digitSeven[9];
	int digitEight[9];
	int digitNine[9];
	enum ClockDigitChars {
		// straight
		empty = 0x0020, horizBar = 0x2550, vertBar = 0x2551,
		// corners
		upLeft = 0x2554, upRight = 0x2557, downLeft = 0x255A, downRight = 0x255D,
		// connectors
		leftTee = 0x2560, rightTee = 0x2563, upTee = 0x2566, downTee = 0x2569,
		// serifs
		serifUL = 0x2552, serifUR = 0x2555, serifDL = 0x2558, serifDR = 0x255B,
		serifLT = 0x255E, // middle bar on 3
		serifMT = 0x2562, // middle bar on 7
		serifLU = 0x2553, // upper left on 4
		serifRU = 0x2556, // upper right on 4
		serifLD = 0x2559, // unused
		serifRD = 0x255C, // lower right on 4, 9
		serifCr = 0x256C, // middle cross of 0
	};

};

#endif // SPACEGAME_GUI_PANEL

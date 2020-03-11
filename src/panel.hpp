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

class GameGUI;
class GameEngine;
class Actor;
class GameMap;
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
struct DataDisplay : public GUIPanel {
	// misc/catch-all category for data visualizations such as stat
	// bars, meters, buttons/switches, etc
	DataDisplay(uint inputID, cpair inputOrigin, uint inputWidth, uint inputHeight, Actor* inputActor);
	Actor* targetActor;
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

#endif // SPACEGAME_GUI_PANEL

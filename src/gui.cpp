/*   gui.cpp
DATE Jan 16 2020
AUTH xazak
DESC Contains definitions of the GameGUI class, which displays the game
     interface, including menu drawing and selection
*/

#include "BearLibTerminal.h"
#include "gui.hpp"
#include <iostream>
#include <string>

using namespace std;

// **** GameGUI Methods
/* *** A short guide to BearLibTerminal API - Jan 19 2020 (Zach)
CALLED BY GameEngine: (ie do not call them here!)
BLT CONFIGURATION
int  terminal_open(); // Starts BLT
int  terminal_set(configString); // Configures ALL non-default params of BLT
void terminal_close(); // Stops BLT

INPUT HANDLING
int  terminal_state(int slot); // Returns value of specified state slot:
int  terminal_check(int slot); // Wrapper for _state to assist with type matching
// FIXME: Include a list of state slots?
int  terminal_has_input(); // true if player input is waiting to be processed
int  terminal_read(); // (waits for and) returns next event from input queue
int  terminal_peek(); // returns value of next event on input queue (0 if empty)
color_t terminal_read_str( ... ); // displays user input onscreen (?)

CALLED BY GameGUI:
OUTPUT METHODS
void terminal_clear(); // Clears ALL LAYERS and sets bkcolor of all cells to current color
void terminal_clear_area(int x, int y, int w, int h); // As above, for selection
void terminal_crop(x, y, w, h); // sets a crop area of the current layer (?)
void terminal_refresh(); // Tell BLT to push all layers onto the screen
int  terminal_pick(x, y, int index); // Returns the char code of the specified tile
// NOTE: "index" specifies an index of a tile in the cell
// If no such tile exists in that cell, or the cell is empty, _pick returns 0
// To enumerate all tiles in a cell, incr. index until 0 is returned
color_t terminal_pick_color(x, y, index); // Returns color at specified tile
color_t terminal_pick_bkcolor(x, y); // Returns background color of the cell
dimensions_t terminal_measure(const char* s);
dimensions_t terminal_measure_ext(w, h, s);
// Returns the size a string would have /if/ it were printed to the screen
// _measure_ext works analogous to _put_ext and _print_ext

void terminal_put(x, y, int code); // Put the assoc. char code to those coords
void terminal_put_ext(x, y, dx, dy, int code, color_t* corners);
// Complex version of _put that allows specifying a tile offset as well as
// allowing tile corners to be painted in different colors
// dx and dy specify the tile offset in pixels relative to tile's normal posn
// corners specifies an array of four color_t values in this order:
//		top-left, bottom-left, bottom-right, top-right
// The corners option can be used to smooth gradients and tile transitions
// if NULL is passed as corners param, current foreground color is used

dimensions_t terminal_print(x, y, const char* s);
dimensions_t terminal_print_ext(x, y, width, height, int align, const char* s);
// _print will print a string on the screen from the given coordinates, placed
// as if terminal_put was called (allowing use of standard options)
// _print_ext allows optional auto-wrapping (within its bounds) and alignment:
//	ALIGNMENT VALUES: (combinations allowed)
//	TK_ALIGN_DEFAULT (top left)
//	TK_ALIGN_LEFT
//	TK_ALIGN_RIGHT
//	TK_ALIGN_CENTER (horizontally)
//	TK_ALIGN_TOP
//	TK_ALIGN_BOTTOM
//	TK_ALIGN_CENTER (vertically)
// The string to be printed can be modified inline using [tags], ie:
//	terminal_print("[color=red]^[/color] (red goblin)", dx, dy);
//	TAGS:
//	Color: [color=NAME] (will be parsed by _color_from_name)
//	Arbitrary character codes: [U+E001] or [0xE001], etc
//	Symbol combination: "a[+]^" produces an "a" with a caret accent
//	Pixel offset: [offset=x,y] adds offset to all chars as if by _put_ext
//	Font change: [font=somename]
// All tags can be closed with a symmetrical tag, ie [color] xxx [/color]
// Tag closure is unneccesary, and tag nesting is technically accidental:
// each tag invokes its own call to a set/reset function (NOT LIKE HTML)
// Note that _print and _measure support all flavors of printf, wprint, etc

OUTPUT STATE
void terminal_color(color_t color); // Sets foreground color by 00xAARRGGBB format
void terminal_bkcolor(color_t color); // Sets background color by 00xAARRGGBB format
void terminal_composition(int mode); // Sets character composition mode ON or OFF
void terminal_layer(int layer); // Sets the current layer to draw output upon (0 = lowest and default)

UTILITIES
void terminal_delay(int interval); // Suspend execution for time in milliseconds
color_t color_from_name(name); // returns numeric value of specified color
color_t color_from_argb(A, R, G, B); // as _from_name, but with ARGB values
*/
/* *** The GUI Model Diagram
  |<-     w          ->|       POINT  COORDS
- 0------ ... -4- ... -2         0     0, 0
^ |(0,0)       |       |         1     i, j
| |   MAP      | S     |         2     i, 0
h |            |  T    |         3     0, j
| 5--    ...  -6   A   |         4     (i-18), 0
v |  MESSAGES  |    T  |         5     0, (j-10)
- 3-          -7-     -1         6     ([4].x), ([5].y)
                        (i, j)   7     ([4].x), j
BOUNDING LIMITS
The map's minimum size is 20x20
The viewport's minimum size is 40x20 (to allow 40 width for message output)
The stat panel's minimum width is 18
The terminal window's minimum size is 60x42
The maximum width and height are supplied by GameEngine on GUI creation

LAYER MODEL
NOTE: clear_area() affects CURRENT layer, clear() affects ALL layers!
NOTE: Only layer 0 has a background color! All other layers have transp. bkgrnd
255
...
09	GUI Elements
08	SFX
07	SFX
06	SFX
05	Actor (Upper)
04	Actor (Middle)
03	Actor (Lower)
02	Map Terrain
01	(Reserved)
00	BACKGROUND - Reserved

Each panel within the GUi will have its dimensions calculated when the game is loaded.
This class will define some sane minimums in case the defined/calculated values are wrongly-sized.
*/
GameGUI::GameGUI() :
statPanelWidthMinimum(36),
msgPanelWidthMinimum(40),
msgPanelHeightMinimum(10)
{	// default constructor

}
GameGUI::~GameGUI() {
	// default destructor

}
//void GameGUI::initialize(uint maxWidth, uint maxHeight) {
//void GameGUI::initialize(uint maxWidth, uint maxHeight, Actor* playerPtr) {
void GameGUI::initialize(uint maxWidth, uint maxHeight, Actor* playerPtr, GameMap* meatspacePtr) {
	// Sets up a created GameGUI object to the runtime default configuration
	// Obtain pointers to the game objects we want to display
	avatar = playerPtr;
	worldMap = meatspacePtr;
	LOGMSG("Player located at: " << avatar);
	LOGMSG("Meatspace located at: " << worldMap);
	// Assign the maximum parameters
	windowWidth = maxWidth;
	windowHeight = maxHeight;
	statPanelHeightMinimum = (windowHeight * 33) / 100;
	globalMsgLog.add("Press Q or Alt+F4 to exit.");
	// drawing method
	//						split:	LU/RD ratio%, field width, field height
	//							+	id#, type, origin
	//						panel:	origin point, panel width, panel height
	// split root into L/R:			75/25, maxWidth, maxHeight
	layoutRoot = new Splitter(0, 0, 0, maxWidth, maxHeight, true, 75);
	GUIPanel* layoutIndex = layoutRoot;
	// split root->L into U/D		75/25, (mW - statpanel), maxHeight)
	layoutIndex->left = new Splitter(1, layoutIndex->leftPanelOrigin(), layoutIndex->rightPanelOrigin().x, maxHeight, false, 75);
	layoutIndex = layoutIndex->left;
	// put MAP at root->L->U		o: r->L->U, mW-statpanel, mH
	layoutIndex->left = new Viewport(4, layoutIndex->leftPanelOrigin(), layoutRoot->rightPanelOrigin().x, layoutIndex->rightPanelOrigin().y, worldMap, avatar->location, avatar);
	// put MSG at root->L->D		o: r->L->D, mW-statpanel, msgHeight
	layoutIndex->right = new MessageReadout(5, layoutIndex->rightPanelOrigin(), layoutRoot->rightPanelOrigin().x, (maxHeight - layoutIndex->rightPanelOrigin().y), &globalMsgLog);
	// split root->R into U/D		33/67, statpanel, maxHeight
	layoutIndex = layoutRoot;
	layoutIndex->right = new Splitter(2, layoutIndex->rightPanelOrigin(), statPanelWidthMinimum, maxHeight, false, 33);
	layoutIndex = layoutIndex->right;
	// put VIT at root->R->U		o: r->R->U, statpanel, maxHeight * 33%
//	layoutIndex->left = new DataDisplay();
	// split root->R->D into U/D	50/50, statpanel, maxHeight * 33%
	layoutIndex->right = new Splitter(3, layoutIndex->rightPanelOrigin(), statPanelWidthMinimum, (maxHeight * 67 / 100), false, 50);
	layoutIndex = layoutIndex->right;
	// put AUX at root->R->D->U		o: r->R->D->U, statpanel, maxHeight * 33%
//	layoutIndex->left = new DataDisplay();
	// put MTR at root->R->D->D		o: r->R->D->D, statpanel, maxHeight * 33%
//	layoutIndex->right = new DataDisplay();
}
void GameGUI::update() {
	// polls game state to see if any of the GUI elements need to change
	// performs scene composition

}
void GameGUI::render() {
	// draws the interface onto the screen
	// -- can use terminal_crop to set scene/layer sizes?
	// -- need some helper funcs to wrap around BLT's print funcs
	// -- need some line-drawing methods
	terminal_bkcolor("black"); // Set the background color
	terminal_clear(); // Wipe everything off
	drawFullLayoutTree();
	terminal_refresh(); // Tell BLT to go ahead and update the display
}
/****	Box-Drawing Char Unicode Codepoints
		topLeft		= 0x250C,
		topRight	= 0x2510,
		bottomLeft	= 0x2514,
		bottomRight	= 0x2518,
		hLine		= 0x2500,
		vLine		= 0x2502,
		leftT		= 0x251C,
		rightT		= 0x2524,
		upperT		= 0x252C,
		lowerT		= 0x2534,
		cross		= 0x253C
		*/
void GameGUI::drawFullLayoutTree() {
	// Traverses the UI layout tree and calls display() on all nodes
	// The tree is traversed in reversed breadth-order: this produces all leaf
	// nodes and then all splitters, in that order.
	GUIPanel* layoutIter = layoutRoot;
	queue<GUIPanel*> drawOrder;
	drawOrder.push(layoutIter);
	while (!drawOrder.empty()) {
		terminal_color("light grey");
		layoutIter = drawOrder.front();
		drawOrder.pop();
		layoutIter->display();
		if (layoutIter->left != nullptr) drawOrder.push(layoutIter->left);
		if (layoutIter->right != nullptr) drawOrder.push(layoutIter->right);
	}
}
// **** WINDOW-DRAWING METHODS
void GameGUI::drawHorizontalLine(unsigned int x, unsigned int y, int length) {
	// Draws a horizontal line from the specified point
	// Specifying a negative length will draw the line 'backwards'
	// If length == 0, it will fall through and do nothing
	terminal_layer(9);
	if (length > 0) {
		for (int offset = 0; offset < length; offset++) {
			terminal_put((x + offset), y, 0x2500);
		}
	} else if (length < 0) {
		for (int offset = 0; offset < length; offset--) {
			terminal_put((x + offset), y, 0x2500);
		}
	}
}
void GameGUI::drawVerticalLine(unsigned int x, unsigned int y, int length) {
	// Draws a vertical line from the specified point
	// Specifying a negative length will draw the line 'backwards'
	// If length == 0, it will fall through and do nothing
	terminal_layer(9);
	if (length > 0) {
		for (int offset = 0; offset < length; offset++) {
			terminal_put(x, (y + offset), 0x2502);
		}
	} else if (length < 0) {
		for (int offset = 0; offset < length; offset--) {
			terminal_put(x, (y + offset), 0x2502);
		}
	}
}
void GameGUI::testBLT() {
	// Debugging function to assist with checking BearLibTerminal functions
	// try printing something to the screen and wait until the window is closed
	terminal_print(1, 1, "Press Q or Alt+F4 to exit.");
	terminal_refresh();
}
void GameGUI::testMessageLog() {
	// Test function for the message log
	string testMessage = "Log size: ";
	testMessage.append(to_string(globalMsgLog.size() + 1));
	testMessage.append("\n");
	globalMsgLog.add(testMessage);
//	globalMsgLog.add("Press Q or Alt+F4 to exit.");
//	globalMsgLog.add("Or don't, it's not my job to tell you what to do.");

}
// **** MessageLog Methods
int GameGUI::MessageLog::add(string newMessage) {
	// Adds the input string to the message log list
	messageList.push_back(newMessage);
	return messageList.size();
}
// **** GUIPanel Methods
// **** GUIPanel Prototypes
GUIPanel::GUIPanel(uint newID, uint xOrigin, uint yOrigin,
	uint inputMinWidth, uint inputMinHeight,
	uint inputMaxWidth, uint inputMaxHeight,
	GUIPanel *inputLeft, GUIPanel *inputRight) :
id(newID),
origin(xOrigin, yOrigin),
minWidth(inputMinWidth),
minHeight(inputMinHeight),
maxWidth(inputMaxWidth),
maxHeight(inputMaxHeight),
left(inputLeft),
right(inputRight)
{	}
GUIPanel::GUIPanel(uint newID, cpair newOrigin,
	uint inputMinWidth, uint inputMinHeight,
	uint inputMaxWidth, uint inputMaxHeight,
	GUIPanel *inputLeft, GUIPanel *inputRight) :
id(newID),
origin(newOrigin),
minWidth(inputMinWidth),
minHeight(inputMinHeight),
maxWidth(inputMaxWidth),
maxHeight(inputMaxHeight),
left(inputLeft),
right(inputRight)
{	}
// **** SPLITTER
GameGUI::Splitter::Splitter(uint inputID, uint xOrigin, uint yOrigin, uint inputWidth, uint inputHeight, bool inputType, uint ratioPercent) :
GUIPanel(inputID, xOrigin, yOrigin, inputWidth, inputHeight),
verticalSplit(inputType),
splitRatio(ratioPercent)
{	
	LOGMSG("Created:" << inputID << ": " << xOrigin << ", " << yOrigin << " d:" << inputWidth << "x" << inputHeight << " @ " << ratioPercent << "%");
}
GameGUI::Splitter::Splitter(uint inputID, cpair inputOrigin, uint inputWidth, uint inputHeight, bool inputType, uint ratioPercent) :
GUIPanel(inputID, inputOrigin, inputWidth, inputHeight),
verticalSplit(inputType),
splitRatio(ratioPercent)
{	
	LOGMSG("Created:" << inputID << ": " << inputOrigin.x << ", " << inputOrigin.y << " d:" << inputWidth << "x" << inputHeight << " @ " << ratioPercent << "%");
}
void GameGUI::Splitter::display() {
	// Draw a line
//	LOGMSG(" (called) ");
	if (verticalSplit) {
//		terminal_color("blue");
		uint xOffset = ( this->minWidth * this->splitRatio ) / 100;
		drawVerticalLine(this->origin.x + xOffset, this->origin.y, this->minWidth);
	} else {
//		terminal_color("red");
		uint yOffset = ( this->minHeight * this->splitRatio ) / 100;
		drawHorizontalLine(this->origin.x, this->origin.y + yOffset, this->minWidth);
	}
}
cpair GameGUI::Splitter::leftPanelOrigin() {
//	LOGMSG("value: " << this->origin.x << ", " << this->origin.y);
	return this->origin;
}
cpair GameGUI::Splitter::rightPanelOrigin() {
	cpair offsetOrigin = this->origin;
	if (this->verticalSplit) {
		uint xOffset = ( this->minWidth * this->splitRatio ) / 100;
		offsetOrigin.x += xOffset + 1;
	} else {
		uint yOffset = ( this->minHeight * this->splitRatio ) / 100;
		offsetOrigin.y += yOffset + 1;
	}
//	LOGMSG("value: " << offsetOrigin.x << ", " << offsetOrigin.y);
	return offsetOrigin;
}
cpair GameGUI::Splitter::upPanelOrigin() {
	return this->leftPanelOrigin();
}
cpair GameGUI::Splitter::downPanelOrigin() {
	return this->rightPanelOrigin();
}
// **** VIEWPORT
GameGUI::Viewport::Viewport(uint inputID, cpair inputOrigin, uint inputWidth, uint inputHeight, GameMap* inputSource, cpair mapCenterpoint, Actor* playerPtr) :
GUIPanel(inputID, inputOrigin, inputWidth, inputHeight),
mapSource(inputSource),
focusPoint(mapCenterpoint),
playerObj(playerPtr)
{	}
/*  void GameGUI::displayMap() {
	// Display the currently-explored map
	// Get the size of the map
	uint mapWidth = worldMap->getWidth();
	uint mapHeight = worldMap->getHeight();
//	uint mapSize = worldMap->getSize();
//	LOGMSG(mapWidth << " " << mapHeight << " " << mapSize);
//	uint mapWidth = 30;
//	uint mapHeight = 30;
	// Figure out the viewport offsets and the initial cursor position
	uint mapViewHorizontalOffset = 0;
	uint mapViewVerticalOffset = 0;
	if (mapWidth < mapViewportWidth) {
		mapViewHorizontalOffset = (mapDisplay.width - mapWidth) / 2;
	}
	if (mapHeight < mapViewportHeight) {
		mapViewVerticalOffset = (mapDisplay.height - mapHeight) / 2;
	}
	uint cursorXOrigin = mapViewHorizontalOffset + mapDisplay.xOrigin;
	uint cursorYOrigin = mapViewVerticalOffset + mapDisplay.yOrigin;
//	LOGMSG("Cursor origin coords: " << cursorXOrigin << ", " << cursorYOrigin);
	terminal_layer(0); // Move to the background layer
	terminal_color("black"); // Set the default background color
	// Display the map (test pattern)
	for (uint xIndex = 0; xIndex < mapWidth; xIndex++) {
		for (uint yIndex = 0; yIndex < mapHeight; yIndex++) {
			// Set the tile background
			terminal_layer(0);
			// Use test colors and glyphs for now until iterator is finished
			uint tileColor = 0x22FF00FF;
			terminal_bkcolor(tileColor);
			terminal_color(tileColor);
			terminal_put(cursorXOrigin + xIndex, cursorYOrigin + yIndex, ' ');
//			LOGMSG(cursorXOrigin + xIndex << ", " << cursorYOrigin + yIndex);
			// Draw the terrain symbols
			terminal_layer(2);
			tileColor += 0x55000000;
			terminal_color(tileColor);
			terminal_put(cursorXOrigin + xIndex, cursorYOrigin + yIndex, '.');
		}
	}
	// Display the game map
	// give me an iterator to the map's tile array
	// iterating across all tiles in the map:
		// get the tile's background color
		// set the background color (helper func for speeding up call)
		// get the tile's foreground color
		// draw the tile's glyph
		// if at the end of a map row, move down and start a new row
	// Display the game entities within the map
	terminal_layer(4);
	terminal_color("light blue");
	// The player's avatar
	terminal_put(cursorXOrigin + avatar->location.x, cursorYOrigin + avatar->location.y, '@');
}
*/
void GameGUI::Viewport::display() {
	// Display the currently-explored map
	terminal_layer(2);// Move to the Terrain layer
	// Get the size of the map
	uint mapWidth = mapSource->getWidth();
	uint mapHeight = mapSource->getHeight();
	int mapViewHorizontalOffset = (this->minWidth - mapWidth) / 2;
	int mapViewVerticalOffset = (this->minHeight - mapHeight) / 2;
	int cursorXPosition = mapViewHorizontalOffset + this->origin.x;
	int cursorYPosition = mapViewVerticalOffset + this->origin.y;
//	LOGMSG("cursor loc: " << cursorXPosition << ", " << cursorYPosition);
	// Display a test pattern for now
	terminal_color("darker green");
	for (uint echs = 0; echs < mapWidth; echs++) {
		for (uint whye = 0; whye < mapHeight; whye++) {
			terminal_put(cursorXPosition + echs, cursorYPosition + whye, '+');
		}
	}
//	terminal_color(playerObject->getColor());
	terminal_color("light blue");
	terminal_put(cursorXPosition + playerObj->location.x, cursorYPosition + playerObj->location.y, playerObj->getSigil());
}
// **** MESSAGE READOUT
GameGUI::MessageReadout::MessageReadout(uint inputID, cpair inputOrigin, uint inputWidth, uint inputHeight, MessageLog *inputSource) :
GUIPanel(inputID, inputOrigin, inputWidth, inputHeight),
logObject(inputSource)
{	}
void GameGUI::MessageReadout::display() {
	// Open a message log and display its contents within the window
	// Obtain the starting position and set some defaults
	int cursorXPosition = this->origin.x;
	int cursorYPosition = this->origin.y;
	terminal_color("white"); // Default text color, can be overridden inline
	if (this->logObject->messageList.size() > 0) {
		vector<string>::reverse_iterator msgLogIter = this->logObject->messageList.rbegin();
		for ( ; msgLogIter != this->logObject->messageList.rend(); msgLogIter++) {
			terminal_print(cursorXPosition, cursorYPosition++, (*msgLogIter).c_str());
		}
	}
}

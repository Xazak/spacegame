/*   gui.cpp
DATE Jan 16 2020
AUTH xazak
DESC Contains definitions of the GameGUI class, which displays the game
     interface, including menu drawing and selection
*/

#include "BearLibTerminal.h"
#include "gui.hpp"
#include "panel.hpp"
#include "engine.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "main.hpp"
#include <iostream>
#include <string>

using namespace std;

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

enum myBoxDrawChars {
    topLeft = 0x250C,
    topRight = 0x2510,
    bottomLeft = 0x2514,
    bottomRight = 0x2518,
    hLine = 0x2500,
    vLine = 0x2502,
    leftT = 0x251C,
    rightT = 0x2524,
    upperT = 0x252C,
    lowerT = 0x2534,
    cross = 0x253C,
    space = 0x0020,
};
wchar_t myBoxDrawArray[] = {
    // 00-11
    space,
    space,
    space,
    bottomLeft,
    // 100-111
    space,
    vLine,
    topLeft,
    leftT,
    // 1000-1011
    space,
    bottomRight,
    hLine,
    lowerT,
    // 1100-1111
    topRight,
    rightT,
    upperT,
    cross,
};
// **** STATICS
GameEngine* GameGUI::engine = nullptr;
Actor* GameGUI::avatar = nullptr;
GameMap* GameGUI::worldMap = nullptr;
uint GameGUI::windowWidth = 0;
uint GameGUI::windowHeight = 0;
char* GameGUI::frameBuffer = nullptr;

// **** GAMEGUI
GameGUI::GameGUI() :
statPanelWidth(36),
statPanelHeight(13), // includes +1 for the splitter edge (? FIXME ?)
msgPanelWidth(0), // same as the map display
msgPanelHeight(10)
{	// default constructor
}
GameGUI::~GameGUI() {
	// default destructor
//	delete cmdPrompt;
	delete [] frameBuffer;
}
void GameGUI::initialize(uint maxWidth, uint maxHeight, GameEngine* enginePtr, Actor* playerPtr, GameMap* meatspacePtr) {
	// Sets up a created GameGUI object to the runtime default configuration
	// Obtain pointers to the game objects we want to display
	GameGUI::engine = enginePtr;
	avatar = playerPtr;
	worldMap = meatspacePtr;
//	LOGMSG("Player located at: " << avatar);
//	LOGMSG("Meatspace located at: " << worldMap);
	// Assign the maximum parameters
	windowWidth = maxWidth;
	windowHeight = maxHeight;
	msgPanelWidth = maxWidth - statPanelWidth;
	GameGUI::frameBuffer = new char[(windowWidth + 1) * (windowHeight + 1)];
	for (uint foo = 0; foo < (windowWidth * windowHeight); foo++) {
		GameGUI::frameBuffer[foo] = 0;
	}
	globalMsgLog.add("Press Q or Alt+F4 to exit.");
	// SPLITTERS have IDs from 0 - 9
	// ALL OTHER PANELS have IDs from 10+
	// drawing method
	//						split:	LU/RD ratio%, field width, field height
	//							+	id#, type, origin
	//						panel:	origin point, panel width, panel height
	// split root into L/R:			75/25, maxWidth, maxHeight
	layoutRoot = new Splitter(1, 0, 0, msgPanelWidth, windowHeight, true);
	GUIPanel* layoutIndex = layoutRoot;
	// split root->L into U/D		75/25, (mW - statpanel), maxHeight)
	layoutIndex->left = new Splitter(2, layoutIndex->leftPanelOrigin(), msgPanelWidth, (windowHeight - msgPanelHeight), false);
	layoutIndex = layoutIndex->left;
	// put MAP at root->L->U		o: r->L->U, mW-statpanel, mH
	layoutIndex->up = new Viewport(11, layoutIndex->upPanelOrigin(), msgPanelWidth, (windowHeight - msgPanelHeight), worldMap, avatar->location, avatar);
	// put MSG at root->L->D		o: r->L->D, mW-statpanel, msgHeight
	layoutIndex->down = new MessageReadout(12, layoutIndex->downPanelOrigin(), msgPanelWidth, msgPanelHeight, &globalMsgLog);
	// grab the position where we'll spawn the CLI at
	int msgBoxYOrigin = layoutIndex->down->origin.y;
	// split root->R into U/D		33/67, statpanel, maxHeight
	layoutIndex = layoutRoot;
	layoutIndex->right = new Splitter(3, layoutIndex->rightPanelOrigin(), statPanelWidth, statPanelHeight, false);
	// put VIT at root->R->U		o: r->R->U, statpanel, maxHeight * 33%
	layoutIndex = layoutIndex->right;
	layoutIndex->up = new DataDisplay(13, layoutIndex->upPanelOrigin(), statPanelWidth, statPanelHeight, avatar);
	vitals = dynamic_cast<DataDisplay *>(layoutIndex->up);
	vitals->addVitals(avatar);
	// split root->R->D into U/D	50/50, statpanel, maxHeight * 33%
	layoutIndex->down = new Splitter(4, layoutIndex->downPanelOrigin(), statPanelWidth, statPanelHeight, false);
//	LOGMSG(layoutIndex << " has up: " << layoutIndex->up << " and down: " << layoutIndex->down);
	layoutIndex = layoutIndex->down;
	// put AUX at root->R->D->U		o: r->R->D->U, statpanel, maxHeight * 33%
	layoutIndex->up = new DataDisplay(14, layoutIndex->upPanelOrigin(), statPanelWidth, statPanelHeight, avatar);
	auxiliary = dynamic_cast<DataDisplay *>(layoutIndex->up);
	auxiliary->addTimer(engine->getGravityWell());
	// put MTR at root->R->D->D		o: r->R->D->D, statpanel, maxHeight * 33%
	layoutIndex->down = new DataDisplay(15, layoutIndex->downPanelOrigin(), statPanelWidth, statPanelHeight, avatar);
	monitor = dynamic_cast<DataDisplay *>(layoutIndex->down);
	// FIXME: add stuff to the monitor
	// The command prompt is drawn specially over the top the UI, is therefore
	// not part of the base layout tree
	cmdPrompt = new CommandPrompt(10, cpair(0, msgBoxYOrigin + msgPanelHeight - 2), msgPanelWidth);
//	LOGMSG("+ Requesting dump +");
//	this->dump(); // DEBUG
}
void GameGUI::update() {
	// polls game state to see if any of the GUI elements need to change
	// performs scene composition

}
void GameGUI::render(double timeDelta) {
	// draws the interface onto the screen
	// -- can use terminal_crop to set scene/layer sizes?
	// -- need some helper funcs to wrap around BLT's print funcs
	// -- need some line-drawing methods
	terminal_bkcolor("black"); // Set the background color
	terminal_clear(); // Wipe everything off
	drawFullLayoutTree();
	if (engine->currMode == GameEngine::EngineState::PAUSED) displayPauseBanner();
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
//	LOGMSG("+ Requesting dump +");
//	dump();
	GUIPanel* layoutIter = layoutRoot;
	queue<GUIPanel*> drawOrder;
	drawOrder.push(layoutIter);
//	LOGMSG("Redrawing layout tree...");
	terminal_color("light grey");
	while (!drawOrder.empty()) {
		layoutIter = drawOrder.front();
		drawOrder.pop();
//		LOGMSG("layoutIter: " << layoutIter);
		layoutIter->display();
//		LOGMSG("panel #" << layoutIter->id << " (" << layoutIter << ") :" );
//		clog << "    left: " << layoutIter->left << endl;
//		clog << "      up: " << layoutIter->up << endl;
//		clog << "   right: " << layoutIter->right << endl;
//		clog << "    down: " << layoutIter->down << endl;
		if (layoutIter->left != nullptr) drawOrder.push(layoutIter->left);
		if (layoutIter->up != nullptr) drawOrder.push(layoutIter->up);
		if (layoutIter->right != nullptr) drawOrder.push(layoutIter->right);
		if (layoutIter->down != nullptr) drawOrder.push(layoutIter->down);
	}
	// Add some edges to the UI frame
	// Top horizontal edge
	drawHorizontalLine(0, 0, windowWidth);
	// Left vertical edge
	drawVerticalLine(0, 0, windowHeight);
	// Right vertical edge
	drawVerticalLine(windowWidth - 1, 0, windowHeight);
	// Bottom edge under right-side data display
	drawHorizontalLine((windowWidth - statPanelWidth), windowHeight - 1, windowWidth);
	// Draw the PLANQ indicator icon
	drawPlanqIcon();
	// Draw the chrome and the CLI if needed
	drawGUIFrame();
	if (cmdPrompt->visible) cmdPrompt->display();
	// NEW METHOD
	// - splitters (and other GUI chromes) draw on a 'template' array
	// - the GUI takes the template array and uses it to render the GUI frame
	// - the non-splitter modules are asked to display their contents
	/*	0	(enter drawing loop)
		1	if the panel-to-be-drawn is a splitter, draw it
		2	if the panel is NOT a splitter, skip it
		3	when all of the splitters are out of the queue/have been drawn...
		4	... use the splitter->template array to draw the GUI frame
		5	reiterate the queue and draw all of the non-splitter panels
	*/
}
// **** WINDOW-DRAWING METHODS
void GameGUI::drawHorizontalLine(uint x, uint y, uint length) {
	// Draws a horizontal line from the specified point
	// Specifying a negative length will draw the line 'backwards'
	// If length == 0, it will fall through and do nothing
	for (uint offset = 0; offset <= length; offset++) {
		setFrameBufferValue((x + offset), y, 1);
	}
//	LOGMSG("Drawing Hline @ " << x << ", " << y << " of length " << length);
}
void GameGUI::drawVerticalLine(uint x, uint y, uint length) {
	// Draws a vertical line from the specified point
	// Specifying a negative length will draw the line 'backwards'
	// If length == 0, it will fall through and do nothing
	for (uint offset = 0; offset <= length; offset++) {
		setFrameBufferValue(x, (y + offset), 1);
	}
//	LOGMSG("Drawing Vline @ " << x << ", " << y << " of length " << length);
}
void GameGUI::drawGUIFrame() {
	// REQUIRES that the frameBuffer array is already populated!
	terminal_color(0xFF336600);
	terminal_layer(10);
	int c = 0;
	uint xIndex = 0;
	uint yIndex = 0;
	for (xIndex = 0; xIndex < windowWidth; xIndex++) {
		for (yIndex = 0; yIndex < windowHeight; yIndex++) {
			if (frameBuffer[xIndex + yIndex * windowWidth]) {
//				c = (get(x - 1, y) << 3) | (get(x, y + 1) << 2) | (get(x + 1, y) << 1) | get(x, y - 1);
				c = (getFrameBufferValue(xIndex - 1, yIndex) << 3) |
					(getFrameBufferValue(xIndex, yIndex + 1) << 2) |
					(getFrameBufferValue(xIndex + 1, yIndex) << 1) |
					getFrameBufferValue(xIndex, yIndex - 1);
				terminal_put(xIndex, yIndex, myBoxDrawArray[c]);
			}
		}
	}
}
void GameGUI::drawPlanqIcon() {
	// Draw the PLANQ icon
	// Bottom corner to left of CLI
	terminal_layer(0);
	terminal_color("black");
	terminal_bkcolor(0xFF333333);
	terminal_put(0, windowHeight - 1, ' ');
	terminal_layer(11);
//	terminal_color("red");
	terminal_put(0, windowHeight - 1, 0x00B6);

}
uint GameGUI::getFrameBufferValue(uint x, uint y) {
	if (x < 0 || y < 0 || x >= windowWidth || y >= windowHeight) return 0;
	return frameBuffer[x + y * windowWidth];
}
void GameGUI::setFrameBufferValue(uint x, uint y, uint value) {
	if (x < 0 || y < 0 || x >= windowWidth || y >= windowHeight) return;
	frameBuffer[x + y * windowWidth] = value;
}
void GameGUI::dump() {
	GUIPanel* layoutIter = layoutRoot;
	queue<GUIPanel*> writeOrder;
	writeOrder.push(layoutIter);
	LOGMSG("Dumping layout tree details:");
	while (!writeOrder.empty()) {
		layoutIter = writeOrder.front();
		writeOrder.pop();
		clog << "    #";
		if (layoutIter->id < 10) clog << "0";
		clog << layoutIter->id << "(" << layoutIter << ") at " << layoutIter->origin << "  	d: ";
		clog << layoutIter->width << "x" << layoutIter->height << endl;
		if (layoutIter->left != nullptr) writeOrder.push(layoutIter->left);
		if (layoutIter->up != nullptr) writeOrder.push(layoutIter->up);
		if (layoutIter->right != nullptr) writeOrder.push(layoutIter->right);
		if (layoutIter->down != nullptr) writeOrder.push(layoutIter->down);
	}
	clog << "    (Skipping command prompt)" << endl;
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
void GameGUI::testMenu() {
	// draws a test menu on top of the game screen
//	uint xOrigin = 5;
//	uint yOrigin = 5;
	// METHOD FOR DRAWING A MENU
	// move to terminal_layer(menu)
	// draw a black box at x, y to w, h
	// draw a white box outline at the box edge
	// OPTIONAL: write the menu message banner
	// write the menu options
	// display the menu selector
}
void GameGUI::addMessage(string messageText) {
	globalMsgLog.add(messageText);
}
void GameGUI::displayPauseBanner() {
	// Draws a banner across the screen to indicate that the game is paused
	terminal_layer(10);
	terminal_color(0xFFFF0000);
	uint xOrigin = 0;
	uint yOrigin = (windowHeight / 3) * 2;
	string pauseMessage = "/- PAUSED -/";
	for (uint xOffset = 0; xOffset < windowWidth; (xOffset += pauseMessage.length()) ) {
		terminal_printf((xOrigin + xOffset), yOrigin, pauseMessage.c_str());
	}
}
void GameGUI::raiseCLI() {
	this->cmdPrompt->visible = true;
}
void GameGUI::hideCLI() {
	this->cmdPrompt->visible = false;
}
void GameGUI::toggleCLI() {
	this->cmdPrompt->visible = !this->cmdPrompt->visible;
}

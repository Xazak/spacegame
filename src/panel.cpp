/*   panel.cpp
DATE Mar 11 2020
AUTH xazak
DESC Implements the GUIPanel class, which describes the set of components that a
	 GameGUI can potentially draw on the screen, including the map viewport,
	 message log, and stat displays
*/

#include "BearLibTerminal.h"
#include "panel.hpp"
#include "main.hpp"
#include "gui.hpp"
#include "actor.hpp"
#include "engine.hpp"
#include "map.hpp"

using namespace std;

// *************
// **** GUIPanel
GUIPanel::GUIPanel(uint newID, uint xOrigin, uint yOrigin,
	uint inputWidth, uint inputHeight,
	GUIPanel *inputLeft, GUIPanel *inputRight) :
	id(newID), origin(xOrigin, yOrigin), width(inputWidth), height(inputHeight),
	left(inputLeft), right(inputRight), up(inputLeft), down(inputRight)
{	
//	LOGMSG("A- Created #" << newID << "(" << this << "): " << xOrigin << ", " << yOrigin << " d:" << inputWidth << "x" << inputHeight);
//	LOGMSG("Pointers: (" << this << ")" );
//	clog << "    left: " << this->left << endl;
//	clog << "      up: " << this->up << endl;
//	clog << "   right: " << this->right << endl;
//	clog << "    down: " << this->down << endl;
}
GUIPanel::GUIPanel(uint newID, cpair newOrigin, uint inputWidth, uint inputHeight,
	GUIPanel *inputLeft, GUIPanel *inputRight) :
	id(newID), origin(newOrigin), width(inputWidth), height(inputHeight),
	left(inputLeft), right(inputRight), up(inputLeft), down(inputRight)
{	
//	LOGMSG("B- Created #" << newID << "(" << this << "): " << origin << " d:" << inputWidth << "x" << inputHeight);
//	LOGMSG("Pointers: (" << this << ")" );
//	clog << "    left: " << this->left << endl;
//	clog << "      up: " << this->up << endl;
//	clog << "   right: " << this->right << endl;
//	clog << "    down: " << this->down << endl;
}
// *************
// **** SPLITTER
Splitter::Splitter(uint inputID, uint xOrigin, uint yOrigin, uint inputWidth, uint inputHeight, bool inputType) :
	GUIPanel(inputID, xOrigin, yOrigin, inputWidth, inputHeight),
	verticalSplit(inputType)
{	}
Splitter::Splitter(uint inputID, cpair inputOrigin, uint inputWidth, uint inputHeight, bool inputType) :
	GUIPanel(inputID, inputOrigin, inputWidth, inputHeight),
	verticalSplit(inputType)
{	}
void Splitter::display() {
	// Draw a line
	terminal_layer(9);
	if (verticalSplit) {
//		terminal_color("blue");
		GameGUI::drawVerticalLine(this->origin.x + this->width, this->origin.y, this->height);
//		if (this->id == 4) LOGMSG("V#" << this->id << " called: " << this->width << ", " << this->origin.y << ": " << this->height);
	} else {
//		if (this->id == 4) terminal_color("red");
		GameGUI::drawHorizontalLine(this->origin.x, this->origin.y + this->height, this->width);
	}
}
cpair Splitter::leftPanelOrigin() {
//	LOGMSG("leftPanel origin: " << this->origin);
	return this->origin;
}
cpair Splitter::rightPanelOrigin() {
	cpair offsetOrigin = this->origin;
	if (this->verticalSplit) {
		uint xOffset = this->width;
		offsetOrigin.x += xOffset + 1;
	} else {
		uint yOffset = this->height;
		offsetOrigin.y += yOffset + 1;
	}
//	LOGMSG("rightPanel origin: " << offsetOrigin);
	return offsetOrigin;
}
cpair Splitter::upPanelOrigin() {
	return this->leftPanelOrigin();
}
cpair Splitter::downPanelOrigin() {
	return this->rightPanelOrigin();
}
// *************
// **** VIEWPORT
Viewport::Viewport(uint inputID, cpair inputOrigin, uint inputWidth, uint inputHeight, GameMap* inputSource, cpair mapCenterpoint, Actor* playerPtr) :
	GUIPanel(inputID, inputOrigin, inputWidth, inputHeight),
	mapSource(inputSource),
	focusPoint(mapCenterpoint),
	playerObj(playerPtr)
{	}
/*  COMMENTS: void GameGUI::displayMap() {
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
void Viewport::display() {
	// Display the currently-explored map
	// Get the size of the map
	uint mapWidth = mapSource->getWidth();
	uint mapHeight = mapSource->getHeight();
	int mapViewHorizontalOffset = (this->width - mapWidth) / 2;
	int mapViewVerticalOffset = (this->height - mapHeight) / 2;
	int cursorXOrigin = mapViewHorizontalOffset + this->origin.x;
	int cursorYOrigin = mapViewVerticalOffset + this->origin.y;
	int currentXPos = 0;
	int currentYPos = 0;
//	LOGMSG("cursor loc: " << cursorXPosition << ", " << cursorYPosition);
	// Paint the background terrain
	for (uint echs = 0; echs < mapWidth; echs++) {
		for (uint whye = 0; whye < mapHeight; whye++) {
			// Calculate the new cursor position
			currentXPos = cursorXOrigin + echs;
			currentYPos = cursorYOrigin + whye;
			// Paint the background
			terminal_bkcolor(mapSource->getTileBkcolor(echs, whye));
			terminal_layer(0);
			terminal_put(currentXPos, currentYPos, 0x0020);
			// Paint the terrain layer
			terminal_layer(2);
			terminal_color(mapSource->getTileColor(echs, whye));
			terminal_put(currentXPos, currentYPos, mapSource->getTileSigil(echs, whye));
		}
	}
	// Paint the local furniture
	terminal_layer(3);
	for (list<Actor*>::iterator furnishIter = mapSource->furnishings.begin(); furnishIter != mapSource->furnishings.end(); furnishIter++) {
		terminal_color((*furnishIter)->getColor());
		terminal_put(cursorXOrigin + (*furnishIter)->getLocation().x, cursorYOrigin + (*furnishIter)->getLocation().y, (*furnishIter)->getSigil());
	}
	// Paint the actors (including Items and sentients, but not the player) onto the map
	for (list<Actor*>::iterator actorIter = mapSource->allActors.begin(); actorIter != mapSource->allActors.end(); actorIter++) {
//		int color = (*actorIter)->getColor();
		if ((*actorIter) == playerObj) continue; // draw the player last
		terminal_color((*actorIter)->getColor());
		if ((*actorIter)->intent) terminal_layer(5);
		else terminal_layer(4);
		terminal_put(cursorXOrigin + (*actorIter)->getLocation().x, cursorYOrigin + (*actorIter)->getLocation().y, (*actorIter)->getSigil());
	}
	// Paint the player on the map
	terminal_layer(6);
	terminal_color(playerObj->getColor());
	terminal_put(cursorXOrigin + playerObj->location.x, cursorYOrigin + playerObj->location.y, playerObj->getSigil());
}
// ********************
// **** MESSAGE READOUT
MessageReadout::MessageReadout(uint inputID, cpair inputOrigin, uint inputWidth, uint inputHeight, MessageLog *inputSource) :
	GUIPanel(inputID, inputOrigin, inputWidth, inputHeight),
	logObject(inputSource)
{	}
void MessageReadout::display() {
	// Open a message log and display its contents within the window
	// Obtain the starting position and set some defaults
	// FIXME: Does not handle text wrapping!
	int cursorXPosition = this->origin.x + 1;
	int cursorYPosition = this->origin.y + height - 3;
	terminal_color("white"); // Default text color, can be overridden inline
	terminal_layer(9);
	if (this->logObject->messageList.size() > 0) {
		vector<string>::reverse_iterator msgLogIter = this->logObject->messageList.rbegin();
		for ( ; msgLogIter != this->logObject->messageList.rend(); msgLogIter++) {
			terminal_print(cursorXPosition, cursorYPosition--, (*msgLogIter).c_str());
		}
	}
}
// *****************
// **** DATA DISPLAY
DataDisplay::DataDisplay(uint inputID, cpair inputOrigin, uint inputWidth, uint inputHeight, Actor* inputActor) :
	GUIPanel(inputID, inputOrigin, inputWidth, inputHeight),
	targetActor(inputActor)
{	}
void DataDisplay::display() {
	int cursorXPosition = this->origin.x + 1;
	int cursorYPosition = this->origin.y + 1;
	terminal_color("white"); // Default text color, can be overridden inline
	terminal_layer(9);
	terminal_print(cursorXPosition, cursorYPosition, targetActor->getName().c_str());
	
	cursorYPosition++;
	terminal_print(cursorXPosition, cursorYPosition, to_string(targetActor->getLocation().x).c_str());
	cursorXPosition += 2;
	terminal_print(cursorXPosition, cursorYPosition, ", ");
	cursorXPosition += 2;
	terminal_print(cursorXPosition, cursorYPosition, to_string(targetActor->getLocation().y).c_str());
	cursorXPosition = this->origin.x + 1;
	cursorYPosition++;
	terminal_print(cursorXPosition, cursorYPosition, GameGUI::engine->worldClock.getCurrentTimeString().c_str());
}
// *******************
// **** COMMAND PROMPT
CommandPrompt::CommandPrompt(uint inputID, cpair inputOrigin, uint inputWidth) :
	GUIPanel(inputID, inputOrigin, inputWidth, 3),
	promptPrefix("> "),
	visible(false),
	inputBuffer(nullptr)
{	}
CommandPrompt::~CommandPrompt()
{	}
void CommandPrompt::display() {
	int cursorXPosition = this->origin.x + 1;
	int cursorYPosition = this->origin.y + 1;
//	GameGUI::drawHorizontalLine(cursorXPosition, cursorYPosition, this->width - 1);
//	GameGUI::drawHorizontalLine(cursorXPosition, cursorYPosition + this->height - 1, this->width - 1);
//	GameGUI::drawVerticalLine(cursorXPosition, cursorYPosition, this->height);
//	GameGUI::drawVerticalLine(cursorXPosition + this->width - 1, cursorYPosition, this->height);
	// Set the background
	terminal_color("black");
	terminal_bkcolor(0xFF333333);
	terminal_layer(0);
	terminal_clear_area(cursorXPosition, cursorYPosition, width - 1, height);

	// Draw the prompt components
	cursorXPosition = this->origin.x + 1;
	cursorYPosition = this->origin.y + 1;
	terminal_layer(10);
	terminal_color("white"); // Default text color, can be overridden inline
	terminal_print(cursorXPosition, cursorYPosition, promptPrefix.c_str());
	cursorXPosition += promptPrefix.length();
	int bufferWidth = this->width - cursorXPosition - 1;
	inputBuffer = new char[bufferWidth];
	inputBuffer[0] = 0; // reterminate the buffer to prevent junk insertion
//	LOGMSG("CLI buffer contents: " << inputBuffer);
	terminal_read_str(cursorXPosition, cursorYPosition, inputBuffer, bufferWidth);
	// Send the input string for interpretation by the game
	GameGUI::engine->interpretLongCommand(inputBuffer);
	delete inputBuffer;
}
// ***************
// **** MESSAGELOG
int MessageLog::add(string newMessage) {
	// Adds the input string to the message log list
	messageList.push_back(newMessage);
	return messageList.size();
}

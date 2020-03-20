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
#include "event.hpp"

using namespace std;

// ***************
// **** MESSAGELOG
int MessageLog::add(string newMessage) {
	// Adds the input string to the message log list
	messageList.push_back(newMessage);
	return messageList.size();
}// *************
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
		offsetOrigin.x += xOffset;
	} else {
		uint yOffset = this->height;
		offsetOrigin.y += yOffset;
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
		int displayColor = (*furnishIter)->getColor();
//		if (mapSource->getOccupant((*furnishIter)->location)) displayColor -= 0x66000000;
		terminal_color(displayColor);
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
	int cursorYPosition = this->origin.y + height - 2;
	terminal_color("white"); // Default text color, can be overridden inline
	terminal_layer(9);
	if (this->logObject->messageList.size() > 0) {
		vector<string>::reverse_iterator msgLogIter = this->logObject->messageList.rbegin();
		for ( ; msgLogIter != this->logObject->messageList.rend(); msgLogIter++) {
//			terminal_print(cursorXPosition, cursorYPosition--, (*msgLogIter).c_str());
			dimensions_t stringDims = terminal_measure_ext(this->width, this->height, (*msgLogIter).c_str());
			if (cursorYPosition > this->origin.y) {
				terminal_print_ext(cursorXPosition, cursorYPosition--, stringDims.width, stringDims.height, TK_ALIGN_DEFAULT, (*msgLogIter).c_str());
			}
		}
	}
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
// *****************
// **** DATA DISPLAY
DataDisplay::DataDisplay(uint inputID, cpair inputOrigin, uint inputWidth, uint inputHeight, Actor* inputActor) :
	GUIPanel(inputID, inputOrigin, inputWidth, inputHeight),
	targetActor(inputActor)
{	}
DataDisplay::~DataDisplay() {
	// delete everything in the displayList
	while (displayList.size() > 0) {
		displayList.pop_back();
	}
}
void DataDisplay::addVitals(Actor* playerPtr) {
	// Sets up the panel as a display for the player's vital statistics
	Vitals *playerVitals = new Vitals();
	playerVitals->target = playerPtr;
	playerVitals->width = this->width;
	displayList.push_back(playerVitals);
}
void DataDisplay::addTimer(CountdownTimer *timerPtr) {
	Clock *newClock = new Clock(timerPtr);
	newClock->width = this->width;
	displayList.push_back(newClock);
}
//addTimer
void DataDisplay::display() {
	// The cursor starts inside the 'bleed' edge of the display
	// Individual panels are responsible for setting the right offset
	uint cursorXPosition = this->origin.x;
	uint cursorYPosition = this->origin.y;
//	LOGMSG("Displaying panel #" << this->id << " at " << cursorXPosition << ", " << cursorYPosition);
	terminal_color("white"); // Default text color, can be overridden inline
	terminal_layer(9);
	/* xxx DISABLED -- old method
	// name
	terminal_print(cursorXPosition, cursorYPosition, targetActor->getName().c_str());
	cursorYPosition++;
	// location coordinates
	terminal_print(cursorXPosition, cursorYPosition, to_string(targetActor->getLocation().x).c_str());
	cursorXPosition += 2;
	terminal_print(cursorXPosition, cursorYPosition, ", ");
	cursorXPosition += 2;
	terminal_print(cursorXPosition, cursorYPosition, to_string(targetActor->getLocation().y).c_str());
	cursorXPosition = this->origin.x + 1;
	cursorYPosition++;
	// current time
	terminal_print(cursorXPosition, cursorYPosition, GameGUI::engine->worldClock.getCurrentTimeString().c_str());
	cursorYPosition++;
	// A position ruler of 40 cols for debugging/planning
//	terminal_print(cursorXPosition, cursorYPosition, "0123456789012345678901234567890123456789");
	*/
	if (!displayList.empty()) {
		list<DataObject*>::iterator dataIter = displayList.begin();
		while (cursorYPosition <= (this->origin.y + height)) {
			if ((cursorYPosition + (*dataIter)->height) > (this->origin.y + height)) {
				ERRMSG("Not enough space in DataDisplay #" << this->id << " for remaining panels");
				break;
			}
			cursorYPosition += (*dataIter)->display(cursorXPosition, cursorYPosition);
			dataIter++;
			if (dataIter == displayList.end()) break;
		};
	}
}
// ***************
// **** DataObject
DataDisplay::DataObject::DataObject() :
	titleString(""),
	height(1)
{	}
DataDisplay::DataObject::DataObject(uint inputHeight) :
	titleString(""),
	height(inputHeight)
{	}
DataDisplay::DataObject::DataObject(string inputTitle, uint inputHeight) :
	titleString(inputTitle),
	height(inputHeight)
{	}
void DataDisplay::DataObject::displayTitle(uint xPos, uint yPos) {
	// Only show a titlebar line if there's enough room
	if (this->height > 1) {
		string titleBar = "-=[0x2261]";
		titleBar += titleString;
		titleBar += "[0x2261]=-";
		uint titleWidth = terminal_measure(titleBar.c_str()).width;
		titleWidth += 3;
//		LOGMSG("displaying a titlebar, length: " << titleWidth);
		while (titleWidth <= this->width) {
			titleBar += "-";
			titleWidth++;
		};
		terminal_print(xPos, yPos, titleBar.c_str());
	}
}
// ***********
// **** Vitals
Vitals::Vitals() :
	DataObject("VITALS", 9)
{	}
uint Vitals::display(uint xPos, uint yPos) {
	// Displays some simple information about player vitals
//	LOGMSG("displaying vitals at " << xPos << ", " << yPos);
	uint xOffset = 1;
	uint yOffset = 1;
	displayTitle(xPos + xOffset, yPos + yOffset);
	xOffset++;
	yOffset++;
	// name
	terminal_print(xPos + xOffset, yPos + yOffset, target->getName().c_str());
	yOffset++;
	// location coordinates
	terminal_print(xPos + xOffset, yPos + yOffset, to_string(target->getLocation().x).c_str());
	xOffset += 2;
	terminal_print(xPos + xOffset, yPos + yOffset, ", ");
	xOffset += 2;
	terminal_print(xPos + xOffset, yPos + yOffset, to_string(target->getLocation().y).c_str());
	xOffset = 2;
	yOffset++;
	// current time
	terminal_print(xPos + xOffset, yPos + yOffset, GameGUI::engine->worldClock.getCurrentTimeString().c_str());
	yOffset++;

	// end display methods
	height = yOffset; // update this object's height to the actual value
	return yOffset;
}
// **********
// **** Timer
Clock::Clock(CountdownTimer *timerPtr) :
	DataObject("GRAVITY WELL", 9),
	localClock(timerPtr),
	digitZero	{ upLeft, horizBar, upRight, vertBar, 0x2508, vertBar, downLeft, horizBar, downRight },
	digitOne	{ serifUL, upRight, empty, empty, vertBar, empty, serifDL, downTee, serifDR },
	digitTwo	{ serifUL, horizBar, upRight, upLeft, horizBar, downRight, downLeft, horizBar, serifDR },
	digitThree	{ serifUL, horizBar, upRight, empty, serifLT, rightTee, serifDL, horizBar, downRight },
	digitFour	{ serifLU, empty, serifRU, downLeft, horizBar, rightTee, empty, empty, serifRD },
	digitFive	{ upLeft, horizBar, serifUR, downLeft, horizBar, upRight, serifDL, horizBar, downRight },
	digitSix	{ upLeft, horizBar, serifUR, leftTee, horizBar, upRight, downLeft, horizBar, downRight },
	digitSeven	{ serifUL, horizBar, upRight, empty, empty, vertBar, empty, empty, serifRD },
	digitEight	{ upLeft, horizBar, upRight, leftTee, horizBar, rightTee, downLeft, horizBar, downRight },
	digitNine	{ upLeft, horizBar, upRight, downLeft, horizBar, rightTee, empty, empty, serifRD }
{
	// Displays the time remaining on the given event's countdown
	// initialize the digit arrays
}
uint Clock::display(uint xPos, uint yPos) {
//	LOGMSG("displaying a clock at " << xPos << ", " << yPos);
	uint xOffset = 1;
	uint yOffset = 1;
	displayTitle(xPos + xOffset, yPos + yOffset);
	yOffset++;
	// get remaining event time
	uint timeValue[5] = {0};
	Chrono::getTimeValues(timeValue, localClock->getRemainingTime());
	// change clock color depending on remaining time
	if (localClock->getRemainingTime() < 5) {
		terminal_color("red");
	} else {
		terminal_color("grey");
	}
	yOffset += drawReadout(timeValue, xPos + xOffset, yPos + yOffset);

	// end display methods
	this->height = yOffset;
	return this->height;
}
uint Clock::drawReadout(uint input[], uint xPos, uint yPos) {
	// All offsets should have been factored into the x/yPos already!
	xPos++;
	// draw a +/- or AM/PM symbol here?
	terminal_put(xPos, yPos + 1, 0x2500);
	xPos++;
	drawClockDigits(input[1], 2, xPos, yPos);
	xPos += 6;
	drawColon(xPos, yPos);
	xPos += 1;
	drawClockDigits(input[2], 2, xPos, yPos);
	xPos += 6;
	drawColon(xPos, yPos);
	xPos += 1;
	drawClockDigits(input[3], 2, xPos, yPos);
	xPos += 6;
	drawPeriod(xPos, yPos);
	xPos += 1;
	drawClockDigits(input[4], 3, xPos, yPos);
	return 3;
}
void Clock::drawClockDigits(uint value, uint numLength, uint xPos, uint yPos) {
	// iterate across the 3x3 blocks of each digit in turn
//	LOGMSG("clock digit: " << value << " of l: " << numLength << " at " << xPos << ", " << yPos);
	uint hundDigit = value / 100;
	uint tensDigit = value / 10;
	if (tensDigit >= 10) tensDigit %= 10;
	uint onesDigit = value % 10;
	int *digitArray = nullptr;
	// select the first digit to draw
	uint targetDigit = 0;

	while (numLength > 0) {
		if (numLength == 3) targetDigit = hundDigit;
		else if (numLength == 2) targetDigit = tensDigit;
		else if (numLength == 1) targetDigit = onesDigit;
		digitArray = getDigitArray(targetDigit);
		for (uint xOffset = 0; xOffset < 3; xOffset++) {
			for (uint yOffset = 0; yOffset < 3; yOffset++) {
				terminal_put(xPos + xOffset, yPos + yOffset, digitArray[xOffset + yOffset * 3]);
			}
		}
		xPos += 3;
		numLength--;
	};
}
void Clock::drawColon(uint xPos, uint yPos) {
	terminal_put(xPos, yPos + 1, 0x2022);
	terminal_put(xPos, yPos + 2, 0x2022);
}
void Clock::drawPeriod(uint xPos, uint yPos) {
	terminal_put(xPos, yPos, ' ');
	terminal_put(xPos, yPos + 1, ' ');
	terminal_put(xPos, yPos + 2, 0x2022);
}
int* Clock::getDigitArray(uint value) {
	int *arrayPtr = nullptr;
	switch(value) {
		case 0:
			arrayPtr = digitZero;
			break;
		case 1:
			arrayPtr = digitOne;
			break;
		case 2:
			arrayPtr = digitTwo;
			break;
		case 3:
			arrayPtr = digitThree;
			break;
		case 4:
			arrayPtr = digitFour;
			break;
		case 5:
			arrayPtr = digitFive;
			break;
		case 6:
			arrayPtr = digitSix;
			break;
		case 7:
			arrayPtr = digitSeven;
			break;
		case 8:
			arrayPtr = digitEight;
			break;
		case 9:
			arrayPtr = digitNine;
			break;
		default:
			ERRMSG("Cannot draw digit: value out of range - " << value);
			break;
	}
	return arrayPtr;
}

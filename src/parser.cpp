/*   parser.cpp
AUTH xazak
DATE Feb 13 2020
DESC Defines the GameParser class, which collects and interprets player input
*/

//Y K U
//H . L
//B J N

#include "parser.hpp"
#include "action.hpp"
#include "sentience.hpp"
#include "container.hpp"
#include "map.hpp"
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void GameParser::interpret(char inputKey) {
	// Perform the set of lookups and clarifications needed to turn a keyboard
	// input value into an action generated on the player-actor's stack
	
	// Update the curr/prev actions
	previousKey = currentKey;
	currentKey = inputKey;
	// Perform the lookup
	localContext.reset();
	localContext.type = keycodeLookup[currentKey];
	localContext.subject = player; // The player originates all actions
	localContext.vicinity = player->getLocality(); // Same as player usually
	// Use the derived type to obtain any needed details and push an action
	bool validInput = false; // Help prevent crashes w/ unimplemented actions
	// ALL HANDLERS must set the localContext.target if nothing else!
	// Even a reflexive command takes the player as the context target
	Actor* tileContents = nullptr;
	switch(localContext.type) {
		case ActionType::IDLE:
			ERRMSG("Action: IDLE was passed to the parser! keycode: [" << currentKey << "]");
		break;
		case ActionType::WAIT:
			LOGMSG("Action: WAIT unimplemented");
		break;
		case ActionType::MOVE:
			// REQ: x, y coords of destination Tile
//			LOGMSG("Action::MOVE detected");
//			LOGMSG("Player located at " << player->location);
			localContext.target = player;
			if (currentKey == 'y' || currentKey == 'h' || currentKey == 'b') {
				localContext.echs = -1 + player->getLocation().x;
			} else if (currentKey == 'u' || currentKey == 'l' || currentKey == 'n') {
				localContext.echs =  1 + player->getLocation().x;
			} else {
				localContext.echs = player->getLocation().x;
			}
			if (currentKey == 'y' || currentKey == 'k' || currentKey == 'u') {
				localContext.whye = -1 + player->getLocation().y;
			} else if (currentKey == 'b' || currentKey == 'j' || currentKey == 'n') {
				localContext.whye =  1 + player->getLocation().y;
			} else {
				localContext.whye = player->getLocation().y;
			}
//			LOGMSG("Creating MOVE action");
//			LOGMSG("Player moving from " << player->location << " to " << localContext.echs << ", " << localContext.whye);
			input = new MoveAction(localContext);
			validInput = true;
		break;
		case ActionType::JUMP:
			LOGMSG("Action: JUMP unimplemented");
		break;
		case ActionType::GET:
//			LOGMSG("Action: GET detected");
			localContext.target = localContext.vicinity->getContents(localContext.subject->location);
//			localContext.dump();
			input = new GetAction(localContext);
			validInput = true;
		break;
		case ActionType::DROP:
//			LOGMSG("Action: DROP detected");
			localContext.target = localContext.subject->contents->itemList.front();
			input = new DropAction(localContext);
			validInput = true;
		break;
		case ActionType::CONSUME:
			LOGMSG("Action: CONSUME unimplemented");
		break;
		case ActionType::OPEN:
//			LOGMSG("Action: OPEN detected");
			// Scan the neighborhood for any external openable objects
			// FIXME: Does not handle multiple Openable objects
			// FIXME: Should not have to duplicate this code for CLOSE...
			tileContents = nullptr;
			for (int xIndex = -1; xIndex <= 1; xIndex++) {
				for (int yIndex = -1; yIndex <= 1; yIndex++) {
					// Obtain access to the contents of the nearby tile
//					tileContents = localContext.vicinity->getContents((localContext.subject->location.x + xIndex), (localContext.subject->location.y + yIndex));
					tileContents = localContext.vicinity->getFurnitureAt((localContext.subject->location.x + xIndex), (localContext.subject->location.y + yIndex));
					// Check all items in tile contents for Openability
					// If the target can't be opened, don't use it as a target
//					LOGMSG("Checking " << tileContents);
					if (tileContents != nullptr) {
						if (tileContents->aperture) {
							localContext.target = tileContents;
						}
					}
				}
			}
			if (localContext.target != nullptr) {
				input = new OpenAction(localContext);
				validInput = true;
//				localContext.dump();
			}
		break;
		case ActionType::CLOSE:
//			LOGMSG("Action: CLOSE detected");
			tileContents = nullptr;
			for (int xIndex = -1; xIndex <= 1; xIndex++) {
				for (int yIndex = -1; yIndex <= 1; yIndex++) {
					// Obtain access to the contents of the nearby tile
//					tileContents = localContext.vicinity->getContents((localContext.subject->location.x + xIndex), (localContext.subject->location.y + yIndex));
					tileContents = localContext.vicinity->getFurnitureAt((localContext.subject->location.x + xIndex), (localContext.subject->location.y + yIndex));
					// Check all items in tile contents for Openability
					// If the target can't be opened, don't use it as a target
//					LOGMSG("Checking " << tileContents);
					if (tileContents != nullptr) {
						if (tileContents->aperture) {
							localContext.target = tileContents;
						}
					}
				}
			}
			if (localContext.target != nullptr) {
				input = new CloseAction(localContext);
				validInput = true;
//				localContext.dump();
			}
		break;
		case ActionType::TOGGLE:
			LOGMSG("Action: TOGGLE unimplemented");
		break;
		case ActionType::OPERATE:
			LOGMSG("Action: OPERATE unimplemented");
		break;
		case ActionType::PUSH:
			LOGMSG("Action: PUSH unimplemented");
		break;
		case ActionType::FARLOOK:
			LOGMSG("Action: FARLOOK unimplemented");
		break;
		case ActionType::EXAMINE:
			LOGMSG("Action: EXAMINE unimplemented");
		break;
		case ActionType::WEAR:
			LOGMSG("Action: WEAR unimplemented");
		break;
		case ActionType::REMOVE:
			LOGMSG("Action: REMOVE unimplemented");
		break;
		case ActionType::INVENTORY:
			LOGMSG("Action: INVENTORY unimplemented");
		break;
		case ActionType::META_SAVE:
			LOGMSG("Action: META_SAVE unimplemented");
		break;
		default:
			ERRMSG("No context handler for ActionType #" << (int)localContext.type);
		break;
	}
//	player->intention.doAction(context);
	// ALL actions should have a target set; if not, DO NOT attempt the action
	if (validInput) {
		if (input->context->target == nullptr) {
			ERRMSG("Action type #" << (uint)input->context->type << " attempted with NO TARGET");
		} else {
			player->intent->pushAction(input);
			delete input;
		}
	}
}
void GameParser::interpret(const char * inputBuffer) {
	// Parses a long command into constituent pieces and calls actions
	string fullString(inputBuffer);
	string commandString;
//	LOGMSG("Received long command: " << fullString);
	// Break the command down by single spaces into a set of keywords
	// Examine the first keyword to find a command match
	// Use the remaining keywords to provide context
	stringstream inputStream;
	inputStream.clear();
	inputStream << fullString;
	getline(inputStream, commandString, ' ');
//	LOGMSG("Parsing command " << commandString);
	// Perform the lookup
	localContext.reset();
	localContext.type = commandLookup[commandString];
	localContext.subject = player; // The player originates all actions
	localContext.vicinity = player->getLocality(); // Same as player usually
	// Perform the context interpretation
	// Break up the rest of the input stream into words
	vector<string> commandTokens;
	while (!inputStream.eof()) {
		string token;
		getline(inputStream, token, ' ');
		commandTokens.push_back(token);
	}
//	LOGMSG("Quantity of trailing command tokens: " << commandTokens.size());
	bool validInput = false;
	Actor* tileContents = nullptr;
	switch(localContext.type) {
		case ActionType::IDLE:
			ERRMSG("Action: IDLE was passed to the parser! (long)");
		break;
		case ActionType::WAIT:
			LOGMSG("Action: WAIT unimplemented (long)");
		break;
		case ActionType::MOVE:
			// REQ: x, y coords of destination Tile
			LOGMSG("Action::MOVE detected (long)");
//			LOGMSG("Player located at " << player->location);
			localContext.target = player;
			// FIXME: Prompt for additional inputs if not enough given
			if (commandTokens.size() != 1) {
				ERRMSG("Incorrect number of arguments: MOVE <direction>");
				break;
			}
			if (commandTokens.front() == "west") localContext.echs -= 1;
			if (commandTokens.front() == "east") localContext.echs += 1;
			if (commandTokens.front() == "north") localContext.whye -= 1;
			if (commandTokens.front() == "south") localContext.whye += 1;
//			LOGMSG("Player moving from " << player->location << " to " << player->location.x + localContext.echs << ", " << player->location.y + localContext.whye);
			input = new MoveAction(localContext);
			validInput = true;
		break;
		case ActionType::JUMP:
			LOGMSG("Action: JUMP unimplemented (long)");
		break;
		case ActionType::GET:
			LOGMSG("Action: GET detected (long)");
//			localContext.target = localContext.vicinity->getContents(localContext.subject->location);
//			localContext.dump();
//			input = new GetAction(localContext);
//			validInput = true;
		break;
		case ActionType::DROP:
			LOGMSG("Action: DROP detected (long)");
//			localContext.target = localContext.subject->contents->itemList.front();
//			input = new DropAction(localContext);
//			validInput = true;
		break;
		case ActionType::CONSUME:
			LOGMSG("Action: CONSUME unimplemented (long)");
		break;
		case ActionType::OPEN:
			LOGMSG("Action: OPEN detected (long)");
			// Scan the neighborhood for any external openable objects
			// FIXME: Does not handle multiple Openable objects
			// FIXME: Should not have to duplicate this code for CLOSE...
			tileContents = nullptr;
			for (int xIndex = -1; xIndex <= 1; xIndex++) {
				for (int yIndex = -1; yIndex <= 1; yIndex++) {
					// Obtain access to the contents of the nearby tile
					tileContents = localContext.vicinity->getContents((localContext.subject->location.x + xIndex), (localContext.subject->location.y + yIndex));
					// Check all items in tile contents for Openability
					// If the target can't be opened, don't use it as a target
//					LOGMSG("Checking " << tileContents);
					if (tileContents != nullptr) {
						if (tileContents->aperture) {
							localContext.target = tileContents;
						}
					}
				}
			}
			if (localContext.target != nullptr) {
				input = new OpenAction(localContext);
				validInput = true;
//				localContext.dump();
			}
		break;
		case ActionType::CLOSE:
			LOGMSG("Action: CLOSE detected (long)");
			tileContents = nullptr;
			for (int xIndex = -1; xIndex <= 1; xIndex++) {
				for (int yIndex = -1; yIndex <= 1; yIndex++) {
					// Obtain access to the contents of the nearby tile
					tileContents = localContext.vicinity->getContents((localContext.subject->location.x + xIndex), (localContext.subject->location.y + yIndex));
					// Check all items in tile contents for Openability
					// If the target can't be opened, don't use it as a target
//					LOGMSG("Checking " << tileContents);
					if (tileContents != nullptr) {
						if (tileContents->aperture) {
							localContext.target = tileContents;
						}
					}
				}
			}
			if (localContext.target != nullptr) {
				input = new CloseAction(localContext);
				validInput = true;
//				localContext.dump();
			}
		break;
		case ActionType::TOGGLE:
			LOGMSG("Action: TOGGLE unimplemented (long)");
		break;
		case ActionType::OPERATE:
			LOGMSG("Action: OPERATE unimplemented (long)");
		break;
		case ActionType::PUSH:
			LOGMSG("Action: PUSH unimplemented (long)");
		break;
		case ActionType::FARLOOK:
			LOGMSG("Action: FARLOOK unimplemented (long)");
		break;
		case ActionType::EXAMINE:
			LOGMSG("Action: EXAMINE unimplemented (long)");
		break;
		case ActionType::WEAR:
			LOGMSG("Action: WEAR unimplemented (long)");
		break;
		case ActionType::REMOVE:
			LOGMSG("Action: REMOVE unimplemented (long)");
		break;
		case ActionType::INVENTORY:
			LOGMSG("Action: INVENTORY unimplemented (long)");
		break;
		case ActionType::META_QUIT:
//			LOGMSG("Action: META_QUIT invoked (long)");
			input = new QuitMetaAction();
			validInput = true;
		break;
		default:
			ERRMSG("No (long) context handler for ActionType #" << (int)localContext.type);
		break;
	}
	// ALL actions are responsible for setting a valid target!
	if (validInput) {
		player->intent->pushAction(input);
		delete input;
	}
}

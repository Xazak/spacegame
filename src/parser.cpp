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
	switch(localContext.type) {
		case ActionType::IDLE:
			ERRMSG("Action: IDLE was passed to the parser!");
		break;
		case ActionType::WAIT:
			LOGMSG("Action: WAIT unimplemented");
		break;
		case ActionType::MOVE:
//			LOGMSG("Action::MOVE detected");
//			LOGMSG("Player located at " << player->location);
			localContext.target = player;
			if (currentKey == 'y' || currentKey == 'h' || currentKey == 'b') {
				localContext.echs = -1;
			}
			if (currentKey == 'u' || currentKey == 'l' || currentKey == 'n') {
				localContext.echs = 1;
			}
			if (currentKey == 'y' || currentKey == 'k' || currentKey == 'u') {
				localContext.whye = -1;
			}
			if (currentKey == 'b' || currentKey == 'j' || currentKey == 'n') {
				localContext.whye = 1;
			}
//			LOGMSG("Creating MOVE action");
//			LOGMSG("Player moving from " << player->location << " to " << player->location.x + localContext.echs << ", " << player->location.y + localContext.whye);
			input = new MoveAction(localContext);
		break;
		case ActionType::JUMP:
			LOGMSG("Action: JUMP unimplemented");
		break;
		case ActionType::GET:
			LOGMSG("Action: GET detected");
//			localContext.target = TILE CONTENTS;
			input = new GetAction(localContext);
		break;
		case ActionType::DROP:
			LOGMSG("Action: DROP unimplemented");
		break;
		case ActionType::CONSUME:
			LOGMSG("Action: CONSUME unimplemented");
		break;
		case ActionType::OPEN:
			LOGMSG("Action: OPEN unimplemented");
		break;
		case ActionType::CLOSE:
			LOGMSG("Action: CLOSE unimplemented");
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
		default:
			ERRMSG("No context handler for ActionType #" << (int)localContext.type);
		break;
	}
//	player->intention.doAction(context);
	player->intent->pushAction(input);
	delete input;
}

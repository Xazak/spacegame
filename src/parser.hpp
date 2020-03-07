/*   parser.hpp
AUTH xazak
DATE Feb 13 2020
DESC Defines the GameParser class, which collects and interprets player input
*/

#ifndef SPACEGAME_PARSER
#define SPACEGAME_PARSER

#include "actor.hpp"
#include "action.hpp"
#include "context.hpp"
#include <map>

struct GameParser {
	std::map<char, ActionType> keycodeLookup = {
		{ 'b', ActionType::MOVE },
		{ 'c', ActionType::CLOSE },
		{ 'd', ActionType::DROP },
		{ 'e', ActionType::CONSUME },
		{ 'g', ActionType::GET },
		{ 'h', ActionType::MOVE },
		{ 'i', ActionType::INVENTORY },
		{ 'j', ActionType::MOVE },
		{ 'k', ActionType::MOVE },
		{ 'l', ActionType::MOVE },
		{ 'n', ActionType::MOVE },
		{ 'o', ActionType::OPEN },
		{ 'p', ActionType::OPERATE },
		{ 'r', ActionType::REMOVE },
		{ 't', ActionType::TOGGLE },
		{ 'u', ActionType::MOVE },
		{ 'w', ActionType::WEAR },
		{ 'x', ActionType::EXAMINE },
		{ 'y', ActionType::MOVE },
		{ 'z', ActionType::WAIT },
		{ 'P', ActionType::PUSH },
		{ ';', ActionType::FARLOOK }
	};
	void interpret(char input);
	void interpret(const char * inputBuffer);
	char currentKey;
	char previousKey;
	// mouse inputs?
	Actor *player;
	void setPlayer(Actor *playerPtr) { player = playerPtr; }
	Action *input;
	ActionContext localContext;
};

#endif //SPACEGAME_PARSER

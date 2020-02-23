/*   services.cpp
AUTH xazak
DATE Feb 20 2020
DESC Describes the Service Locator class, which maintains a registry of game
	services (such as the MessageLog) for use by the game system
*/

#include "services.hpp"
#include "main.hpp"

MessageLog* ServiceLocator::msgLogService = nullptr;

void ServiceLocator::provide(MessageLog* msgLogSource) {
	msgLogService = msgLogSource;
	LOGMSG("Message service created: " << msgLogService);
}
MessageLog* ServiceLocator::getMsgLog() {
	return msgLogService;
}

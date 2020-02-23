/*   services.hpp
AUTH xazak
DATE Feb 20 2020
DESC Describes the Service Locator class, which maintains a registry of game
	services (such as the MessageLog) for use by the game system
*/

#ifndef SPACEGAME_SERVICE_LOCATOR
#define SPACEGAME_SERVICE_LOCATOR

struct MessageLog; // fwd-decl

class ServiceLocator {
	public:
//		static ServiceLocator();
		static void provide(MessageLog* msgLogSource);
		static MessageLog* getMsgLog();

	private:
		static MessageLog* msgLogService;
};

#endif//SPACEGAME_SERVICE_LOCATOR

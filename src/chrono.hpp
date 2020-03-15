/*   chrono.hpp
AUTH xazak
DATE Mar 10 2020
DESC Describes the Chronometer class, which handles all of the clock and timing-
     related systems, such as the internal world-time and event timers.
*/

#ifndef SPACEGAME_CHRONOMETER
#define SPACEGAME_CHRONOMETER

#include "main.hpp"

class Chrono {
	public:
		Chrono();
//		~Chrono();
		void update(double timeDelta);
		std::string getCurrentTimeString();
		uint getDays() { return days; }
		uint getHours() { return hours; }
		uint getMinutes() { return minutes; }
		uint getSeconds() { return seconds; }
		uint getLastDelta() { return delta; }

	private:
		double rawTimeValue;
		uint delta;
		uint days;
		uint hours;
		uint minutes;
		uint seconds;
		uint milliseconds;
};

#endif // SPACEGAME_CHRONOMETER

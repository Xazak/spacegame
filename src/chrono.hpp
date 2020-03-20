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
		static std::string timeToString(double timeValue);
		static void getTimeValues(uint target[], double inputValue);

	private:
		double rawTimeValue;
		uint days;
		uint hours;
		uint minutes;
		uint seconds;
		uint milliseconds;
		static double dayFactor;
		static double hourFactor;
		static double minuteFactor;
		static double msecFactor;
};

#endif // SPACEGAME_CHRONOMETER

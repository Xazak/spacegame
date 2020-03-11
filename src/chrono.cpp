/*   chrono.cpp
AUTH xazak
DATE Mar 10 2020
DESC Describes the Chronometer class, which handles all of the clock and timing-
     related systems, such as the internal world-time and event timers.
*/

#include "chrono.hpp"
#include "main.hpp"
#include <string>

using namespace std;

Chrono::Chrono() :
	rawTimeValue(0),
	days(0),
	hours(0),
	minutes(0),
	seconds(0)
{	update(INGAME_EPOCH); }
void Chrono::update(double timeDelta) {
	rawTimeValue += timeDelta;
	uint approxTime = (uint)rawTimeValue;
	days = approxTime / 60 / 60 / 24;
	approxTime -= (days * 24 * 60 * 60);
	hours = approxTime / 60 / 60;
	approxTime -= (hours * 60 * 60);
	minutes = approxTime / 60;
	approxTime -= (minutes * 60);
	seconds = approxTime;
//	LOGMSG("D-H:M.S : " << days << "-" << hours << ":" << minutes << "." << seconds);
}
string Chrono::getCurrentTimeString() {
	string timeString = "";
	timeString += to_string(days);
	timeString += "d, ";
	timeString += to_string(hours);
	timeString += ":";
	timeString += to_string(minutes);
	timeString += ".";
	timeString += to_string(seconds);
//	LOGMSG("Current time: " << timeString);
	return timeString;
}

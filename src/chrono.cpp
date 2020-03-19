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
	delta(0),
	days(0),
	hours(0),
	minutes(0),
	seconds(0),
	milliseconds(0)
{	update(INGAME_EPOCH); }
void Chrono::update(double timeDelta) {
	delta = (uint)(timeDelta * 1000);
//	LOGMSG("input: " << timeDelta << " / Delta: " << delta);
	rawTimeValue += timeDelta * 1000;
//	LOGMSG("raw time value: " << rawTimeValue);
	uint approxTime = (uint)rawTimeValue;
	days = approxTime / 60 / 60 / 24 / 1000;
	approxTime -= (days * 24 * 60 * 60 * 1000);
	hours = approxTime / 60 / 60 / 1000;
	approxTime -= (hours * 60 * 60 * 1000);
	minutes = approxTime / 60 / 1000;
	approxTime -= (minutes * 60 * 1000);
	seconds = approxTime / 1000;
	approxTime -= seconds * 1000;
	milliseconds = approxTime;
//	LOGMSG("mss: " << milliseconds);
//	LOGMSG("D-H:M.S : " << days << "-" << hours << ":" << minutes << ":" << seconds << "." << milliseconds);
}
string Chrono::getCurrentTimeString() {
	string timeString = "";
	timeString += "3173-06-";
	timeString += to_string(days);
	timeString += " / ";
	if (hours < 10) timeString+= "0";
	timeString += to_string(hours);
	timeString += ":";
	if (minutes < 10) timeString+= "0";
	timeString += to_string(minutes);
	timeString += ":";
	if (seconds < 10) timeString+= "0";
	timeString += to_string(seconds);
	timeString += ".";
	if (milliseconds < 100) timeString += "0";
	if (milliseconds < 10) timeString += "0";
	timeString += to_string(milliseconds);
	return timeString;
}
string Chrono::timeToString(double timeValue) {
	// Returns a string formatted as HH:MM:SS.mss
	string timeString = "";
	// Calculate the time values
	// dropping # of days for now
	int tempDays = timeValue / 60 / 60 / 24;
	timeValue -= (tempDays * 24 * 60 * 60);
	int tempHours = timeValue / 60 / 60;
	timeValue -= (tempHours * 60 * 60);
	int tempMinutes = timeValue / 60;
	timeValue -= (tempMinutes * 60);
	int tempSeconds = timeValue;
	timeValue -= (tempSeconds);
	int tempMSS = timeValue * 1000;
	// Build the string
	if (tempHours < 10) timeString += "0";
	timeString += to_string(tempHours);
	timeString += ":";
	if (tempMinutes < 10) timeString += "0";
	timeString += to_string(tempMinutes);
	timeString += ":";
	if (tempSeconds < 10) timeString += "0";
	timeString += to_string(tempSeconds);
	timeString += ".";
	if (tempMSS < 100) timeString += "0";
	if (tempMSS < 10) timeString += "0";
	timeString += to_string(tempMSS);
//	LOGMSG("Created string " << timeString << " from " << timeValue);
	return timeString;
}

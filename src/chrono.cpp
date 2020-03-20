/*   chrono.cpp
AUTH xazak
DATE Mar 10 2020
DESC Describes the Chronometer class, which handles all of the clock and timing-
     related systems, such as the internal world-time and event timers.
*/

#include "chrono.hpp"
#include "main.hpp"
#include <string>
#include <cmath>

using namespace std;

double Chrono::dayFactor = 60 * 60 * 24;
double Chrono::hourFactor = 60 * 60;
double Chrono::minuteFactor = 60;
double Chrono::msecFactor = 1000;

Chrono::Chrono() :
	rawTimeValue(0),
	days(0),
	hours(0),
	minutes(0),
	seconds(0),
	milliseconds(0)
//	dayFactor(60 * 60 * 24),
//	hourFactor(60 * 60),
//	minuteFactor(60),
//	msecFactor(1000)
{	update(INGAME_EPOCH);
}
void Chrono::update(double timeDelta) {
	rawTimeValue += timeDelta;
	double approxTime = rawTimeValue;
	/* VERIFIED GOOD
	days = approxTime / 60 / 60 / 24;
	approxTime -= (days * 24 * 60 * 60);
	hours = approxTime / 60 / 60;
	approxTime -= (hours * 60 * 60);
	minutes = approxTime / 60;
	approxTime -= (minutes * 60);
	seconds = approxTime;
	approxTime -= seconds;
	milliseconds = approxTime * 1000;
	*/
	days = approxTime / dayFactor;
	approxTime -= (days * dayFactor);
	hours = approxTime / hourFactor;
	approxTime -= (hours * hourFactor);
	minutes = approxTime / minuteFactor;
	approxTime -= (minutes * minuteFactor);
	seconds = approxTime;
	approxTime -= seconds;
	milliseconds = approxTime * msecFactor;
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
	/*
	int tempDays = timeValue / 60 / 60 / 24;
	timeValue -= (tempDays * 24 * 60 * 60);
	int tempHours = timeValue / 60 / 60;
	timeValue -= (tempHours * 60 * 60);
	int tempMinutes = timeValue / 60;
	timeValue -= (tempMinutes * 60);
	int tempSeconds = timeValue;
	timeValue -= (tempSeconds);
	int tempMSS = timeValue * 1000;
	*/
	int tempDays = timeValue / dayFactor;
	timeValue -= (tempDays * dayFactor);
	int tempHours = timeValue / hourFactor;
	timeValue -= (tempHours * hourFactor);
	int tempMinutes = timeValue / minuteFactor;
	timeValue -= (tempMinutes * minuteFactor);
	int tempSeconds = timeValue;
	timeValue -= (tempSeconds);
	int tempMSS = timeValue * msecFactor;
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
void Chrono::getTimeValues(uint target[], double inputValue) {
	target[0] = inputValue / dayFactor;
	inputValue -= target[0] * dayFactor;
	target[1] = inputValue / hourFactor;
	inputValue -= target[1] * hourFactor;
	target[2] = inputValue / minuteFactor;
	inputValue -= target[2] * minuteFactor;
	target[3] = inputValue;
	inputValue -= target[3];
	target[4] = inputValue * msecFactor;
}

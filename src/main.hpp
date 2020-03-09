/*   main.hpp
AUTH xazak
DATE Jan 27 2020
DESC Contains the definitions of various useful data structures
*/

#ifndef MAIN_HPP_HEADER_GUARD
#define MAIN_HPP_HEADER_GUARD

#include "services.hpp"
#include <iostream>

// *** DEFINITIONS AND ALIASES
typedef unsigned int uint;

#define MS_PER_UPDATE 0.25

struct CoordinatePair {
	int x;
	int y;
	// need copy ctor? operator=? check code archives for possibles
	CoordinatePair(); 						// default ctor
	CoordinatePair(int inputX, int inputY);	// elemental ctor
	//CoordinatePair(const CoordinatePair& inputPair); // copy ctor
	void set(int inputX, int inputY) { x = inputX; y = inputY; }
	//operator=(int x, int y);					// assign by elements
	//operator=(const CoordinatePair& inputPair); // assign by copy
	friend std::ostream& operator<< (std::ostream &output, const CoordinatePair &inputPair);
//	std::ostream& operator<< (std::ostream &output);
	CoordinatePair operator+(const CoordinatePair& rhs);
	bool isEqual(const int& inputX, const int& inputY);
	bool isEqual(const CoordinatePair& inputPair);
	//uint distanceToAbs(int x, int y);
	//uint distanceToRel(int x, int y);
	//uint distanceTo(const CoordinatePair& targetLocation);
	//uint distanceTo(const Actor& targetActor);

};
typedef CoordinatePair cpair; // shorthand alias

#endif //MAIN_HPP_HEADER_GUARD
// *******************

// *** DEBUGGING TOOLS
// Enable debugging macros; comment out to disable all debugging macros
#define DEBUG_TOOLS

// Defines a debug macro for sending information to cerr easily. The contents
// of x will be rendered as literally as possible. Include quotes and newlines.
//		xxxMSG( "Debug Message\n" )
// The do-while statement ensures that x is interpreted correctly.
#ifdef DEBUG_TOOLS
#include <iostream>
#define LOGMSG(x) do { std::clog << "*** " << x << "\n    >> " << __FILE__ << ":" << __LINE__ << ":" << __func__ << "()" << std::endl; } while (0)
#else
#define LOGMSG(x)
#endif

#ifdef DEBUG_TOOLS
#define ERRMSG(x) do { std::cerr << "!*! " << x << "\n    >> " << __FILE__ << ":" << __LINE__ << ":" << __func__ << "()" << std::endl; } while (0)
#else
#define ERRMSG(x)
#endif

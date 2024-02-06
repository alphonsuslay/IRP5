// a_delays.cpp

// Version 0.1 Aug, 2014
// Copyright(c) 2014 Wong Song Sing
// Electronic & Computer Engineering
// Ngee Ann Polytechnic, Singapore

#include "a_delays.h"
// -------------------------------------------------
DELAYMS_T::DELAYMS_T(uint32_t ms ) {
	dlySet(ms);
}
// -------------------------------------------------
void DELAYMS_T::dlySet( uint32_t ms ) {
	// allow to be set again even not expired yet
	// thus suitable for implementing timeout
    dlyinus = true;     // if <1000, use us
	startTimeus = micros(); // start from current time
	if (ms == 0) {
		desiredDlyus = 0;
		wasInit = false;
	}
	else {
		desiredDlyus = ms*1000; // in us
		wasInit=true;
	}
}
// -------------------------------------------------
// This function prevents accumulative error
void DELAYMS_T::dlySetSync( uint32_t ms ) {
	if (ms == 0) {
		startTimeus = micros(); // assume expired straight away
		desiredDlyus = 0;
		wasInit = false;
	}
	else {
		if (wasInit) {
			// what IF has not expired yet and called again ?
			// should not be used this way ! Should Expired first before calling this
			startTimeus = micros(); // take NOW as the start time !
		}
		else {
			// proper usage would come here
			startTimeus = startTimeus + desiredDlyus; // start from old start + old desired delay
		}
        if (ms > DELAYMS_T_MAX) {    // CAP! due to use of micros()
            ms = DELAYMS_T_MAX;
        }
		desiredDlyus = ms*1000;
		wasInit = true;
	}
}
void DELAYMS_T::dlySetSinceOld( uint32_t ms ) {
    dlySetSync(ms);
}
// -------------------------------------------------
bool DELAYMS_T::dlyExpired( void ) {
	if (wasInit) {
		if (((micros() - startTimeus)) >= desiredDlyus) {
			wasInit = false;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		// not initialized, assume expired
		return true;
	}
}
// -------------------------------------------------

// delays.cpp

// formerly named a_delays.cpp

// Version 0.1 Aug, 2014
// Copyright(c) 2014 Wong Song Sing
// Electronic & Computer Engineering
// Ngee Ann Polytechnic, Singapore

// Nov 2015: cater for 32-bit ms, max delay 2^32ms=4294967s=1193hr=49.7days
// Dec 2015: make it portable to Windows with only millis()
// See a_delays.h for more details

#include "delays.h"

#define LMT_FOR_US 10 // below this, use micros(), else use millis()
// -------------------------------------------------
DELAYMS_T::DELAYMS_T(uint32_t ms ) {
	dlySet(ms);
}
// -------------------------------------------------
void DELAYMS_T::dlyRestart() {
    dlyReset();
}
void DELAYMS_T::dlyRestartSync() {
    dlyResetSync();
}
// -------------------------------------------------
void DELAYMS_T::dlyReset() {
#if DLY_USE_US
    if (dlyinus) {
        dlySet(desiredDly/1000);
    }
    else {
        dlySet(desiredDly);
    }
#else
    dlySet(desiredDly);
#endif
}
// -------------------------------------------------
void DELAYMS_T::dlyResetSync() {
#if DLY_USE_US
    if (dlyinus) {
        dlySetSync(desiredDly/1000);
    }
    else {
        dlySetSync(desiredDly);
    }
#else
    dlySetSync(desiredDly);
#endif
}
// -------------------------------------------------
void DELAYMS_T::dlySet( uint32_t ms ) {
	// allow to be set again even not expired yet
	// thus suitable for implementing timeout
    if (ms == 0) {
        wasInit = false;
		return;
	}
#if DLY_USE_US
    if (ms < LMT_FOR_US) {
        startTime = micros(); // start from current time
        dlyinus = true;     // if <1000, use us
        desiredDly = ms*1000; // in us
        wasInit = true;
//        Serial.print('u');
    }
    else {
        startTime = millis(); // start from current time
        dlyinus = false;
        desiredDly = ms; // in ms
        wasInit = true;
//        Serial.print('m');
    }
#else
    // micros() not used
    startTime = millis(); // start from current time
    if (ms == 0) {
        wasInit = false;
    }
    else {
        desiredDly = ms; // in ms
        wasInit = true;
    }    
#endif
//    dlyinus = true;
}
// -------------------------------------------------
// if already expired, do nothing
void DELAYMS_T::dlyChange( uint32_t ms ) {
	if (ms == 0) {
		wasInit = false;
		return;
	}
	if (wasInit) { // not expired yet
        // Not expired yet! Modify current delay!
		// Do not touch startTime
#if DLY_USE_US
        if (dlyinus) { 	// current delay in us
			if (ms < LMT_FOR_US) {
				// was in us and still in us, accept
				desiredDly = ms*1000; // continue with same starttime, only modify desiredDly
			}
			else {
				dlyinus = false;	// was in us, change to ms from now
				// startTime is in us, cannot proceed!
				dlySet(ms);
			}
        }
        else { // current delay in ms, leave it in ms still
			desiredDly = ms;
        }
#else
        desiredDly = ms;
#endif
	}
	// else do nothing
}
// -------------------------------------------------
// This function prevents accumulative error
void DELAYMS_T::dlySetSync( uint32_t ms ) {
	if (ms == 0) {
		wasInit = false;
		return;
	}
	if (wasInit) {
        // Not expired yet! Modify current delay!
#if DLY_USE_US
        if (dlyinus) { 	// current delay in us
			if (ms < LMT_FOR_US) {
				// was in us and still in us, accept
				desiredDly = ms*1000; // continue with same starttime, only modify desiredDly
			}
			else {
				dlyinus = false;	// was in us, change to ms from now
				dlySet(ms);
			}
        }
        else { // current delay in ms, leave it in ms still
			desiredDly = ms;
        }
#else
        desiredDly = ms;
#endif
	}
	else { // expired, new delay since last expiry time
#if DLY_USE_US
        // proper usage would come here
        if (dlyinus && (ms < LMT_FOR_US)) {
            // was in us and still in dlyinus, accept
            startTime = startTime + desiredDly; // in us: start from old start + old desired delay
            desiredDly = ms*1000;
            wasInit = true;
//            Serial.print(" sync");
        }
        else {
            // cross from us to ms, or ms to us, ignore!
            dlySet(ms); // delay from now; wasInit take care off
        }
#else
        startTime = startTime + desiredDly; // start from old start + old desired delay
        desiredDly = ms;
        wasInit = true;
#endif
    }
}
// -------------------------------------------------
void DELAYMS_T::dlySetSinceOld( uint32_t ms ) {
    dlySetSync(ms);
}
// -------------------------------------------------
bool DELAYMS_T::dlyExpired( void ) {
	if (wasInit) {
#if DLY_USE_US
        if (dlyinus) {
            // delay in us
            if ((micros() - startTime) >= desiredDly) {
                wasInit = false;
                return true; // expired
            }
		}
        else {
            // delay in ms
            if ((millis() - startTime) >= desiredDly) {
                wasInit = false;
                return true; // expired
            }
        }
        return false;
#else
        if ((millis() - startTime) >= desiredDly) {
            wasInit = false;
            return true; // expired
        }
        return false;
#endif
	}
    // not initialized, assume expired
	return true;
}
// -------------------------------------------------
bool DELAYMS_T::dlyExpiredRestart( void ) {
	if (dlyExpired()) {
		dlyRestart();
		return true;
	}
	return false;
}
bool DELAYMS_T::dlyExpiredRestartSync( void ) {
	if (dlyExpired()) {
		dlyRestartSync();
		return true;
	}
	return false;
}

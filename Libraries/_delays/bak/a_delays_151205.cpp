// a_delays.cpp

// Version 0.1 Aug, 2014
// Copyright(c) 2014 Wong Song Sing
// Electronic & Computer Engineering
// Ngee Ann Polytechnic, Singapore

// Nov 2015: cater for 32-bit ms, max delay 2^32ms=4294967s=1193hr=49.7days

#include "a_delays.h"

#define DLY_USE_US 1    // 1 if micros() will be used, 0 only use millis()
                        // reason: port to other platforms ike Windows with no micros()

#define LMT_FOR_US 10000 // below this, use us, else use ms
// -------------------------------------------------
DELAYMS_T::DELAYMS_T(uint32_t ms ) {
	dlySet(ms);
}
// -------------------------------------------------
void DELAYMS_T::dlySet( uint32_t ms ) {
	// allow to be set again even not expired yet
	// thus suitable for implementing timeout
    if (ms < LMT_FOR_US) {
        startTime = micros(); // start from current time
        dlyinus = true;     // if <1000, use us
        desiredDly = ms*1000; // in us
        if (ms == 0) {
            wasInit = false;
        }
        else {
            wasInit = true;
        }
//        Serial.print('u');
    }
    else {
        startTime = millis(); // start from current time
        dlyinus = false;
        desiredDly = ms; // in ms
        wasInit = true;
//        Serial.print('m');
    }
}
// -------------------------------------------------
// This function prevents accumulative error
void DELAYMS_T::dlySetSync( uint32_t ms ) {
	if (wasInit || (ms == 0)) {
        // Not expired yet! Called again? should not be used this way!
        // Should Expired first before calling this. Nevertheless ...
        // (Do the same for 0 case)
        dlySet(ms); // delay from now; wasInit take care off
	}
	else {
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
    }
}
// -------------------------------------------------
void DELAYMS_T::dlySetSinceOld( uint32_t ms ) {
    dlySetSync(ms);
}
// -------------------------------------------------
bool DELAYMS_T::dlyExpired( void ) {
	if (wasInit) {
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
	}
	else {
		// not initialized, assume expired
		return true;
	}
}
// -------------------------------------------------

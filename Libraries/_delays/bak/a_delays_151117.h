// a_delays.h

// Version 0.1 Aug, 2014
// Copyright(c) 2014 Wong Song Sing
// Electronic & Computer Engineering
// Ngee Ann Polytechnic, Singapore

#ifndef _A_DELAYS_H
#define _A_DELAYS_H

#include <arduino.h>
#include <stdint.h>

// Implementing delay in ms. Micros used to improve accuracy
// Cannot exceed i.e. 4294 seconds (32-bit us used)

#define DELAYMS_T_MAX (0xffffffff/1000uL) // due to use of micros()

class DELAYMS_T {
	private:
		bool wasInit;
        bool dlyinus;
		uint32_t startTimeus; // internally us use for accuracy
		uint32_t desiredDlyus;
	public:
		DELAYMS_T(uint32_t ms = 0);
		// set a delay to expire ms later
		void dlySet( uint32_t ms);
		// set a delay of n ms since last delay expired (prevent accumulative error)
		void dlySetSinceOld( uint32_t ms);
		void dlySetSync( uint32_t ms); // identical to dlySetSinceOld
		// has delay expired?
		bool dlyExpired( void );
};

// Cannot exceed i.e. 4294 seconds (32-bit us used)
class TIMEOUT_T {
	private:
		DELAYMS_T d;
		uint32_t TODur_ms;
	public:
		TIMEOUT_T() {
			TODur_ms=0L;
		}
		TIMEOUT_T(uint32_t ms) {
			timeoutSet(ms);
		}
		bool timeout(void) {
			return d.dlyExpired();
		}
		void timeoutSet(uint32_t ms) {
			d.dlySet(ms);
			TODur_ms=ms;
		}
		void timeoutReset( void ) {
			d.dlySet(TODur_ms);
		}
};

// Cannot exceed 1193 hrs i.e.  (32-bit ms used)
class STOPW_T {
	private:
		uint8_t stopWOn;
		uint32_t stopWStartV;
		uint32_t stopWStopV;
	public:
		STOPW_T();
		void SWStart( void );	// start stop watch
		bool SWStarted( void );	// check if stop watch started already
		uint32_t SWGet( void );		// get stop watch time.
			// If it is still running, get current time lapse
			// if it is stopped, get the time lapse when it was stopped
		uint32_t SWGetRestart( void );		// get stop watch time, stop and start
		uint32_t SWStop( void );	// stop and return the time lapse
};

// Cannot exceed i.e. 4294 seconds (32-bit us used)
class STOPWUS_T {
	private:
		uint8_t stopWUSOn;
		uint32_t stopWUSStartV;
		uint32_t stopWUSStopV;
	public:
		STOPWUS_T();
		void SWUSStart( void );	// start stop watch
		bool SWUSStarted( void );	// check if stop watch started already
		uint32_t SWUSGet( void );		// get stop watch time.
			// If it is still running, get current time lapse
			// if it is stopped, get the time lapse when it was stopped
		uint32_t SWUSStop( void );	// stop and return the time lapse
};

#endif

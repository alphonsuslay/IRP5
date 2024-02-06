// delays.h

// Formerly named a_delays.h

// Version 0.1 Aug, 2014
// Copyright(c) 2014 Wong Song Sing
// Electronic & Computer Engineering
// Ngee Ann Polytechnic, Singapore

// 2017-11: dlySetSync() allow change of delay before expiry
// 		dlyChange() allow change of delay if not expired yet
// 2016-2-12 Added SWResume()
// 2015-12: make it portable to Windows with only millis()
//      dlyRestart() and dlyRestartSync() to do a new delay of the same value after expiry
// 2015-11: cater for 32-bit ms, max delay 2^32ms=4294967s=1193hr=49.7days
// 2019-03: dlyTicked() added

#ifndef __DELAYS_H
#define __DELAYS_H

#define DLY_USE_US 0    // 1 if micros() will be used, 0 only use millis()
                        // reason: port to other platforms like Windows with no micros()

#include <stdint.h>

#ifdef ARDUINO
  #include <Arduino.h>
  #ifdef DLY_USE_US
    #undef DLY_USE_US
  #endif
  #ifdef __AVR__
    #define DLY_USE_US 1    // Arduino should use micros() for smaller ms
  #else // looks like ESP32 micro() is not reliable? Set to 0
    #define DLY_USE_US 0    // Arduino should use micros() for smaller ms
  #endif
#else
  uint32_t millis();
#endif

#define DELAYMS_T_MAX 0xffffffff

class DELAYMS_T {
	private:
		bool wasInit;
#if DLY_USE_US
        bool dlyinus;       // use micros() for smaller ms for accuracy
#endif
		uint32_t startTime;
		uint32_t desiredDly;
	public:
		DELAYMS_T(uint32_t ms = 0);
		// set a delay to expire ms later
		void dlyRestart(); // do a new delay of the same value after expiry
		void dlyRestartSync();
		void dlyReset(); // same as dlyRestart()
		void dlyResetSync(); // same as dlyRestartSync
		void dlyChange(uint32_t ms); // change previous set dly.
			// if expired already, do nothing as its too late!
		void dlySet( uint32_t ms);
		// set a delay of n ms since last delay expired (prevent accumulative error)
		void dlySetSync( uint32_t ms); // identical to dlySetSinceOld
		void dlySetSinceOld( uint32_t ms);

        // has delay expired?
		bool dlyExpired( void );
		bool dlyExpiredRestart( void );
		bool dlyExpiredRestartSync( void );
		bool dlyTicked() {
		    if (dlyExpired()) {
				dlyResetSync();
				return true;
			}
			return false;
		}
};

// max timeout depends on DELAYMS_T
class TIMEOUT_T {
	private:
		DELAYMS_T d;
		bool enabled;
		uint32_t TODur_ms;
	public:
		TIMEOUT_T() {
			TODur_ms=0L;
			enabled = false;
		}
		TIMEOUT_T(uint32_t ms, bool initialEnabled = true) {
			timeoutSet(ms);
			enabled = initialEnabled; // if disabled, enabled by timeoutReset() or timeoutSet()
		}
		bool timeout(void) {
			if (!enabled) return true; // not enabled, thus timeout
			return d.dlyExpired();
		}
		void timeoutSet(uint32_t ms) {
			d.dlySet(ms);
			TODur_ms=ms;
			enabled = true;
		}
		void timeoutReset( void ) {
			d.dlySet(TODur_ms);
			enabled = true;
		}
		void timeoutDisabled() {
			enabled = false;
		}
};

#endif // A_DELAY_H

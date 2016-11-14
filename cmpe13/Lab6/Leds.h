/* 
 * File:   Leds.h
 * Author: Isaiah Solomon
 *
 * Created on May 9, 2014, 12:11 PM
 */

#ifndef LEDS_H
#define	LEDS_H
#include <xc.h>

#define LEDS_INIT() do { \
        TRISE = 0x0; \
        LATE = 0x0; \
} while (0)
#define LEDS_GET()  (LATE)
#define LEDS_SET(x) (LATE = x)

#endif	/* LEDS_H */
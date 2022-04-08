/* 
 * File:   utils.h
 * Author: cbini
 *
 * Created on April 6, 2022, 11:34 AM
 */

#ifndef UTILS_H
#define	UTILS_H

#include "definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t temperature;
    uint8_t humidity;
    uint16_t luminosity;
} dConditions; 

typedef struct {
    uint32_t timestamp;
    uint8_t temperature;
    uint8_t humidity;
    uint16_t luminosityLow;
    uint16_t luminosityHigh;
} cConditions; 

char *itoa(int value);


#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */


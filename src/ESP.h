/* 
 * File:   ESP.h
 * Author: cbini
 *
 * Created on March 29, 2022, 2:14 PM
 */

#ifndef ESP_H
#define	ESP_H

#include "definitions.h"
#include <string.h>
#include "utils.h"


#ifdef	__cplusplus
extern "C" {
#endif
    
#define RX_BUFFER_SIZE  350
    

// A0
dConditions ESP_GetDesiredConditions(uint8_t enclosure, uint8_t hour);

// A1
bool ESP_RecordCurrentConditions(uint8_t enclosure, cConditions conditions);

// A2
uint32_t ESP_CurrentTime(void);

//A3
bool ESP_IPAddress(char *pData, size_t size);

// A4
bool ESP_ChangeDesiredConditions(uint8_t enclosure, dConditions cons[24]);

// A5
bool ESP_Status(void);

// A6
bool ESP_UpdateWifiCredentials(char* ssid, char* password);

// A7
bool ESP_GetAPIKey(char *pData, size_t size);

// A8
bool ESP_ConnectedSSID(char *pData, size_t size);


#ifdef	__cplusplus
}
#endif

#endif	/* ESP_H */


/*******************************************************************************
  Indoor Farming Enclosure - Control Panel: Main Source File

  Company:
    University of Nebraska - Lincoln
    College of Engineering
    Electrical & Computer Engineering
    Capstone Team 108 (2021 - 2022)

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "ESP.h"
#include "utils.h"
#include <time.h>

#define CST     -6
#define ENCLOSURE_1_I2C_ADDRESS     0x54
#define ENCLOSURE_2_I2C_ADDRESS     0x55
#define ENCLOSURE_3_I2C_ADDRESS     0x56


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

static uint8_t wBuffer[20];
static uint8_t rBuffer[20];

time_t t;
struct tm *cTime;
dConditions tempDC;
cConditions tempCC;
bool update = false;


uint16_t ENCLOSURE_Address(uint8_t enclosure) {
    switch(enclosure) {
        case 1:
            return ENCLOSURE_1_I2C_ADDRESS;
        case 2:
            return ENCLOSURE_2_I2C_ADDRESS;
        case 3:
            return ENCLOSURE_3_I2C_ADDRESS;
        default:
            return 0;
    }
}

cConditions ENCLOSURE_CurrentConditions(uint8_t enclosure) {
    cConditions cc;
    
    // Command code
    wBuffer[0] = 1;
    
    if(!I2C1_WriteRead(ENCLOSURE_Address(enclosure), &wBuffer[0], 1, rBuffer, 7)) {
        return cc;
    }
    
    if(rBuffer[0] != 1) {
        return cc;
    }
    
    cc.temperature = rBuffer[1];
    cc.humidity = rBuffer[2];
    cc.luminosityLow = rBuffer[4] << 8 | rBuffer[3];
    cc.luminosityHigh = rBuffer[6] << 8 | rBuffer[5];
    
    return cc;
}

bool ENCLOSURE_SetConditions(uint8_t enclosure, dConditions dc) {
    // Load write buffer with the enclosure desired conditions
    wBuffer[0] = 2;                     // Command code
    wBuffer[1] = dc.temperature;
    wBuffer[2] = dc.humidity;
    wBuffer[3] = dc.luminosity;
    wBuffer[4] = dc.luminosity >> 8;
    
    // Send data to the enclosure
    if(!I2C1_WriteRead(ENCLOSURE_Address(enclosure), wBuffer, 5, rBuffer, 1)) {
        return false;
    }
    
    if(I2C1_ErrorGet() != I2C_ERROR_NONE) {
        return false;
    }
    
    // Check if the enclosure received the data successfully
    if(rBuffer[0] != 2) {
        return false;
    }
    
    // Enclosure conditions set successfully
    return true;
}

bool ENCLOSURE_Update(void) {
    // Get current time 
    t = ESP_CurrentTime();
    cTime = gmtime(&t);
    
    if(t == 0) {
        return false;
    }
    
    // Set conditions for the enclosures
    for(uint8_t i = 1; i < 4; i++) {
        // Get set conditions from SD card
        tempDC = ESP_GetDesiredConditions(i, cTime->tm_hour);
        
        // Send conditions to the enclosure
        if(!ENCLOSURE_SetConditions(i, tempDC)) {
            UART2_Write(&"error\n", 6);
            return false;
        } 
        else {
            UART2_Write(&"succ\n", 5);
        }
        
        CORETIMER_DelayMs(100);
    }
    
    return true;
}

void CONTROLPANEL_Refresh(uint32_t status, uintptr_t contextHandle) {
    update = true;
    UART2_Write(&"Timer\n", 6);
}

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    // Configure timer interrupt - use to update enclosures at set intervals
    CORETIMER_CallbackSet(CONTROLPANEL_Refresh, 0);
    CORETIMER_Start();    

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
        // Update enclosure conditions after timer interrupt is received
        if(update) {
            // Send conditions to the enclosure
            ENCLOSURE_Update();
            update = false;
        }
                
        CORETIMER_DelayMs(100);
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/


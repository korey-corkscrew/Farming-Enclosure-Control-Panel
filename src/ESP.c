#include "ESP.h"

bool ESP_Write(char* wBuffer, size_t wSize);
bool ESP_WriteRead(char* wBuffer, size_t wSize, char* rBuffer, size_t rSize);
bool ESP_Read(char* rBuffer, const size_t rSize);
void ESP_WriteWithCarriage(char *str);
void ESP_WriteUntilNull(char *str);
char* ESP_ParseReceivedData(char* ESPData, uint8_t location);


char receiveBuffer[RX_BUFFER_SIZE] = {};
char data = 0;
char tempBuffer[RX_BUFFER_SIZE] = {};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// A0 - DONE
dConditions ESP_GetDesiredConditions(uint8_t enclosure, uint8_t hour) {
    dConditions dc;
    
    memset(tempBuffer, 0, sizeof(tempBuffer));
    strcpy(tempBuffer, "A0,");
    strcat(tempBuffer, itoa(enclosure));
    strcat(tempBuffer, ",");
    strcat(tempBuffer, itoa(hour));
    strcat(tempBuffer, ",\0");
    
    ESP_WriteRead(tempBuffer, strlen(tempBuffer)+1, tempBuffer, sizeof(tempBuffer));
        
    if(strcmp(ESP_ParseReceivedData(tempBuffer, 0), "A0") != 0) {
        return dc;
    }
    
    dc.temperature = atoi(ESP_ParseReceivedData(tempBuffer, 1));
    dc.humidity = atoi(ESP_ParseReceivedData(tempBuffer, 2));
    dc.luminosity = atoi(ESP_ParseReceivedData(tempBuffer, 3));
    
    return dc;
}

// -----------------------------------------------------------------------------

// A1 - DONE
bool ESP_RecordCurrentConditions(uint8_t enclosure, cConditions conditions) {
    memset(tempBuffer, 0, sizeof(tempBuffer));
    strcpy(tempBuffer, "A1,");
    strcat(tempBuffer, itoa(enclosure));
    strcat(tempBuffer, ",");
    strcat(tempBuffer, itoa(conditions.timestamp));
    strcat(tempBuffer, ",");
    strcat(tempBuffer, itoa(conditions.temperature));
    strcat(tempBuffer, ",");
    strcat(tempBuffer, itoa(conditions.humidity));
    strcat(tempBuffer, ",");
    strcat(tempBuffer, itoa(conditions.luminosityLow));
    strcat(tempBuffer, ",");
    strcat(tempBuffer, itoa(conditions.luminosityHigh));
    strcat(tempBuffer, ",\0");
    
    ESP_WriteRead(tempBuffer, strlen(tempBuffer)+1, tempBuffer, sizeof(tempBuffer));
    
    if(strcmp(ESP_ParseReceivedData(tempBuffer, 0), "A1") != 0) {
        return false;
    }
    if(strcmp(ESP_ParseReceivedData(tempBuffer, 1), "ABC") != 0) {
        return false;
    }
    
    return true;
}

// -----------------------------------------------------------------------------

// A2 - DONE
uint32_t ESP_CurrentTime(void) {
    memset(tempBuffer, 0, sizeof(tempBuffer));
    strcpy(tempBuffer, "A2,\0");
    
    ESP_WriteRead(tempBuffer, strlen(tempBuffer)+1, tempBuffer, sizeof(tempBuffer));
        
    if(strcmp(ESP_ParseReceivedData(tempBuffer, 0), "A2") == 0) {
        return atoi(ESP_ParseReceivedData(tempBuffer, 1));
    }
    
    return 0;
}

// -----------------------------------------------------------------------------

// A3 - DONE 
bool ESP_IPAddress(char *pData, size_t size) {
    memset(tempBuffer, 0, sizeof(tempBuffer));
    strcpy(tempBuffer, "A3,\0");
    
    ESP_WriteRead(tempBuffer, strlen(tempBuffer)+1, tempBuffer, sizeof(tempBuffer));
    
    if(strcmp(ESP_ParseReceivedData(tempBuffer, 0), "A3") != 0) {
        return false;
    }
    
    strncpy(pData, ESP_ParseReceivedData(tempBuffer, 1), size);
    return true;
}

// -----------------------------------------------------------------------------

// A4 - DONE
bool ESP_ChangeDesiredConditions(uint8_t enclosure, dConditions cons[24]) {
    memset(tempBuffer, 0, sizeof(tempBuffer));
    strcpy(tempBuffer, "A4,");
    strcat(tempBuffer, itoa(enclosure));
    strcat(tempBuffer, ",");
    for(int i = 0; i < 24; i++) {
        strcat(tempBuffer, itoa(cons[i].temperature));
        strcat(tempBuffer, ",");
        strcat(tempBuffer, itoa(cons[i].humidity));
        strcat(tempBuffer, ",");
        strcat(tempBuffer, itoa(cons[i].luminosity));
        strcat(tempBuffer, "+");
    }
    
    tempBuffer[strlen(tempBuffer)-1] = '\0';
    
    ESP_WriteRead(tempBuffer, strlen(tempBuffer)+1, tempBuffer, sizeof(tempBuffer));
    
    if(strcmp(ESP_ParseReceivedData(tempBuffer, 0), "A4") != 0) {
        return false;
    }
    if(strcmp(ESP_ParseReceivedData(tempBuffer, 1), "ABC") != 0) {
        return false;
    }
    
    return true;
}

// -----------------------------------------------------------------------------

// A5 - DONE
bool ESP_Status(void) {
    memset(tempBuffer, 0, sizeof(tempBuffer));
    strcpy(tempBuffer, "A5,\0");
    
    ESP_WriteRead(tempBuffer, strlen(tempBuffer)+1, tempBuffer, sizeof(tempBuffer));
        
    if(strcmp(ESP_ParseReceivedData(tempBuffer, 0), "A5") != 0) {
        return false;
    }
    if(strcmp(ESP_ParseReceivedData(tempBuffer, 1), "ABC") != 0) {
        return false;
    }
    
    return true;
}

// -----------------------------------------------------------------------------

// A6 - DONE
bool ESP_UpdateWifiCredentials(char* ssid, char* password) {
    memset(tempBuffer, 0, sizeof(tempBuffer));
    strcpy(tempBuffer, "A6,");
    strcat(tempBuffer, ssid);
    strcat(tempBuffer, ",");
    strcat(tempBuffer, password);
    strcat(tempBuffer, ",\0");
    
    ESP_WriteRead(tempBuffer, strlen(tempBuffer)+1, tempBuffer, sizeof(tempBuffer));
        
    if(strcmp(ESP_ParseReceivedData(tempBuffer, 0), "A6") != 0) {
        return false;
    }
    if(strcmp(ESP_ParseReceivedData(tempBuffer, 1), "ABC") != 0) {
        return false;
    }
    
    return true;
}

// -----------------------------------------------------------------------------

// A7 - DONE
bool ESP_GetAPIKey(char* pData, size_t size) {
    memset(tempBuffer, 0, sizeof(tempBuffer));
    strcpy(tempBuffer, "A7,\0");
    
    ESP_WriteRead(tempBuffer, strlen(tempBuffer)+1, tempBuffer, sizeof(tempBuffer));
    
    if(strcmp(ESP_ParseReceivedData(tempBuffer, 0), "A7") != 0) {
        return false;
    }
    
    strncpy(pData, ESP_ParseReceivedData(tempBuffer, 1), size);
    return true;
}

// -----------------------------------------------------------------------------

// A8 - DONE
bool ESP_ConnectedSSID(char *pData, size_t size) {
    memset(tempBuffer, 0, sizeof(tempBuffer));
    strcpy(tempBuffer, "A8,\0");
    
    ESP_WriteRead(tempBuffer, strlen(tempBuffer)+1, tempBuffer, sizeof(tempBuffer));
    
    if(strcmp(ESP_ParseReceivedData(tempBuffer, 0), "A8") != 0) {
        return false;
    }
    
    strncpy(pData, ESP_ParseReceivedData(tempBuffer, 1), size);
    return true;
}

// -----------------------------------------------------------------------------

bool ESP_Write(char* wBuffer, size_t wSize) {
    return UART1_Write(wBuffer, wSize);
}

// -----------------------------------------------------------------------------

bool ESP_WriteRead(char* wBuffer, size_t wSize, char* rBuffer, size_t rSize) {
    // Write the contents of the write buffer to the ESP
    ESP_Write(wBuffer, wSize);
    
    memset(rBuffer, 0, rSize);
    
    return ESP_Read(rBuffer, rSize);
}

// -----------------------------------------------------------------------------

bool ESP_Read(char* rBuffer, const size_t rSize) {
    // Clear the receive buffer
    memset(receiveBuffer, 0, sizeof(receiveBuffer));
    uint8_t rxCounter=0;
    
    // While ESP is responding
    while ( true )
    {           
        // Check if there is a received character
        if(UART1_ReceiverIsReady() == true)
        {      
            if(UART1_ErrorGet() == UART_ERROR_NONE)
            {                
                
                // Read next byte from ESP
                UART1_Read(&data, 1);

                // Check for end of transmission
                if((data == '\n') || (data == '\0'))
                {
                    // Copy the data received from the ESP into the read buffer
                    strncpy(rBuffer, receiveBuffer, rSize);
                    return true;
                }
                else
                {
                    // Fill the receive buffer
                    receiveBuffer[rxCounter++] = data;
                }
            }
            else
            {       
                // Transmission error
                return false;
            }
        }
    }
}

// -----------------------------------------------------------------------------

void ESP_WriteWithCarriage(char *str) {
    ESP_WriteUntilNull(str);
    ESP_Write(",", 1);
}

// -----------------------------------------------------------------------------

void ESP_WriteUntilNull(char *str) {
    for(int i = 0; str[i] != '\0'; i++) {
        ESP_Write(&str[i], 1);
    }
}

// -----------------------------------------------------------------------------

char* ESP_ParseReceivedData(char* ESPData, uint8_t location) {
    uint8_t delimiterCount = 0;
    uint8_t charCount = 0;
    char temp = 0;
    static char tempString[15] = {};
    uint8_t tempCount = 0;
    
    memset(tempString, 0, 15);
        
    while(delimiterCount < location) {
        temp = ESPData[charCount++];
        if(temp == ',' || temp == '\n' || temp == '\0') {
            delimiterCount++;
        }
    }
    
    temp = ESPData[charCount++];
    
    while(temp != ',' && temp != '\n' && temp != '\0') {
        tempString[tempCount++] = temp;
        temp = ESPData[charCount++];
    }
    
    tempString[tempCount] = '\0';
    return tempString;
}
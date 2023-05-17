/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

#define KBD_DELAY   5


void keypadInit (void) {
    COL1_Write(1);
    COL2_Write(1);
    COL3_Write(1);
}

uint8_t keypadScan (void) {
    
    COL2_Write(0);
    //CyDelay(KBD_DELAY);
    if (!ROW0_Read()) {
        COL2_Write(1);
        return('0');
    }
    if (!ROW1_Read()) {
        COL2_Write(1);
        return('8');
    }
    if (!ROW3_Read()) {
        COL2_Write(1);
        return('2');
    }
    COL2_Write(1);

    
    return('z');
}


/* [] END OF FILE */

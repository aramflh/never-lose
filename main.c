//
// Created by Aramson Felho on 10/05/2023.
//
#include <math.h>

#include "keypad.h"  // Keypad header file
#include "utils.h"   // Other functions header file

// Interrupt management
CY_ISR (myISR){
    // Place ISR code here
}

int main(void)
{
    CyGlobalIntEnable;  /* Enable global interrupts. */

    /* Place your initialization/startup code*/

    keypadInit();


    // Checking buttons states

    char key                = 'z';



    for(;;)
    {
        /* Place your application code here. */


    }
}



// Keyboard detection
void detectKeyboard(char* key, char* last_key){
    // Detect the char pressed on the keybpard
    *key = keypadScan();

    if (*key != 'z'){
        if (*key == '2'){
            // Call "JUMP" fuction
        }
        else if (*key == '8') {
            // Call "DOWN" fuction

        }
    }
}

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
    LCD_Start();
    LCD_ClearDisplay();
    Mux_Start();
    ADC_Start();  // Start the Analog-to-Digital Converter

    keypadInit();  // Call initialization function form keypad.h


    // Checking buttons states

    char key = 'z';



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

// Light detection
void detectLight(uint32_t* photoResVal){

    Mux_Select(0);
    CyDelay(10); //This one let some time for the switch to occur. Otherwise conversion does not work properly
    ADC_StartConvert();

    CyDelay(10);
    ADC_StartConvert();
    if(ADC_IsEndConversion(ADC_WAIT_FOR_RESULT)){
        *photoResVal = ADC_GetResult32();
    }
    if(*photoResVal > 0 && *photoResVal <= 10000){
        // BLACK
    }
    else if(*photoResVal < 10000){
        // WHITE
    }
}



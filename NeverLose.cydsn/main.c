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

// Global vaiables
uint8_t score = 0;

int main(void)
{
    CyGlobalIntEnable;  /* Enable global interrupts. */

    /* Place your initialization/startup code*/
    LCD_Start();
    LCD_ClearDisplay();
    Mux_Start();
    ADC_Start();  // Start the Analog-to-Digital Converter
    keypadInit();  // Call initialization function form keypad.h

    // Variables initialization

    uint8_t sw3_pressed = 0;  // Reset button

    // Checking buttons states

    char key = 'z';



    for(;;)
    {
        /* Place your application code here. */

        // Printing the score
        LCD_Position(0,0);
        LCD_PrintNumber(*score);


    }
}

// Reset the score
/*
 * Components name : Button 3 = SW3
 */
void resetScore(uint8_t* sw3_pressed, uint8_t* score){
/* Assignment 3 : here we detect a rising edge on SW2 and display the number
    of time SW2 has been pressed using ctr */

    if (SW3_Read()){
        *sw3_pressed = 1;
    }
    if ((!SW3_Read()) && *sw3_pressed){
        *sw3_pressed = 0;
        (*score) = 0;
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
/*
 * Components name : ADC = ADC
 */
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
        // Call "JUMP" fuction
    }
    else if(*photoResVal < 10000){
        // WHITE
        // Call "DOWN" fuction
    }
}



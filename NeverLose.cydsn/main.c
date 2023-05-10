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

    // Variables initialization

    uint8_t score = 0;
    uint8_t first_jump = 1;
    uint8_t day = 0;
    uint8_t night = 0;
    uint32_t photoResVal1 = 0;
    uint32_t photoResVal2 = 0;
    char key = 'z';



    for(;;)
    {
        /* Place your application code here. */

        // TODO: Increase the score over time (10 points per seconds)
        // Printing the score on the second half of the LCD
        LCD_Position(1,0);
        LCD_PrintNumber(*score);

        // Button detection
        /*
         * Components name: SW1, SW2, SW3
         */
        if (SW1_Read()){
           jump(&first_jump);
        }
        else if (SW2_Read()) {
            duck();

        } else if (SW3_Read()) {
            // Reset the score
            (*score) = 0;

        }
        // Light detection
        detectLight(&photoResVal1, &photoResVal2, &day, &night);

        // Keypad detection
        detectKeyboard(&key);

        // Shun-ting down  the LEDs
        LED1_Write(0);
        LED2_Write(0);
        LED3_Write(0);
        LED4_Write(0);

    }
}

// Jump function
void jump(uint8_t* first_jump){
    if (*first_jump){
        (*first_jump) = 0;
    }
    // Printing on LCD
    LCD_Position(0,0);
    LCD_PrintString("JUMP");
    // Lighting LEDs
    LED1_Write(1);
    LED2_Write(1);
}
// Down function
void duck(){
    // Printing on LCD
    LCD_Position(0,0);
    LCD_PrintString("DUCK");
    // Lighting LEDs
    LED3_Write(1);
    LED4_Write(1);
}


// Keyboard detection
void detectKeyboard(char* key){
    // Detect the char pressed on the keybpard
    *key = keypadScan();

    if (*key != 'z'){
        if (*key == '2'){
            jump();
        }
        else if (*key == '8') {
            duck();
        }
    }
}


// Light detection
/*
 * Components name : ADC
 */
void detectLight(uint32_t* photoResVal1, uint32_t* photoResVal2,uint8_t* day = 0, uint8_t* night = 0 ){

    Mux_Select(0);
    CyDelay(10); //This one let some time for the switch to occur. Otherwise conversion does not work properly
    ADC_StartConvert();
    if(ADC_IsEndConversion(ADC_WAIT_FOR_RESULT)){
        *photoResVal1 = ADC_GetResult32();
    }
    Mux_Select(1);
    CyDelay(10);
    ADC_StartConvert();
    if(ADC_IsEndConversion(ADC_WAIT_FOR_RESULT)){
        *photoResVal2 = ADC_GetResult32();
    }


    // TODO: Calculate the speed delta
    if(*photoResVal1 > 0 && *photoResVal1 <= 10000 && *photoResVal1 != *photoResVal2 ){
        // Top: White, Bottom: Black
        jump();
    }
    else if(*photoResVal2 > 0 && *photoResVal2 <= 10000 && *photoResVal1 != *photoResVal2 ){
        // Top: Black, Bottom: White
        duck();
    }

    // Checking the cycle of the day
    if(*photoResVal2 > 10000 && (*photoResVal1 - *photoResVal2) <= 100 ){
        // Top: White, Bottom: White
        // Day
        (*day) = 1;
        (*night) = 0;

    }
    else if (*photoResVal1 > 0 && *photoResVal1 <= 10000 && (*photoResVal1 - *photoResVal2) <= 100) {
        // Top: Black, Bottom: Black
        // Night
        (*day) = 0;
        (*night) = 1;

    }

}



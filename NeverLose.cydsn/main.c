//
// Created by Aramson Felho on 10/05/2023.
//
#include <math.h>

//#include "keypad.h"  // Keypad header file
#include "utils.h"   // Other functions header file
#include "project.h"
#include "keypad.h"
#include <stdint.h>

#define PWM_VALUE 5000  // hasarg
#define COLOR_CHANGE 10000  // The value at wich the color change on the screen
#define COLOR_DELTA 100  // The minimum value to ignore the delta between de 2 photoresistor


uint8 rxData;
/*
CY_ISR(isr_uart_Handler){
        uint8 status = 0;
        do{
            // Check that rx buffer is not empty and get rx data
            if ( (status & UART_RX_STS_FIFO_NOTEMPTY) != 0){
                rxData = UART_ReadRxData();
                UART_PutChar(rxData);
                if (rxData == "jump") {
                    jump();
                }else if (rxData == "duck") {
                    duck();
                }
            }
        }while ((status & UART_RX_STS_FIFO_NOTEMPTY) != 0);

}
*/
int main(void)
{
    CyGlobalIntEnable;  /* Enable global interrupts. */

    /* Place your initialization/startup code*/
    int score = 0;
    uint8_t first_jump = 1;
    uint8_t day = 0;
    uint8_t night = 0;
    uint16_t pwm_period = 48000;
    uint32_t photoResVal1 = 0;
    uint32_t photoResVal2 = 0;
    //uint32_t val_CMP;
    //uint32_t val_adc;
    char key = 'z';
    uint16_t cnt = 0;

    PWM1_WritePeriod(pwm_period);
    PWM2_WritePeriod(pwm_period);
    //isr_uart_StartEx(isr_uart_Handler);


    LCD_Start();
    LCD_ClearDisplay();
    //Mux_Start();
    ADC_Start();  // Start the Analog-to-Digital Converter
    UART_Start(); //Start the UART
    keypadInit();  // Call initialization function form keypad.h
    PWM1_Start();
    PWM2_Start();
    PWM1_WriteCompare(3000); //Servo à l'horizontal
    PWM2_WriteCompare(3000); //Servo à l'horizontal
    
    Timer_Start();

    for(;;)
    {
        /* Place your application code here. */

        // TODO: Increase the score over time (10 points per seconds)
       /* 
        if(0x80 & Timer_ReadStatusRegister()){ //In case of overflow
            if (cnt < 1000){
                cnt++;
            }
            else{
                LED1_Write(!LED1_Read());
                cnt = 0; //Reset counter
            }
        }
        */
        
        // Printing the score on the second half of the LCD
        LCD_Position(1,0);
        LCD_PrintNumber(score);

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
            LCD_ClearDisplay();
           (score) = 0;
           (first_jump) = 1;

        }
        //
        
        
        
        // Light detection
        //detectLight(&photoResVal1, &photoResVal2, &day, &night);

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
    //Activate servo motor 1
    
    // A Changer
    
    PWM1_WriteCompare(1700);
    CyDelay(200); 
    PWM1_WriteCompare(3000);
    
    // Printing "Jump" on LCD
    LCD_Position(0,0);
    LCD_PrintString("Jump");
    // Lighting LEDs (1&2)
    LED1_Write(1);
    LED2_Write(1);
    CyDelay(500);
    // Audio Output 1
    // TODO
}
// Down function
void duck(){
    //Activate servo motor 2
    
    // A Changer
    
    PWM2_WriteCompare(1700);
    CyDelay(200); 
    PWM2_WriteCompare(3000);
    
    // Printing "Duck" on LCD
    LCD_Position(0,0);
    LCD_PrintString("Duck");
    // Lighting LEDs (3&4)
    LED3_Write(1);
    LED4_Write(1);
    CyDelay(200);
    // Audio Output 2
    // TODO
}


// Keyboard detection

void detectKeyboard(char* key){
    // Detect the char pressed on the keybpard
    *key = keypadScan();

    if (*key != 'z'){
        LED1_Write(1);
        if (*key == '2'){
            jump(0); //attention, on hardcode un zero comme si c'etait pas un first jump
        }
        else if (*key == '8') {
            duck();
        }
    }
    else if(*key == 'z'){
        LED2_Write(1);
    }
}


// Light detection
/*
 * Components name : ADC
 */
/*
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


    if(*photoResVal1 > 0 && *photoResVal1 <= COLOR_CHANGE && (*photoResVal1 - *photoResVal2) > COLOR_DELTA ){
        // Top: White, Bottom: Black
        jump();
    }
    else if(*photoResVal2 > 0 && *photoResVal2 <= COLOR_CHANGE && (*photoResVal1 - *photoResVal2) > COLOR_DELTA ){
        // Top: Black, Bottom: White
        duck();
    }

    // Checking the cycle of the day
    if(*photoResVal2 > COLOR_CHANGE && (*photoResVal1 - *photoResVal2) <= COLOR_DELTA ){
        // Top: White, Bottom: White
        // Day
        (*day) = 1;
        (*night) = 0;
        // Printing the cycle on the LCD
        LCD_Position(0,5);
        LCD_PrintString("D");

    }
    else if (*photoResVal1 > 0 && *photoResVal1 <= COLOR_CHANGE && (*photoResVal1 - *photoResVal2) <= COLOR_DELTA) {
        // Top: Black, Bottom: Black
        // Night

        (*day) = 0;
        (*night) = 1;
        // Printing the cycle on the LCD
        LCD_Position(0,5);
        LCD_PrintString("N");

    }

}
*/


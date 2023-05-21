// TODO: Try to avoid global variables
#include <math.h>
#include <stdint.h>
#include "project.h"
#include "keypad.h"

// Constants
#define PI 3.14
#define N 100
const uint16_t START_DELAY = 600;
const uint16_t PWM_PERIOD = 48000;
const uint16_t MOTOR_HORIZ = 3000;
const uint16_t MOTOR_PRESS = 2000;
const uint32_t RES_VAL_1 = 28000; // A verifier
const uint32_t RES_VAL_2 = 28000; // A verifier

// Global varialbes: 'g_...'
float g_signal1[N];
float g_signal2[N];
uint16_t g_soundCounter = 0;


int jumpDelay = START_DELAY;
int score = 0; // OK ? (voir ligne )
uint8 rxData; // TODO


uint8_t g_dac_value;
uint8_t startGame = 0;
uint8_t moved = 0;
uint8_t mux_selector = 0;


uint16_t scoreTimer = 0; // OK
// iniStatesTimer delays the time before returning to the stable state after a movement
uint16_t iniStatesTimer = 0; // OK
uint16_t jumpTimer = 0; // OK
uint16_t cnt5 = 0;
uint32_t photoResVal1 = 0;
uint32_t photoResVal2 = 0;
uint32_t val_CMP;
uint32_t val_adc;


/*
CY_ISR(isr_uart_Handler){
        uint8 status = 0;
        do{
            // Check that rx buffer is not empty and get rx data
            if ( (status & UART_RX_STS_FIFO_NOTEMPTY) != 0){
                rxData = UART_GetChar();
                //UART_PutChar(rxData);
                if (rxData == "jump") {
                    LED1_Write(1);
                }else if (rxData == "duck") {
                    LED1_Write(0);
                }
                else{
                    LCD_Position(0,0);
                    LCD_PrintInt8(rxData);}
            }
        }while ((status & UART_RX_STS_FIFO_NOTEMPTY) != 0);

}*/


/**
 * Adds 10 to the value of score every second.
 * The delay before jumping decrease of 100 every 100 points
 * Formula: Area = PI*r^2
 * @param[in] *scoreTimer
 * @param[in] *score
 */
void start_score_counter(uint16_t *scoreTimer, int *score){
    if (*scoreTimer < 1000){
        *scoreTimer++;
    } else {
        *score += 10;
        if(*score%100 == 0){
            // /!\ delay is int => Possible to have neg number ?
            // TODO: Change to uint or put a while
            jumpDelay -= 100;
        }
        *scoreTimer = 0;
    }
}

/**
 * Deactivates the LED, clear the LCD display, put back the servo motor to their initial states
 * and set the sound flags to '0' 200 milliseconds after un movement
 * @param[in] *iniStatesTimer
 */
void initial_state(uint16_t *iniStatesTimer){
        if (*iniStatesTimer < 200){
            *iniStatesTimer++;
        }
        else {
            LED1_Write(0);
            LED2_Write(0);
            LED3_Write(0);
            LED4_Write(0);
            LCD_ClearDisplay();
            PWM1_WriteCompare(MOTOR_HORIZ);
            PWM2_WriteCompare(MOTOR_HORIZ);
            *iniStatesTimer = 0;
            moved = 0 ;
        }
}

/**
 * Generates sound
 * @param[in] *signal
 */
void make_sound(array signal){
    if(0x80 & Timer_DAC_ReadStatusRegister()){
        g_dac_value = 128 + 128*signal[*g_soundCounter];
        DAC_SetValue(g_dac_value);
        *g_soundCounter++;
        if (*g_soundCounter==N){*g_soundCounter=0;}
        Timer_DAC_ReadStatusRegister();
    }
}

/**
 * Prints "Jump" on the LCD display, lights LED1 & 2, generate sound and turn on the servo motor
 * @param[in] *moved
 */
void jump(uint8_t *moved){

    PWM1_WriteCompare(MOTOR_PRESS);
    LCD_Position(0,0);
    LCD_PrintString("Jump");
    LED1_Write(1);
    LED2_Write(1);
    make_sound(g_signal1);
    (*moved) = 1 ;
}

/**
 * Prints "Duck" on the LCD display, lights LED3 & 24, generate sound and turn on the servo motor
 * @param[in] *moved
 */
void duck(uint8_t *moved){

    PWM2_WriteCompare(MOTOR_PRESS);
    LCD_Position(0,0);
    LCD_PrintString("Duck");
    LED3_Write(1);
    LED4_Write(1);
    make_sound(g_signal2);
    (*moved) = 1 ;
}

/**
 * Prints "Duck" on the LCD display, lights LED3 & 24, generate sound and turn on the servo motor
 * @param[in] *moved
 */
uint8_t objDetected(uint8_t mux_selector, uint32_t VAL){

    Mux_Select(mux_selector);
    // TODO: ???
    //CyDelay(10);
    ADC_StartConvert();
    if(ADC_IsEndConversion(ADC_WAIT_FOR_RESULT)){
        photoResVal = ADC_GetResult32();
    }
    if (photoResVal < VAL) {
        return 1;
    } else {
        return 0;
    }
}

int main(void)
{
    CyGlobalIntEnable;  /* Enable global interrupts. */

    /* Variable declaration and initialization*/

    /*Startup code*/

    LCD_Start();
    LCD_ClearDisplay();
    ADC_Start();
    DAC_Start();
    DAC_SetValue(0);
    Mux_Start();
    UART_Start();
    keypadInit();
    PWM1_WritePeriod(PWM_PERIOD);
    PWM2_WritePeriod(PWM_PERIOD);
    PWM1_Start();
    PWM2_Start();
    PWM1_WriteCompare(MOTOR_HORIZ);
    PWM2_WriteCompare(MOTOR_HORIZ);
    Timer_Start();
    Timer_DAC_Start();
    
    // Interupt
    //isr_uart_StartEx(isr_uart_Handler);
    
    //Sound
    for (int j=0; j<N; j++){
        g_signal1[j] = sin(50*j/N);
        g_signal2[j] = sin(200*j/N);
    }
    
    // LOOP
    for(;;)
    {
        LCD_Position(1,0);
        LCD_PrintNumber(score);
        // In case of overflow
        if(0x80 & Timer_ReadStatusRegister()){
            if (startGame) {
                start_score_counter(&scoreTimer, &score);
            }
            if (moved) {
                initial_state(*iniStatesTimer)
            }

            if(objDetected(0, RES_VAL_1)){
                if (jumpTimer < jumpDelay){
                    jumpTimer++;
                }
                else{
                    jump(&moved);
                    jumpTimer = 0;

                }
            } else if ( objDetected(1, RES_VAL_2) )
            {
                duck(&moved);
            }
        }
            if (keypadScan() == '2' || SW1_Read() ) {
                jump(&moved);
            } else if (keypadScan() == '8' || SW2_Read() ) {
                duck(&moved);
            } else if (SW3_Read()) {
                LCD_ClearDisplay();
                score = 0;
                startGame = 0;
                jumpDelay = START_DELAY;
            }
        }


}


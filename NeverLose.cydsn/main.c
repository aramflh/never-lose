/*
 * [ELEC-H-310] Digital Electronics
 * Project: Never-lose
 * Authors:
 * Mushingelete FELHO 574088
 * Ayoub TOUHAMI 575007
 * Clément ALLOIN 573143
 * Baptiste BEMELMANS 581037
 *
 */
#include <math.h>

#include "project.h"
#include "keypad.h"   
#include <stdint.h>


#define PI 3.14
#define N 100

float signal1[N];
float signal2[N];


uint8 rxData;

uint8_t sound1 = 0;
uint8_t sound2 = 0;
uint16_t i = 0;
uint8_t value;

uint16_t pwmCounter = 0;

uint8_t isDucking = 0;
uint8_t isJumping = 0;

uint8_t uartDucking = 0;
uint8_t uartJumping = 0;

uint8_t firstJump = 0;
int score = 0;
uint16_t scoreCounter = 0;

uint8_t cactusDetected = 0;
uint8_t birdDetected = 0;

uint16_t pwm_period = 48000;

uint32_t photoResVal1 = 0;
uint32_t photoResVal2 = 0;


// Jump function
void jump(uint8_t* isJumping ){
    if (!firstJump){firstJump = 1;}
    (*isJumping) = 1 ;
    UART_PutString("JUMP\r\n");
    PWM1_WriteCompare(2000);
    LED1_Write(1);
    LED2_Write(1);
    sound1 = 1;
}
// Down function
void duck(uint8_t* isDucking){
    (*isDucking) = 1 ;
    UART_PutString("DUCK\r\n");
    PWM2_WriteCompare(2000);
    sound2 = 1;
    LED3_Write(1);
    LED4_Write(1);
}

CY_ISR(duckingHandler) {
    if (isDucking) {
        pwmCounter ++;
        if (pwmCounter == 300){      
            PWM2_WriteCompare(2700);
            pwmCounter = 0;
            isDucking = 0;
        }
    }
    Timer_ReadStatusRegister();
}


CY_ISR(isrUartHandler){
    uint8 status = 0;

    do{
        // Checks if no UART Rx errors
        status = UART_ReadRxStatus();
        if ((status & UART_RX_STS_PAR_ERROR) || (status & UART_RX_STS_STOP_ERROR) || (status & UART_RX_STS_BREAK) || (status & UART_RX_STS_OVERRUN) ) {
            // Parity, framing, break or overrun error
            // ... process error
            LCD_Position(1,0);
            LCD_PrintString("UART err");
        }
        // Check that rx buffer is not empty and get rx data
        if ( (status & UART_RX_STS_FIFO_NOTEMPTY) != 0){
            rxData = UART_GetChar();
            if (rxData == 'j'){
                uartJumping = 1;   
            }
            
            if (rxData == 'd'){
                uartDucking = 1;
            }
        }
    }while ((status & UART_RX_STS_FIFO_NOTEMPTY) != 0);

    
}

CY_ISR(soundHandler){
    
    if (sound1) {
        value = 128 + 128*signal1[i];
        DAC_SetValue(value);
        i++;
        if (i==N){i=0;}
    }
    else if (sound2){
        value = 128 + 128*signal2[i];
        DAC_SetValue(value);
        i++;
        if (i==N){i=0;}
    } 
    Timer_DAC_ReadStatusRegister();
    
    
}

CY_ISR(scoreHandler){
    if (firstJump) {
        scoreCounter ++;
        if (scoreCounter == 1000){      
            score += 10;
            scoreCounter = 0;
        }
    }
    Timer_ReadStatusRegister();

}
    
    


// Light detection
/*
 * Components name : ADC
 */

void detectCactus(uint32_t* photoResVal1, uint8_t* CactusDetected){

    ADC_StartConvert();
    if(ADC_IsEndConversion(ADC_WAIT_FOR_RESULT)){
        *photoResVal1 = ADC_GetResult32();
    }
    
    if(*photoResVal1 < 30000){
        // Top: White, Bottom: Black
        *CactusDetected = 0;
    }
}

void detectBird(uint32_t* photoResVal2, uint8_t* birdDetected){

    ADC_StartConvert();
    if(ADC_IsEndConversion(ADC_WAIT_FOR_RESULT)){
        *photoResVal2 = ADC_GetResult32();
    }
   

    if(*photoResVal2 < 10000){
        // Top: White, Bottom: Black
        *birdDetected =0;
    }
}

int main(void)
{
    CyGlobalIntEnable;  /* Enable global interrupts. */

    /* Place your initialization/startup code*/
    PWM1_WritePeriod(pwm_period);
    PWM2_WritePeriod(pwm_period);


    LCD_Start();
    LCD_ClearDisplay();
    Mux_Start();
    ADC_Start();  // Start the Analog-to-Digital Converter
    UART_Start(); //Start the UART
    keypadInit();  // Call initialization function form keypad.h
    PWM1_Start();
    PWM2_Start();
    PWM1_WriteCompare(2700); //Servo à l'horizontal
    PWM2_WriteCompare(2700); //Servo à l'horizontal
    
    Timer_Start();
    DAC_Start();
    DAC_SetValue(0);
    Timer_DAC_Start();
    
    isr_1_StartEx(duckingHandler);
    isr_uart_StartEx(isrUartHandler);
    isr_2_StartEx(soundHandler);
    isr_3_StartEx(scoreHandler);
    
    
    for (int j=0; j<N; j++){
        signal1[j] = sin(50*j/N);
        signal2[j] = sin(200*j/N);
    }

    for(;;)
    {
        LCD_Position(1,0);
        LCD_PrintNumber(score);
        
        
        if (isJumping) {
            LED1_Write(0);
            LED2_Write(0);
            sound1 = 0;

            PWM1_WriteCompare(2700);
            isJumping = 0;
        }
        
        if(isDucking) {
            LED3_Write(0);
            LED4_Write(0);
            sound2 = 0;
            
        }
        
        if(cactusDetected || SW1_Read() ||uartJumping || keypadScan() == '8'){
            jump(&isJumping);
            cactusDetected = 0;
            uartJumping = 0;
        }
        
        if (birdDetected || SW2_Read() ||uartDucking || keypadScan() == '2' ){
            duck(&isDucking);
            birdDetected = 0;
            uartDucking = 0;
        }
        
         if (SW3_Read()) {
            // Reset the score
            LCD_ClearDisplay();
            score = 0;
            firstJump = 0;
        }
        
        // Cactus detection
        Mux_Select(0);
        CyDelay(10);
        detectCactus(&photoResVal1, &cactusDetected);
        
        //Bird detection
        Mux_Select(1);
        CyDelay(10);
        detectBird(&photoResVal2, &birdDetected);

    }
}






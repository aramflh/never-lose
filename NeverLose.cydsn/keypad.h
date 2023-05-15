//
// Created by Aramson Felho on 10/05/2023.
//



#ifndef NEVER_LOSE_KEYPAD_H
#define NEVER_LOSE_KEYPAD_H

#define KBD_DELAY   5
#include <stdint.h>

void keypadInit (void) {
    COL1_Write(1);
    COL2_Write(1);
    COL3_Write(1);
}

uint8_t keypadScan (void) {
    COL1_Write(0);
    CyDelay(KBD_DELAY);
    if (!ROW0_Read()) {
        COL1_Write(1);
        return('#');
    }
    if (!ROW1_Read()) {
        COL1_Write(1);
        return('9');
    }
    if (!ROW2_Read()) {
        COL1_Write(1);
        return('6');
    }
    if (!ROW3_Read()) {
        COL1_Write(1);
        return('3');
    }
    COL1_Write(1);

    COL2_Write(0);
    CyDelay(KBD_DELAY);
    if (!ROW0_Read()) {
        COL2_Write(1);
        return('0');
    }
    if (!ROW1_Read()) {
        COL2_Write(1);
        // "DOWN"
        return('8');
    }
    if (!ROW2_Read()) {
        COL2_Write(1);
        return('5');
    }
    if (!ROW3_Read()) {
        COL2_Write(1);
        // "JUMP"
        return('2');
    }
    COL2_Write(1);

    COL3_Write(0);
    CyDelay(KBD_DELAY);
    if (!ROW0_Read()) {
        COL3_Write(1);
        return('*');
    }
    if (!ROW1_Read()) {
        COL3_Write(1);
        return('7');
    }
    if (!ROW2_Read()) {
        COL3_Write(1);
        return('4');
    }
    if (!ROW3_Read()) {
        COL3_Write(1);
        return('1');
    }
    COL3_Write(1);

    return('z');
}


#endif //NEVER_LOSE_KEYPAD_H

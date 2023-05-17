/*******************************************************************************
* File Name: pot.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_pot_H) /* Pins pot_H */
#define CY_PINS_pot_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "pot_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 pot__PORT == 15 && ((pot__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    pot_Write(uint8 value);
void    pot_SetDriveMode(uint8 mode);
uint8   pot_ReadDataReg(void);
uint8   pot_Read(void);
void    pot_SetInterruptMode(uint16 position, uint16 mode);
uint8   pot_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the pot_SetDriveMode() function.
     *  @{
     */
        #define pot_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define pot_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define pot_DM_RES_UP          PIN_DM_RES_UP
        #define pot_DM_RES_DWN         PIN_DM_RES_DWN
        #define pot_DM_OD_LO           PIN_DM_OD_LO
        #define pot_DM_OD_HI           PIN_DM_OD_HI
        #define pot_DM_STRONG          PIN_DM_STRONG
        #define pot_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define pot_MASK               pot__MASK
#define pot_SHIFT              pot__SHIFT
#define pot_WIDTH              1u

/* Interrupt constants */
#if defined(pot__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in pot_SetInterruptMode() function.
     *  @{
     */
        #define pot_INTR_NONE      (uint16)(0x0000u)
        #define pot_INTR_RISING    (uint16)(0x0001u)
        #define pot_INTR_FALLING   (uint16)(0x0002u)
        #define pot_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define pot_INTR_MASK      (0x01u) 
#endif /* (pot__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define pot_PS                     (* (reg8 *) pot__PS)
/* Data Register */
#define pot_DR                     (* (reg8 *) pot__DR)
/* Port Number */
#define pot_PRT_NUM                (* (reg8 *) pot__PRT) 
/* Connect to Analog Globals */                                                  
#define pot_AG                     (* (reg8 *) pot__AG)                       
/* Analog MUX bux enable */
#define pot_AMUX                   (* (reg8 *) pot__AMUX) 
/* Bidirectional Enable */                                                        
#define pot_BIE                    (* (reg8 *) pot__BIE)
/* Bit-mask for Aliased Register Access */
#define pot_BIT_MASK               (* (reg8 *) pot__BIT_MASK)
/* Bypass Enable */
#define pot_BYP                    (* (reg8 *) pot__BYP)
/* Port wide control signals */                                                   
#define pot_CTL                    (* (reg8 *) pot__CTL)
/* Drive Modes */
#define pot_DM0                    (* (reg8 *) pot__DM0) 
#define pot_DM1                    (* (reg8 *) pot__DM1)
#define pot_DM2                    (* (reg8 *) pot__DM2) 
/* Input Buffer Disable Override */
#define pot_INP_DIS                (* (reg8 *) pot__INP_DIS)
/* LCD Common or Segment Drive */
#define pot_LCD_COM_SEG            (* (reg8 *) pot__LCD_COM_SEG)
/* Enable Segment LCD */
#define pot_LCD_EN                 (* (reg8 *) pot__LCD_EN)
/* Slew Rate Control */
#define pot_SLW                    (* (reg8 *) pot__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define pot_PRTDSI__CAPS_SEL       (* (reg8 *) pot__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define pot_PRTDSI__DBL_SYNC_IN    (* (reg8 *) pot__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define pot_PRTDSI__OE_SEL0        (* (reg8 *) pot__PRTDSI__OE_SEL0) 
#define pot_PRTDSI__OE_SEL1        (* (reg8 *) pot__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define pot_PRTDSI__OUT_SEL0       (* (reg8 *) pot__PRTDSI__OUT_SEL0) 
#define pot_PRTDSI__OUT_SEL1       (* (reg8 *) pot__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define pot_PRTDSI__SYNC_OUT       (* (reg8 *) pot__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(pot__SIO_CFG)
    #define pot_SIO_HYST_EN        (* (reg8 *) pot__SIO_HYST_EN)
    #define pot_SIO_REG_HIFREQ     (* (reg8 *) pot__SIO_REG_HIFREQ)
    #define pot_SIO_CFG            (* (reg8 *) pot__SIO_CFG)
    #define pot_SIO_DIFF           (* (reg8 *) pot__SIO_DIFF)
#endif /* (pot__SIO_CFG) */

/* Interrupt Registers */
#if defined(pot__INTSTAT)
    #define pot_INTSTAT            (* (reg8 *) pot__INTSTAT)
    #define pot_SNAP               (* (reg8 *) pot__SNAP)
    
	#define pot_0_INTTYPE_REG 		(* (reg8 *) pot__0__INTTYPE)
#endif /* (pot__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_pot_H */


/* [] END OF FILE */

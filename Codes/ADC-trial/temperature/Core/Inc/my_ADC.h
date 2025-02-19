/*
 * ESPE_ADC3
 * libreria contenente tutte le funzioni necessarie per la lettura
 * di una temperatura
 *
 * in caso ci sia il bisogno di leggere dati da un altro sensore, cambiare i codici dai pin
 *
 *
 */


#ifndef __my_ADC_H__
#define __my_ADC_H__

#endif

#include "adc.h"

#define len_vec 50
#define TCAL_30C *(uint16_t *)(0x1FF1E820)
#define TCAL_110C *(uint16_t *)(0x1FF1E840)
#define TS_diff (uint32_t)(TCAL_110C-TCAL_30C)
#define Const1 (uint32_t)(80000)
#define Const2 (uint32_t)(30000)


#define conversion

void ESPE_ADC_init(uint8_t cont_measurement);
void ESPE_ADC_vec_read(void);
void ESPE_ADC_temp_conversion(void);
void ESPE_ADC_TIM6_setup(void);
uint8_t ESPE_ADC_loop_vec_read(uint8_t trigger);
uint16_t temp_conversion(uint8_t indice);

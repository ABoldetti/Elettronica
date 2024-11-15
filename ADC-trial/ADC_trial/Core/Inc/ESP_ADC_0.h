/*
 * ESP_ADC.h
 *
 *  Created on: Nov 6, 2023
 *      Author: utente
 */

#ifndef INC_ESP_ADC_H_
#define INC_ESP_ADC_H_



#endif /* INC_ESP_ADC_H_ */

#include "adc.h"


#define TCAL_30C *(uint16_t *)(0x1FF1E820)
#define TCAL_110C *(uint16_t *)(0x1FF1E840)
#define TS_diff (uint32_t)(TCAL_110C-TCAL_30C)
#define Const1 (uint32_t)(80000)
#define Const2 (uint32_t)(30000)




void ESPE_ADC_0_init(void);
void ESPE_ADC_data(void);
void ESPE_ADC_temp_conversion(void);



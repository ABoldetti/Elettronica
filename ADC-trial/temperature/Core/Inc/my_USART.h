/*
 * ESPE_USART3
 * libreria contenente tutte le funzioni necessarie per il passaggio seriale
 *
 *
 */


#ifndef __my_USART_H__
#define __my_USART_H__

#endif

#include "usart.h"


#define lung_uint sizeof(vector)/sizeof(uint8_t)

void ESPE_USART_init(uint8_t flag_rec);
void ESPE_USART_vec_output(void);
uint8_t ESPE_USART_char_start(void);
void ESPE_USART_invert_mode(void);
void ESPE_USART_transmit_loop_vec(uint8_t start);
void ESPE_USART_transmit_loop_vec_init(uint8_t start);

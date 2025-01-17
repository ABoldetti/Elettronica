/*
 * ESPE_seriale..h
 *
 *  Created on: Oct 20, 2023
 *      Author: utente
 */

// #ifndef INC_ESPE_SERIALE__H_
// #define INC_ESPE_SERIALE__H_

extern unsigned char stringa[];
extern unsigned int vec[];
//extern unsigned int esp1[];
//extern unsigned int esp2[];

#define tau1 (uint32_t)((0.935506985)*((1)<<10))
#define tau2 (uint32_t)((0.904837418)*((1)<<10))
#define lunghezza_stringa sizeof(stringa) /sizeof(stringa[0])
#define lung_uint sizeof(vec)/sizeof(uint8_t)

extern unsigned int count_pointer;

// #endif /* INC_ESPE_SERIALE__H_ */

//Ricordiamoci che tutto quanto definito nel .c qui va ripreso con extern

void ESPE_USART_interrupt(void);
void inizializzo_USART(void);
void create_vec(void);
void exponential(void);


/*
 * ESPE_seriale..h
 *
 *  Created on: Oct 20, 2023
 *      Author: utente
 */

// #ifndef INC_ESPE_SERIALE__H_
// #define INC_ESPE_SERIALE__H_

extern unsigned char stringa[];

extern char *point;
//extern unsigned int esp1[];
//extern unsigned int esp2[];

#define lunghezza_stringa sizeof(stringa) /sizeof(stringa[0])
#define lung_uint sizeof(vector)/sizeof(uint8_t)



// #endif /* INC_ESPE_SERIALE__H_ */

//Ricordiamoci che tutto quanto definito nel .c qui va ripreso con extern

void ESPE_USART_interrupt(void);
void ESPE_USART_init(void);
void ESPE_USART_interrupt_send_vector(void);
void exponential(void);


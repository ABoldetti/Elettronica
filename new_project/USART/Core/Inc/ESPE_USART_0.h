/*
 * ESPE_seriale..h
 *
 *  Created on: Oct 20, 2023
 *      Author: utente
 */

// #ifndef INC_ESPE_SERIALE__H_
// #define INC_ESPE_SERIALE__H_

extern unsigned char stringa[];

#define lunghezza_stringa sizeof(stringa) /sizeof(stringa[0])

extern unsigned int indice;
// #endif /* INC_ESPE_SERIALE__H_ */

//Ricordiamoci che tutto quanto definito nel .c qui va ripreso con extern

void ESPE_USART_interrupt(void);
void inizializzo_USART(void);

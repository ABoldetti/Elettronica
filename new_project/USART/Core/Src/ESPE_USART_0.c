/*
 * ESPE_seriale.c
 *
 *  Created on: Oct 20, 2023
 *      Author: utente
 */
//Zona include
#include "usart.h"
#include "ESPE_USART_0.h"


unsigned char stringa[]={"Buongiorno!\r"};
unsigned int count_pointer=0;

//Zona definizione variabili, vettori globali


void inizializzo_USART(void){

	//Accendo la trasmissione

	USART3 ->CR1 |= USART_CR1_TE;

	USART3 ->CR1 |= USART_CR1_RE;
	//Abilito interrupt trasmissione: attenzione che il flag viene subito impostato ad 1
//	USART3 -> CR1 |= USART_CR1_TXEIE;
	USART3 -> CR1 |= USART_CR1_RXNEIE;
	//Abilito interruput ricezione

	//Accendo USART
	USART3 ->CR1 |= USART_CR1_UE;

}


//Zona definizione di funzioni
void ESPE_USART_interrupt(void){

	if( USART3->ISR & USART_ISR_TC){

		if( USART3->ISR & USART_ISR_RXNE_RXFNE){
			//Qui mettiamo il codice per la gestione della sola ricezione
			USART3 -> TDR = USART3 -> RDR;
		}
	}
//Per ovviare a dimenticanze azzeriamo all'uscita i flag della ricezione e trasmissione in ogni caso
	USART3->ICR |= USART_ICR_ORECF; //Cancella l'overrun. Capita quando si entra in debug
	USART3->ICR |= USART_ICR_TCCF ;  //Azzeramento flag interrupt trasmissione
	USART3->RQR |= USART_RQR_RXFRQ;  //Azzeramento flag interrupt ricezione

//Ricordiamoci di commentare la funzione introdotta dallo IDE cosi:
//
//if (0){
//	/* USER CODE END USART3_IRQn 0 */
//	HAL_UART_IRQHandler(&huart3);
//	/* USER CODE BEGIN USART3_IRQn 1 */
//}

}

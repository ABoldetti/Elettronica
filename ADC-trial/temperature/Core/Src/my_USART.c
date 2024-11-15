/*
 * ESPE_seriale.c
 *
 *  Created on: Oct 20, 2023
 *      Author: utente
 */
//Zona include
#include "usart.h"
#include "my_USART.h"


#define char_trigger (char)'e'
#define len_vec 50

uint8_t count_pointer = 0;
uint8_t triggered = 0;
uint16_t vector[len_vec];
char *point = (char *) (vector);
//Zona definizione variabili, vettori globali


void ESPE_USART_init(uint8_t flag_rec){

	//Accendo la trasmissione

	USART3 ->CR1 |= USART_CR1_TE;

	USART3 ->CR1 |= USART_CR1_RE;
	//Abilito interrupt trasmissione: attenzione che il flag viene subito impostato ad 1
	//USART3 -> CR1 |= USART_CR1_TCIE;

	if( flag_rec){
		USART3 -> CR1 |= USART_CR1_RXNEIE;
	}else{
		USART3 -> CR1 |= USART_CR1_TCIE;
	}
	//Abilito interruput ricezione/trasmissione

	//Accendo USART
	USART3 ->CR1 |= USART_CR1_UE;

}

void ESPE_USART_invert_mode(void){
	if(USART3 -> CR1 & USART_CR1_RXNEIE){
		USART3 -> CR1 &= ~USART_CR1_RXNEIE;
		USART3 -> CR1 |= USART_CR1_TCIE;
	}else if(USART3 -> CR1 & USART_CR1_TCIE){
		USART3 -> CR1 |= USART_CR1_RXNEIE;
		USART3 -> CR1 &= ~USART_CR1_TCIE;
	}
}


//Zona definizione di funzioni
uint8_t ESPE_USART_char_start(void){
	if ( USART3 ->ISR & USART_ISR_RXNE_RXFNE){
		if ( USART3 -> RDR == char_trigger){
			triggered = 1;
			ESPE_USART_invert_mode();
			count_pointer = 0;
		}
	}

//Per ovviare a dimenticanze azzeriamo all'uscita i flag della ricezione e trasmissione in ogni caso
	USART3->ICR |= USART_ICR_ORECF; //Cancella l'overrun. Capita quando si entra in debug
	USART3->ICR |= USART_ICR_TCCF ;  //Azzeramento flag interrupt trasmissione
	USART3->RQR |= USART_RQR_RXFRQ;  //Azzeramento flag interrupt ricezione

	return triggered;
//Ricordiamoci di commentare la funzione introdotta dallo IDE cosi:
//
//if (0){
//	/* USER CODE END USART3_IRQn 0 */
//	HAL_UART_IRQHandler(&huart3);
//	/* USER CODE BEGIN USART3_IRQn 1 */
//}

}










void ESPE_USART_interrupt_send_vector(){
	if (USART3->ISR & USART_ISR_TC && USART3 -> CR1 & USART_CR1_TCIE && triggered){
		if( count_pointer < lung_uint){
			USART3 -> TDR = *(point+count_pointer);
			count_pointer++;
		}
		else{
			count_pointer = 0;
			USART3 -> CR1 ^= USART_CR1_TCIE;
		}
//		if ( vector < e_max && indice == 0) ;
//		else USART3 -> CR1 ^= USART_CR1_TCIE;
	}

	//Per ovviare a dimenticanze azzeriamo all'uscita i flag della ricezione e trasmissione in ogni caso
	USART3->ICR |= USART_ICR_ORECF; //Cancella l'overrun. Capita quando si entra in debug
	USART3->ICR |= USART_ICR_TCCF ;  //Azzeramento flag interrupt trasmissione
	USART3->RQR |= USART_RQR_RXFRQ;  //Azzeramento flag interrupt ricezione


}

void ESPE_USART_transmit_loop_vec(uint8_t start){
 	if ( ((USART3->ISR & USART_ISR_TC) || count_pointer== 0) && (USART3 -> CR1 & USART_CR1_TCIE)){
		if( count_pointer < lung_uint){
			USART3 -> TDR = *(point+(( count_pointer + start )%lung_uint));
			count_pointer++;
		}
		else{
			count_pointer = 0;
			USART3 -> CR1 &= ~USART_CR1_TCIE;
		}
//		if ( vector < e_max && indice == 0) ;
//		else USART3 -> CR1 ^= USART_CR1_TCIE;
	}

	//Per ovviare a dimenticanze azzeriamo all'uscita i flag della ricezione e trasmissione in ogni caso
 	if( 1){
	USART3->ICR |= USART_ICR_ORECF; //Cancella l'overrun. Capita quando si entra in debug
	USART3->ICR |= USART_ICR_TCCF ;  //Azzeramento flag interrupt trasmissione
	USART3->RQR |= USART_RQR_RXFRQ; //Azzeramento flag interrupt ricezione
 	}

}

/*
void ESPE_USART_transmit_loop_vec_init(uint8_t start){
	USART3 -> CR1 |= USART_CR1_TCIE;
	if (USART3->ISR & USART_ISR_TC && USART3 -> CR1 & USART_CR1_TCIE){
		USART3 -> TDR = *(point+(count_pointer+start)%lung_uint);
		count_pointer++;
	}
}
*/


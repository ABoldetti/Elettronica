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
unsigned int len;
unsigned char str[32];
uint8_t flag = 0;
unsigned int counter = 0;
uint16_t vector[50];
char *point = (char *) (vector);

//Zona definizione variabili, vettori globali


void ESPE_USART_init(void){

	//Accendo la trasmissione

	USART3 ->CR1 |= USART_CR1_TE;

	USART3 ->CR1 |= USART_CR1_RE;
	//Abilito interrupt trasmissione: attenzione che il flag viene subito impostato ad 1
	//USART3 -> CR1 |= USART_CR1_TCIE;

	USART3 -> CR1 |= USART_CR1_RXNEIE;
	//Abilito interruput ricezione

	//Accendo USART
	USART3 ->CR1 |= USART_CR1_UE;

}


//Zona definizione di funzioni
void ESPE_USART_interrupt(void){
	if ( USART3 ->ISR & USART_ISR_RXNE_RXFNE){
		if ( USART3 -> RDR != '\r'){
			*(str+count_pointer) = USART3 -> RDR;
			count_pointer++;
		}else{
			if (*str == 's' && count_pointer == 1 ){
				flag = 1;
				count_pointer = 0;

			}else{
				len = count_pointer;
				count_pointer = 1;
				USART3 -> CR1 |= USART_CR1_TCIE;
				USART3 -> TDR = *(str);
			}
		}
	}
	if( USART3 ->ISR & USART_ISR_TC){
		if (len != 0 && count_pointer < len){
			USART3->TDR = *(str+count_pointer);
			count_pointer++;
		}else{
			if(count_pointer == len && len != 0){
				USART3 -> TDR = '\r';
				len = 0;
				count_pointer = 0;
				USART3 -> CR1 ^= USART_CR1_TCIE;
			}
		}
	}

	if( flag){
		USART3 -> CR1 |= USART_CR1_TCIE;
		if (count_pointer < lunghezza_stringa){
			USART3 -> TDR = *(stringa + count_pointer);
			count_pointer++;
		}else{
			flag = 0;
			count_pointer = 0;
			USART3 -> CR1 ^= USART_CR1_TCIE;


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










void ESPE_USART_interrupt_send_vector(){
	if (USART3 -> ISR & USART_ISR_RXNE_RXFNE){
		if( USART3 -> RDR != '\r'){
			*(str+count_pointer) = USART3 -> RDR;
			count_pointer++;
		}else if ( str[0] == 'e' && count_pointer == 1){
			USART3 -> CR1 |= USART_CR1_TCIE;
			ADC3 -> CR |= ADC_CR_ADSTART;
			count_pointer = 0;
		}
	}

	if (USART3->ISR & USART_ISR_TC && USART3 -> CR1 & USART_CR1_TCIE){
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


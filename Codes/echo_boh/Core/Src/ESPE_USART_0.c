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
unsigned int indice=0;
unsigned int len;
unsigned char str[32] = "";
uint8_t flag = 0;

const unsigned int tau = 12;
unsigned int exp = 1;
char *point = exp;
const unsigned int e_max = 4294967295/tau;
//Zona definizione variabili, vettori globali


void inizializzo_USART(void){

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
			*(str+indice) = USART3 -> RDR;
			indice++;
		}else{
			if (*str == 's' && indice == 1 ){
				flag = 1;
				indice = 0;
			}else{
				len = indice;
				indice = 1;
				USART3 -> CR1 |= USART_CR1_TCIE;
				USART3 -> TDR = *(str);
			}
		}
	}
	if( USART3 ->ISR & USART_ISR_TC){
		if (len != 0 && indice < len){
			USART3->TDR = *(str+indice);
			indice++;
		}else{
			if(indice == len && len != 0){
				USART3 -> TDR = '\r';
				len = 0;
				indice = 0;
				USART3 -> CR1 ^= USART_CR1_TCIE;
			}
		}
	}

	if( flag){
		USART3 -> CR1 |= USART_CR1_TCIE;
		if (indice < lunghezza_stringa){
			USART3 -> TDR = *(stringa + indice);
			indice++;
		}else{
			flag = 0;
			indice = 0;
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









void exponential( void ){
	if (USART3->ISR & USART_ISR_RXNE_RXFNE){
		if( USART3 -> RDR != '\r'){
			*(str+indice) = USART3 -> RDR;
			indice++;
		}else if ( *(str) == 'e' && indice == 1){
			str = "";
			USART3 -> CR1 |= USART_CR1_TCIE;
			USART3 -> TDR = *(point);

		}
	}

	if (USART3->ISR & USART_ISR_TC){
		if( indice < lung_uint){
			USART -> TDR = *(point+indice);
			indice++;
		}
		else{
			indice = 0;
//			USART3 -> CR1 ^= USART_CR1_TCIE;
		}
		if ( exp < e_max && indice == 0) exp *=tau;
		else USART3 -> CR1 ^= USART_CR1_TCIE;
	}

	//Per ovviare a dimenticanze azzeriamo all'uscita i flag della ricezione e trasmissione in ogni caso
		USART3->ICR |= USART_ICR_ORECF; //Cancella l'overrun. Capita quando si entra in debug
		USART3->ICR |= USART_ICR_TCCF ;  //Azzeramento flag interrupt trasmissione
		USART3->RQR |= USART_RQR_RXFRQ;  //Azzeramento flag interrupt ricezione


}





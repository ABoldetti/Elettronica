/*
 * ESPE_my_DMA.c
 *
 *  Created on: Nov 6, 2024
 *      Author: aderf
 */



#include "ESPE_my_DMA.h"
#include "dma.h"
#include "adc.h"
#include "USART.h"
#include "tim.h"


volatile uint16_t DMA_data[A];
uint8_t flag_USART = 0;
uint8_t flag_Pretriggered = 0;
uint8_t flag_Triggered = 0;
uint8_t flag_Trigger_EN = 0;

uint16_t index_stop;
uint16_t index_transmission = 0;
unsigned char *pointer = (unsigned char *)(&index_stop);




void DMA_setup_ADC(void){
	DMA1_Stream0 ->M0AR = (uint32_t) &DMA_data;
	DMA1_Stream0 ->PAR = (uint32_t) &(ADC3->DR);
	DMA1_Stream0 ->NDTR = (uint16_t) A;
	ADC3->CFGR |= ADC_CFGR_DMNGT;
	DMA1_Stream0 ->CR |= DMA_SxCR_TCIE;
}

void DMA_reset(void){
	DMA2->LIFCR = 0xffffffff;
	DMA2->HIFCR = 0xffffffff;

	DMA1->LIFCR = 0xffffffff;
	DMA1->HIFCR = 0xffffffff;

}
void DMA_setup_USART(void){
	DMA2_Stream0 ->M0AR = (uint32_t) &DMA_data;
	DMA2_Stream0 ->PAR = (uint32_t) &(USART3->TDR);
	DMA2_Stream0 ->NDTR = (uint16_t) A*2;
	USART3->CR3 |= USART_CR3_DMAT;
	DMA2_Stream0->CR |=DMA_SxCR_TCIE;
}



void ESPE_USART_init(void){

	//Accendo la trasmissione
	USART3 ->CR1 |= USART_CR1_TE;

	USART3 ->CR1 |= USART_CR1_RE;
	//Abilito interrupt trasmissione: attenzione che il flag viene subito impostato ad 1
	//USART3 -> CR1 |= USART_CR1_TCIE;

	USART3 -> CR1 |= USART_CR1_RXNEIE;
	//Abilito interruput ricezione


}


//Queste sono le operazioni che dobbiamo fare, magari con una funzione, nel main(), prima del while(1) (va fatta una sola volta)
void ESPE_ADC_init(void){

	// azzeriamo per evitare casini di configurazione
	ADC3 -> SQR1 = 0;

	// ogni numero è collegato ad un pin a sèstante
	ADC3->SQR1 |= 0 <<ADC_SQR1_L_Pos;							// ti dice quante misure deve prendere (n+1)
	ADC3->SQR1 |= 0 << ADC_SQR1_SQ1_Pos; 						// ti dice qual è la prima misura da fare
//	ADC3->SQR1 |= 19 << ADC_SQR1_SQ2_Pos; 						// ti dice qual è la seconda misura da fare




	ADC3->PCSEL |= ADC_PCSEL_PCSEL_0;		//segna quali sono i canali in lettura per velocità massima
//  ADC3->PCSEL |= ADC_PCSEL_PCSEL_19;



	ADC3 -> CR &= ~ADC_CR_DEEPPWD_Pos;						//Deep power down state (se attivo non overclocka)
	ADC3 -> CR |= 1 << ADC_CR_ADVREGEN_Pos;						//Voltage regulator activated




	ADC3 -> CR &= ~ADC_CR_ADCALDIF_Pos;							//seleziona modalità differenziata di calibrazione (a 0)
	ADC3 -> CR |= 1<< ADC_CR_ADCALLIN_Pos;	//seleziona la modalità lineare di calibrazione (a 1)
	ADC3 -> CR &= ~ADC_CR_ADEN_Pos;								//Controlliamo che l'ADC non sia acceso e che il bit sia stato resettato
	ADC3 -> CR |= 1<< ADC_CR_ADCAL_Pos;							// Inizia la calibrazione

	while( ADC3->CR & ADC_CR_ADCAL ){							//Aspetti che la calibrazione sia finita, il bit viene cambiato dall'hardware
	}



	ADC3->ISR &= ~ADC_ISR_ADRDY_Pos;							//Controlli che il bit per l'inizio della presa dati sia a 0
	ADC3->CR |= 1<<ADC_CR_ADEN_Pos;								//Attiviamo l'ADC (non la presa dati)
	while( !(ADC3->ISR & ADC_ISR_ADRDY)){						//Aspettiamo che sia setuppato correttamente
	}


	ADC3 -> IER |= ADC_IER_EOCIE;								//Attiviamo l'interrupt

	ADC3 -> SMPR1 |= 0<<ADC_SMPR1_SMP0_Pos; 					//Inseriamo un ritardo di 7 cicli prima della prima misura in modo da dare al termometro il tempo di regolarsi


}

void ESPE_TIM6_init(void){
	TIM6->CNT = 0;
	TIM6->ARR = 10;
	TIM6->PSC = 24;
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
void ESPE_USART_char_start(void){
	if ( USART3 ->ISR & USART_ISR_RXNE_RXFNE){
		if ( USART3 -> RDR == char_trigger){
			//flag_USART = 1;
			flag_Trigger_EN = 1;
			ESPE_USART_invert_mode();

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


void ESPE_DMA_switch_mode(void){
	//if(flag_USART){
	if( flag_Triggered){
		TIM6_stop;
		DMA_reset();
		DMA_start_USART;
		//flag_USART = 0;

	}
}


void ESPE_DMA_switch_back(void){
	if( !(DMA2_Stream0 -> NDTR)){
		flag_Triggered = 0;
		ESPE_USART_invert_mode();
		DMA_stop_USART;
		DMA_reset();
		DMA2_Stream0 -> NDTR = 2*A;
		TIM6_start;
	}
}

void ESPE_DMA_Trigger(void){
	if(!flag_Triggered && flag_Trigger_EN){
		//TIM6_stop;
		if( ADC3 -> DR > Trigger_Value){
			flag_Triggered = 1;
			flag_Trigger_EN = 0;
		}
		//TIM6_start;
	}
}

void ESPE_DMA_Trigger_Pretrigger(void){
	if(!flag_Triggered && flag_Trigger_EN){
		if( flag_Pretriggered){
			if( ADC3 -> DR > Trigger_Value){
				flag_Triggered = 1;
				flag_Trigger_EN = 0;
				flag_Pretriggered = 0;
				index_stop = (DMA1_Stream0 ->NDTR + data_len)%A +1000;
				return;
			}
		}
		if( ADC3 -> DR < Pretrigger_Value){
			flag_Pretriggered = 1;
		}
	}
}


void ESPE_DMA_data_manipulation(void){
	if(DMA1_Stream0 ->NDTR == (index_stop-1000)){
		ESPE_DMA_switch_mode();
	}
}

void ESPE_DMA_end_transmission(void){
	if( flag_Triggered){
		while(!(USART3->ISR & USART_ISR_TXE_TXFNF));
		if(USART3 ->CR1 & USART_CR1_TCIE){
			if( index_transmission < 2){
				USART3->TDR = pointer[index_transmission];
				index_transmission ++;
			}else{
				index_transmission = 0;
				ESPE_DMA_switch_back();
			}

		}
	}

}


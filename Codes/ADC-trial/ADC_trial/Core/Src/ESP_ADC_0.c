/*
 * ESP_ADC.c
 *
 *  Created on: Nov 6, 2023
 *      Author: utente
 */


#include "ESP_ADC_0.h"
#include "adc.h"
#include "ESPE_USART_0.h"


unsigned int counter_ADC = 0;
unsigned int voltaggio;
unsigned int conversion_counter = 0;
uint8_t flag_ADC = 0;
uint8_t conversion_flag = 0;
uint16_t temp[50];

//Passo 1:   IdeCUBE

//selezionare ADC3. Per le prime misure la freqeunza non ha importanza
//Impostare i 2 ingressi VSENSE (forse questa non la misuriamo) e VREFINT

//Queste sono le operazioni che dobbiamo fare, magari con una funzione, nel main(), prima del while(1) (va fatta una sola volta)
void ESPE_ADC_0_init(void) {//Inizializzazione ADC
//Passo 2: impostare gli ingressi per la misura

	// azzeriamo per evitare casini di configurazione
	ADC3 -> SQR1 = 0;
	ADC3->SQR1 |= 1<<ADC_SQR1_L_Pos;
	// ogni numero è collegato ad un pin a sèstante
	ADC3->SQR1 |= 18 << ADC_SQR1_SQ1_Pos; // ti dice qual è la prima misura da fare
	ADC3->SQR1 |= 19 << ADC_SQR1_SQ2_Pos; // ti dice qual è la seconda misura da fare

//1)
//selezionaimao gli ingressi ed il numero di ingressi mediante il registro SQR1



//2)
//Impostiamo anche PCSEL: dobbiamo indicare quali canali legggiamo affinchè si impsti la velocità massima degli ingressi
	ADC3->PCSEL |= ADC_PCSEL_PCSEL_18|ADC_PCSEL_PCSEL_19;

//3)
//nel CR vanno impostati DEEPPWD e ADVREGEN
	ADC3 -> CR |= 0 << ADC_CR_DEEPPWD_Pos;
	ADC3 -> CR |= 1 << ADC_CR_ADVREGEN_Pos;

//4)
//Calibrare l'ADC, agendo nel CR con i flag ADCALDIF, ADCALLIN, ADEN e ADCAL
	ADC3 -> CR |= 0<< ADC_CR_ADCALDIF_Pos;			//seleziona modalità differenziata di calibrazione (a 0)
	ADC3 -> CR |= 1<< ADC_CR_ADCALLIN_Pos;
	ADC3 -> CR |= 0<< ADC_CR_ADEN_Pos;
	ADC3 -> CR |= 1<< ADC_CR_ADCAL_Pos;				// Inizia la calibrazione

	while( ADC3->CR & ADC_CR_ADCAL ){
	}
//5)
//Accendiamo l'ADC, questo non significa effettuare misure, ADRDY nell'ISR e ADEN nel CR
	ADC3->ISR |= 0<<ADC_ISR_ADRDY_Pos;
	ADC3->CR |= 1<<ADC_CR_ADEN_Pos;
	while( !(ADC3->ISR & ADC_ISR_ADRDY)){
	}

//6)
//Abilitare l'interrupt, EOCIE nell'IER  (sarà l'EOC nell'ISR il flag di interrupt)
	ADC3 -> IER |= ADC_IER_EOCIE;
//7)
//Eventialmente inseire un ritardo per la lettura del canale dopo la connessione in SMPR1 e SMPR2
	ADC3 -> SMPR2 |= 7	 <<ADC_SMPR2_SMP18_Pos; // timer di 5 cicli
//8)
//Per potere leggere la temperatura e la tensione occorre abilitare i flag 	TSEN e VREFEN nel CCR
	ADC3_COMMON ->CCR |= ADC_CCR_TSEN;		// attiva termometro
	ADC3_COMMON ->CCR |= ADC_CCR_VREFEN;	// attiva voltaggio di riferimento



//9)
//Nel registro CRGR indicare che misura si vuole considerare, CONT: single o continua, oppure con trigger per esempio con timer, consigliato TIM6.
//oppure si può usare il trigger sw con ADSTART nel CR, consigliato all'inizio.
	ADC3 -> CFGR |= ADC_CFGR_CONT;
	ADC3 -> CFGR |= ADC_CFGR_AUTDLY;

}//Fine inizializzazione ADC

void ESPE_ADC_data(void){
	if( counter_ADC < 50){
		if( flag_ADC ){
			voltaggio = ADC3->DR;
			flag_ADC = 0;
		}else{
			temp[counter_ADC] = ADC3->DR;
			flag_ADC = 1;
			counter_ADC++;
		}
	}else{
		conversion_flag = 1;
		ADC3 -> IER &= ~(ADC_IER_EOCIE);
	}
}

void ESPE_ADC_temp_conversion(){
	extern uint16_t vector[50];
	extern unsigned int indice;
	extern char *point;
	if (conversion_counter < 50){
		vector[conversion_counter] = ((Const1)*((uint32_t)(temp[conversion_counter]) - (uint32_t)(TCAL_30C))/(TS_diff))+(Const2);
		conversion_counter++;
	}else{
		conversion_flag = 0;
		USART3 -> CR1 |= USART_CR1_TCIE;
		USART3 -> TDR = *(point);
		indice = 1;
	}
}


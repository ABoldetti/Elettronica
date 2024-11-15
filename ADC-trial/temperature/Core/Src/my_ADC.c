/*
 * ESPE_ADC3
 * libreria contenente tutte le funzioni necessarie per la lettura
 * di una temperatura
 *
 * in caso ci sia il bisogno di leggere dati da un altro sensore, cambiare i codici dai pin
 *
 *
 */

#include "my_ADC.h"
#include "adc.h"
#include "my_USART.h"




//variabili
//counter
uint8_t counter_ADC = 0;
uint8_t counter_conversion = 0;
//flag
uint8_t flag_data = 0;
uint8_t flag_conversion = 0;
uint8_t end_read = 0;
//vec
uint16_t data_vec[len_vec];
uint16_t temp_vec[len_vec];




void ESPE_ADC_TIM6_setup(void){
	TIM6->CNT = 0;
	TIM6->ARR = 1000;
	TIM6->PSC = 100;
	TIM6->CR1 |= TIM_CR1_CEN;
}


void ESPE_ADC_init(uint8_t cont_measurement){

	// azzeriamo per evitare casini di configurazione
	ADC3 -> SQR1 = 0;

	// ogni numero è collegato ad un pin a sèstante
	ADC3->SQR1 |= 0<<ADC_SQR1_L_Pos;							// ti dice quante misure deve prendere (n+1)
	ADC3->SQR1 |= 18 << ADC_SQR1_SQ1_Pos; 						// ti dice qual è la prima misura da fare
//	ADC3->SQR1 |= 19 << ADC_SQR1_SQ2_Pos; 						// ti dice qual è la seconda misura da fare




	ADC3->PCSEL |= ADC_PCSEL_PCSEL_18;		//segna quali sono i canali in lettura per velocità massima
//  ADC3->PCSEL |= ADC_PCSEL_PCSEL_19;



	ADC3 -> CR |= 0 << ADC_CR_DEEPPWD_Pos;						//Deep power down state (se attivo non overclocka)
	ADC3 -> CR |= 1 << ADC_CR_ADVREGEN_Pos;						//Voltage regulator activated




	ADC3 -> CR &= ~ADC_CR_ADCALDIF_Pos;							//seleziona modalità differenziata di calibrazione (a 0)
	ADC3 -> CR |= 1<< ADC_CR_ADCALLIN_Pos;						//seleziona la modalità lineare di calibrazione (a 1)
	ADC3 -> CR &= ~ADC_CR_ADEN_Pos;								//Controlliamo che l'ADC non sia acceso e che il bit sia stato resettato
	ADC3 -> CR |= 1<< ADC_CR_ADCAL_Pos;							// Inizia la calibrazione

	while( ADC3->CR & ADC_CR_ADCAL ){							//Aspetti che la calibrazione sia finita, il bit viene cambiato dall'hardware
	}



	ADC3->ISR &= ~ADC_ISR_ADRDY_Pos;							//Controlli che il bit per l'inizio della presa dati sia a 0
	ADC3->CR |= 1<<ADC_CR_ADEN_Pos;								//Attiviamo l'ADC (non la presa dati)
	while( !(ADC3->ISR & ADC_ISR_ADRDY)){						//Aspettiamo che sia setuppato correttamente
	}


	ADC3 -> IER |= ADC_IER_EOCIE;								//Attiviamo l'interrupt


	ADC3_COMMON ->CCR |= ADC_CCR_TSEN;							// attiva termometro
	ADC3_COMMON ->CCR |= ADC_CCR_VREFEN;						// attiva voltaggio di riferimento

	ADC3 -> SMPR2 |= 7<<ADC_SMPR2_SMP18_Pos; 					//Inseriamo un ritardo di 7 cicli prima della prima misura in modo da dare al termometro il tempo di regolarsi


	if( cont_measurement){
		ADC3 -> CFGR |= ADC_CFGR_CONT;							//Abilitiamo la lettura continua dei dati
		ADC3 -> CFGR |= ADC_CFGR_AUTDLY;						//Abilitiamo il record ritardato dei dati nel caso in cui il buffer sia ancora pieno (se DR != 0 -> misura non parte)
	}

	ADC3-> CR |= ADC_CR_ADSTART;								//Abilitiamo la presa dati

}



void ESPE_ADC_vec_read(void){
	if( counter_ADC < len_vec){
		data_vec[counter_ADC] = ADC3->DR;

		counter_ADC++;
	}else{
		flag_data = 1;
		ADC3 -> IER &= ~(ADC_IER_EOCIE);
	}
}

uint8_t ESPE_ADC_loop_vec_read(uint8_t trigger){
	if( !trigger){
		data_vec[counter_ADC] = ADC3->DR;
		if( counter_ADC < len_vec){
			counter_ADC++;
		}else{
			counter_ADC = 0;
		}
	}else{
		end_read = 1;
		ADC3 -> IER &= ~(ADC_IER_EOCIE);
		TIM6 -> CR1 &= ~( TIM_CR1_CEN);
	}
	return counter_ADC;
}



void ESPE_ADC_temp_conversion(void){
	extern uint16_t vector[len_vec];
	if (counter_conversion < len_vec && !flag_conversion){
		vector[(counter_conversion+counter_ADC)%len_vec] = ((Const1)*((uint16_t)(data_vec[(counter_conversion+counter_ADC)%len_vec]) - (uint32_t)(TCAL_30C))/(TS_diff))+(Const2);
		counter_conversion++;
	}else{
		flag_conversion = 1;

	}
}



#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "systen_delay.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_timer.h"
#include "LPC17xx_nvic.h"
#include "lpc_types.h"
void header();
void Fin_de_trame();
void envoi_message(char*[]);
void envoi0();
void envoi1();

char* T[4]={"R","90","A","90"};
LPC_TIM_TypeDef TIM0;
LPC_TIM_TypeDef TIM1;
TIM_MATCHCFG_Type match0;
PINSEL_CFG_Type pinsel01;
TIM_MATCHCFG_Type match1;
TIM_TIMERCFG_Type timer0;
TIM_TIMERCFG_Type timer1;
unsigned char flag_int=0;
int bit,i,j;

void emission (){
	{
	/*TIMER0 en timer counter en microseconde*/
	timer0.PrescaleOption=TIM_PRESCALE_USVAL;//le TC de Timer0 s'incrémente toutes les µs
	timer0.PrescaleValue=1;
	
	/*TIMER1 en timer counter en microseconde*/

	timer1.PrescaleOption=TIM_PRESCALE_USVAL;//le TC de Timer1 s'incrémente toutes les µs
	timer1.PrescaleValue=1;
	
	/*MR0 en toggle a 14 microseconde*/
		
	NVIC_EnableIRQ(TIMER1_IRQn);
		
	match0.MatchChannel=0;//match MAT0.0
	match0.ExtMatchOutputType=TIM_EXTMATCH_TOGGLE;//Timer0 en mode Toggle
	match0.MatchValue=13; //14µs pour avoir un créneau à 36kHz => 14-1 car il compte 0
	match0.ResetOnMatch=ENABLE; //RAZ du compteur lorsque l'on match	
		
	match1.MatchChannel=0;//match MAT0.0
	match1.MatchValue=9000;
	match1.ResetOnMatch=ENABLE; //RAZ du compteur lorsque l'on match
	match1.IntOnMatch=ENABLE;
	
	

	//on selectionne MAT0.O de P1.28
	pinsel01.Portnum=1; //selection du port 1
	pinsel01.Pinnum=28; //selection du pin 28
	pinsel01.Funcnum=3; //on prend la 3eme fonctionalité qui est MAT0.0
	
	PINSEL_ConfigPin(&pinsel01);//on configure la sortie du Timer0 sur la sortie MAT0.0
	
	
	
	TIM_ConfigMatch(LPC_TIM0,&match0);
	TIM_ConfigMatch(LPC_TIM1,&match1);
	TIM_Init(LPC_TIM1,TIM_TIMER_MODE,&timer1);
	TIM_Init(LPC_TIM0,TIM_TIMER_MODE,&timer0);
	//TIM_Cmd(LPC_TIM0,ENABLE);

	header();
	envoi_message(T);
	Fin_de_trame();
		
	}while(1);
	
}


void header(){
	{
	TIM_Cmd(LPC_TIM0,ENABLE);
	TIM_UpdateMatchValue(LPC_TIM1,0,9000);
	TIM_Cmd(LPC_TIM1,ENABLE);
		
	}while(!flag_int);
	flag_int=0;
	{
	GPIO_SetDir(1,(1<<28),1);
	GPIO_ClearValue(1,(0<<28));
	TIM_UpdateMatchValue(LPC_TIM1,0,4500);
	TIM_Cmd(LPC_TIM1,ENABLE);
	
		
	}while(!flag_int);
	flag_int=0;
}

void envoi_message(char** T){
	for(i=0;i<4;i++){
		for(j=0;j<8;j++){
			bit=((T[i][j])&0x01);
			if(bit==0){
				envoi0();
			}
			else{
				envoi1();
			}
		}
	}
}


void Fin_de_trame(){
	{
	TIM_Init(LPC_TIM0,TIM_TIMER_MODE,&timer0);
	TIM_Cmd(LPC_TIM0,ENABLE);
	TIM_UpdateMatchValue(LPC_TIM1,0,600);
	TIM_Cmd(LPC_TIM1,ENABLE);
		
	}while(!flag_int);
	flag_int=0;
	{
	GPIO_SetDir(1,(1<<28),1);
	GPIO_ClearValue(1,(0<<28));
	TIM_UpdateMatchValue(LPC_TIM1,0,40000);
	TIM_Cmd(LPC_TIM1,ENABLE);
		
	}while(!flag_int);
	flag_int=0;
	
}
void TIMER1_IRQHandler(){
	TIM_ClearIntPending(LPC_TIM1,TIM_MR0_INT);
	TIM_DeInit (LPC_TIM0);
	TIM_ResetCounter(LPC_TIM0);
	
	flag_int=1;
	
}

void envoi0(){
	{
	TIM_Init(LPC_TIM0,TIM_TIMER_MODE,&timer0);
	TIM_Cmd(LPC_TIM0,ENABLE);
	TIM_UpdateMatchValue(LPC_TIM1,0,600);
	TIM_Cmd(LPC_TIM1,ENABLE);
		
	}while(!flag_int);
	flag_int=0;
	{
	GPIO_SetDir(1,(1<<28),1);
	GPIO_ClearValue(1,(0<<28));
	TIM_UpdateMatchValue(LPC_TIM1,0,2000);
	TIM_Cmd(LPC_TIM1,ENABLE);
		
	}while(!flag_int);
	flag_int=0;
	
}

void envoi1(){
	{
	TIM_Init(LPC_TIM0,TIM_TIMER_MODE,&timer0);
	TIM_Cmd(LPC_TIM0,ENABLE);
	TIM_UpdateMatchValue(LPC_TIM1,0,600);
	TIM_Cmd(LPC_TIM1,ENABLE);
		
	}while(!flag_int);
	flag_int=0;
	{
	GPIO_SetDir(1,(1<<28),1);
	GPIO_ClearValue(1,(0<<28));
	TIM_UpdateMatchValue(LPC_TIM1,0,1000);
	TIM_Cmd(LPC_TIM1,ENABLE);
		
	}while(!flag_int);
	flag_int=0;
	
}

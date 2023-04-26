#include <LedsViaUart.h>
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdlib.h>
#include <stdio.h>



#define FIRSTCHAR ':'
#define LASTCHAR ';'
#define MAXPERIOD 6
#define MAXRxMessage 100
#define MAXRxBuf 1

extern uint8_t RxBuf[MAXRxBuf];
extern uint8_t RxMessage[MAXRxMessage];


uint8_t LastCharCount = 0;



uint8_t WrongSeqMes[20] = "WrongSeq \n";
uint8_t RightSeqMes[20] = "RightSeq \n";
uint8_t OutOfRangeMes[20] = "OutOfRange \n";
uint8_t EndOfParsingMes[5] = "End\n";
uint8_t ParsingMes[20] = "Parsing \n";
int CurrentPeriod = 1000;

void TimerBlinkIT(){
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_9);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

  if(huart == &huart2) {
	  HAL_UART_Receive_IT(&huart2, RxBuf, 1);
	  ParsingProtocol();
  }
}

void ParsingProtocol(){
	if (ParsingSTATUS == WAITING){
	  if(RxBuf[0] == FIRSTCHAR){
		  ParsingSTATUS = PARSING;

	  }else{
		  while(HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_TX);
		  HAL_UART_Transmit_IT(&huart2, WrongSeqMes, strlen((const char*)WrongSeqMes));
	  }
	}else if(ParsingSTATUS == PARSING){
	  if(RxBuf[0] == LASTCHAR){
		  ParsingSTATUS = EndOfParsing;
		  LastCharCount = 0;

	  }else{
		  if (LastCharCount < MAXPERIOD){
			  if (RxBuf[0] >= 0x30 && RxBuf[0] <= 0x39){
				  RxMessage[LastCharCount++] = RxBuf[0];
			  }else{
				  ParsingSTATUS = WAITING;
				  LastCharCount = 0;
			  }
		  } else{
			  LastCharCount = 0;
			  ParsingSTATUS = WAITING;
		  }
	  }
	}
}


void ChangeDelayBlinking(){
	if (ParsingSTATUS == EndOfParsing){
		ParsingSTATUS = WAITING;

		int channel = RxMessage[0] - '0';
		char periodChar[MAXPERIOD] = {};
		uint8_t temp[200] = "";

		for (int i = 1; i < strlen((const char*)RxMessage); i++){
		 periodChar[i - 1] = RxMessage[i];
		}
		int periodInt = atoi(periodChar);


		sprintf((char *)temp, "Status: %s RxMes: %s \n",(char *)EndOfParsingMes, (char *)RxMessage);
		while(HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_TX);
		HAL_UART_Transmit_IT(&huart2, temp, strlen((const char*)temp));

		if (channel == 0){
			htim6.Init.Period = periodInt;
			HAL_TIM_Base_Init(&htim6);
			HAL_TIM_Base_Start_IT(&htim6);
		}else if(channel == 1){

		}
		memset(RxMessage, 0, MAXRxMessage*sizeof(RxMessage[0]));
	}
}
















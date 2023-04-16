#include <LedsViaUart.h>
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#define FIRSTCHAR ':'
#define LASTCHAR ';'

extern uint8_t RxBuf[1];
extern uint8_t RxMessage[100];
uint8_t LastCharCount = 0;


uint8_t WrongSeqMes[50] = "WrongSeq";
uint8_t RightSeqMes[50] = "RightSeq";
uint8_t OutOfRangeMes[50] = "OutOfRange";
uint8_t EndOfParsingMes[50] = "EndOfParsing";
uint8_t ParsingMes[50] = "Parsing";
int CurrentPeriod = 1000;

void TimerBlinkIT(){
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_9);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

  if(huart == &huart2) {
	  //HAL_UART_Transmit_IT(&huart2, buf, 1);
	  //HAL_UART_Transmit(&huart2, test, strlen((const char*)test), 100);
	  HAL_UART_Receive_IT(&huart2, RxBuf, 1);

//	  if (RxBuf[0] == ':' && RxBuf[3] == ';'){
//		  HAL_UART_Transmit(&huart2, RightSeqMes, strlen((const char*)RightSeqMes), 100);
//		  HAL_FLASH_Program_IT(FLASH_CR_PG, 0x0801F800, 1);
//	  }else{
//		  HAL_UART_Transmit(&huart2, WrongSeqMes, strlen((const char*)WrongSeqMes), 100);
//	  }
	  //HAL_UART_Transmit(&huart2, ParsingMes, strlen((const char*)ParsingMes), 100);
	  if (ParsingSTATUS == WAITING){
		  if(RxBuf[0] == FIRSTCHAR){
			  ParsingSTATUS = PARSING;
			  HAL_UART_Transmit(&huart2, ParsingMes, strlen((const char*)ParsingMes), 100);
		  }
	  }else if(ParsingSTATUS == PARSING){
		  if(RxBuf[0] == LASTCHAR){
		  	  ParsingSTATUS = EndOfParsing;
		  	  HAL_UART_Transmit(&huart2, EndOfParsingMes, strlen((const char*)EndOfParsingMes), 100);
		  }else{
			  if (LastCharCount < 100){
				  RxMessage[LastCharCount++] = RxBuf[0];
			  } else{
				  HAL_UART_Transmit(&huart2, OutOfRangeMes, strlen((const char*)OutOfRangeMes), 100);
				  LastCharCount = 0;
				  ParsingSTATUS = WAITING;
			  }
		  }
	  }
  }
}


void ChangeDelayBlinking(){
	if (ParsingSTATUS == EndOfParsing){
		 TIM6->ARR = CurrentPeriod;
		 ParsingSTATUS = WAITING;
	}
}
















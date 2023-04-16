#include <LedsViaUart.h>
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdlib.h>

#define FIRSTCHAR ':'
#define LASTCHAR ';'

extern uint8_t RxBuf[1];
extern uint8_t RxMessage[100];
uint8_t LastCharCount = 0;


uint8_t WrongSeqMes[50] = "WrongSeq\n";
uint8_t RightSeqMes[50] = "RightSeq\n";
uint8_t OutOfRangeMes[50] = "OutOfRange\n";
uint8_t EndOfParsingMes[50] = "EndOfParsing\n";
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
			  HAL_UART_Transmit_IT(&huart2, ParsingMes, strlen((const char*)ParsingMes));
		  }else{
			  HAL_UART_Transmit_IT(&huart2, WrongSeqMes, strlen((const char*)WrongSeqMes));
		  }
	  }else if(ParsingSTATUS == PARSING){
		  if(RxBuf[0] == LASTCHAR){
			  ParsingSTATUS = EndOfParsing;
			  //HAL_UART_Transmit_IT(&huart2, RxMessage, strlen((const char*)RxMessage));
			  //HAL_UART_Transmit_IT(&huart2, EndOfParsingMes, strlen((const char*)EndOfParsingMes));

			  uint8_t temp[50] = {};
			  strcat((char *)temp, (char *)EndOfParsingMes);
			  strcat((char *)temp, (char *)RxMessage);

			  HAL_UART_Transmit_IT(&huart2, temp, strlen((const char*)temp));
			  LastCharCount = 0;
		  }else{
			  if (LastCharCount < 100){
				  RxMessage[LastCharCount++] = RxBuf[0];
				  HAL_UART_Transmit_IT(&huart2, ParsingMes, strlen((const char*)ParsingMes));
			  } else{
				  HAL_UART_Transmit_IT(&huart2, OutOfRangeMes, strlen((const char*)OutOfRangeMes));
				  LastCharCount = 0;
				  ParsingSTATUS = WAITING;
			  }
		  }
	  }
  }
}


void ChangeDelayBlinking(){
	if (ParsingSTATUS == EndOfParsing){
		 ParsingSTATUS = WAITING;

		 int period = atoi((const char*)RxMessage);
		 TIM6->ARR = period;
	}
}
















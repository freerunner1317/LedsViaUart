#ifndef __LEDSVIAUART_H__
#define __LEDSVIAUART_H__

#ifdef __cplusplus
extern "C" {
#endif


void ChangeDelayBlinking();

typedef enum
{
	WAITING = 		0x00,
	PARSING = 		0x01,
	EndOfParsing =  0x02
}ParSTATUS;

ParSTATUS ParsingSTATUS = WAITING;


#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

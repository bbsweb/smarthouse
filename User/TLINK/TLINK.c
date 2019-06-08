#include "TLINK.h"
#include "esp8266.h"
#include "led.h"

void TlinkProcess(void)
{
	if(esp8266SearchBuffer("#STATUS"))
	{
		if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))
		{
		  esp8266TCPSend("#0#");
		}
		else
		{
			esp8266TCPSend("#1#");
		}
	}
  else if(esp8266SearchBuffer("LED_ON"))
	{
		LED(LED_ON);
	}
	else if(esp8266SearchBuffer("LED_OFF"))
	{
		LED(LED_OFF);
	}
}

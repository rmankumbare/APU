#include "headers.h"
#include "stdint.h"
#include "my_uart.h"
uint8_t read_data;
unsigned char UART1_readchar( void)
{
     while( !(USART1_SR & 1<<5) );
     read_data=USART1_DR;
     return read_data;
}
void UART1_Sendchar(unsigned char send_data)
{
     USART1_DR= send_data;
     while( !((USART1_SR & 1<<6))); //&& (USART1_SR & 1<<7)) );
}
void UART1_PrintString(unsigned char *pcString)
{
  int i = 0;
  while (pcString[i] != 0)    // loop through until reach string's zero terminator
   {
     UART1_Sendchar(pcString[i]); // print each character
     i++;
   }
}

//-----------------------------UART 6---------------------------------------------------------------------------------------------------------------
unsigned char UART6_readchar( void)
{
     while( !(USART6_SR & 1<<5) );
     read_data=USART6_DR;
     return read_data;
}
void UART6_Sendchar(unsigned char send_data)
{
     USART6_DR= send_data;
     while( !((USART6_SR & 1<<6))); //&& (USART6_SR & 1<<7)) );
}
void UART6_PrintString(unsigned char *pcString)
{
  int i = 0;
  while (pcString[i] != 0)    // loop through until reach string's zero terminator
   {
     UART6_Sendchar(pcString[i]); // print each character
     i++;
   }
}